/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-1-22
 * Author: scott.cgi
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Engine/Toolkit/Head/ArrayRange.h"
#include "Engine/Extension/Particle/ParticleEmitterData.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/BufferReader.h"
#include "Engine/Toolkit/Utils/FileTool.h"


static inline void ReadFindString(char* buffer, ArrayRange* range, ArrayRange* line, char* name)
{
    ABufferReader->ReadLine(buffer, range, line);
    bool isFound = ABufferReader->TryFindString(buffer, line, name);
    ALog_A(isFound, "AParticleEmitterData can not find string = %s", name);
}


static inline bool ReadBool(char* buffer, ArrayRange* range, ArrayRange* line, char* name)
{
    ReadFindString(buffer, range, line, name);
    return ABufferReader->TryFindString(buffer, line, "true");
}


static inline int ReadInt(char* buffer, ArrayRange* range, ArrayRange* line, char* name)
{
    ReadFindString(buffer, range, line, name);
    return atoi(buffer + line->start);
}


static inline float ReadFloat(char* buffer, ArrayRange* range, ArrayRange* line, char* name)
{
    ReadFindString(buffer, range, line, name);
    return atof(buffer + line->start);
}


static float RandomLowValue(ParticleRangedValue* rangedValue)
{
    return rangedValue->lowMin + (rangedValue->lowMax - rangedValue->lowMin) * AMath_Random();
}


static float RandomHighValue(ParticleScaledValue* scaledValue)
{
    return scaledValue->highMin + (scaledValue->highMax - scaledValue->highMin) * AMath_Random();
}


static float GetScale(ParticleScaledValue* scaledValue, float percent)
{
    int    endIndex  = -1;
    int    length    = scaledValue->timelineArr->length;
    float* timelines = AArray_GetData(scaledValue->timelineArr, float);

    // timeline is x axis points
    for (int i = 1; i < length; i++)
    {
        if (timelines[i] > percent)
        {
            endIndex = i; // find timeline after percent
            break;
        }
    }

    float* scalings = AArray_GetData(scaledValue->scalingArr, float);

    if (endIndex == -1)
    {
        // percent out of timeline , use last scaling
        // scaling is y axis points
        return scalings[length - 1];
    }

    int   startIndex = endIndex - 1;
    float startValue = scalings [startIndex];
    float startTime  = timelines[startIndex];

    // start value add value rate in percent
    return startValue             +
           (scalings[endIndex]    - startValue) *
           ((percent - startTime) /
           (timelines[endIndex]   - startTime));
}


static void GetRGB(ParticleRGBValue* rgbValue, float percent, float outRGB[3])
{
    int    startIndex   =  0;
    int    endIndex     = -1;
    int    length       = rgbValue->timelineArr->length;
    float* timelines    = AArray_GetData(rgbValue->timelineArr, float);

    for (int i = 1; i < length; i++)
    {
        if (timelines[i] > percent)
        {
            endIndex = i;
            break;
        }

        // before percent timeline
        startIndex = i;
    }

    float startTime = timelines[startIndex];
    startIndex     *= 3;

    float* rgbs     = AArray_GetData(rgbValue->rgbArr, float);

    outRGB[0]       = rgbs[startIndex];
    outRGB[1]       = rgbs[startIndex + 1];
    outRGB[2]       = rgbs[startIndex + 2];

    if (endIndex == -1)
    {
        return;
    }

    float ratio     = (percent - startTime) / (timelines[endIndex] - startTime);
    endIndex       *= 3;

    outRGB[0]      += (rgbs[endIndex]     - outRGB[0]) * ratio;
    outRGB[1]      += (rgbs[endIndex + 1] - outRGB[1]) * ratio;
    outRGB[2]      += (rgbs[endIndex + 2] - outRGB[2]) * ratio;
}


static inline void LoadRangedValue(char* buffer, ArrayRange* range, ArrayRange* line, ParticleRangedValue* rangedValue, bool isActive)
{
    rangedValue->isActive = isActive;

    if (rangedValue->isActive)
    {
        rangedValue->lowMin = ReadFloat(buffer, range, line, "lowMin:");
        rangedValue->lowMax = ReadFloat(buffer, range, line, "lowMax:");
    }
}


static inline void LoadScaledValue(char* buffer, ArrayRange* range, ArrayRange* line, ParticleScaledValue* scaledValue, bool isActive)
{
    LoadRangedValue(buffer, range, line, scaledValue->rangedValue, isActive);

    if (scaledValue->rangedValue->isActive)
    {
        scaledValue->highMin    = ReadFloat(buffer, range, line, "highMin:");
        scaledValue->highMax    = ReadFloat(buffer, range, line, "highMax:");
        scaledValue->isRelative = ReadBool(buffer, range, line, "relative:");

        int    length           = ReadInt(buffer, range, line, "scalingCount:");

        scaledValue->scalingArr = AArray->Create(sizeof(float), length);
        float* scalings         = AArray_GetData(scaledValue->scalingArr, float);

        char scaling[sizeof("scaling") + 4];
        for (int i = 0; i < length; i++)
        {
            sprintf(scaling, "scaling%d:", i);
            scalings[i] = ReadFloat(buffer, range, line, scaling);
        }

        length                   = ReadInt(buffer, range, line, "timelineCount:");

        scaledValue->timelineArr = AArray->Create(sizeof(float), length);
        float* timelines         = AArray_GetData(scaledValue->timelineArr, float);

        char timeline[sizeof("timeline") + 4];
        for (int i = 0; i < length; i++)
        {
            sprintf(timeline, "timeline%d:", i);
            timelines[i] = ReadFloat(buffer, range, line, timeline);
        }

    }
    else
    {
        scaledValue->scalingArr  = NULL;
        scaledValue->timelineArr = NULL;
    }
}


static inline void LoadRGBValue(char* buffer, ArrayRange* range, ArrayRange* line, ParticleRGBValue* rgbValue)
{
    int    length    = ReadInt(buffer, range, line, "colorsCount:");
    rgbValue->rgbArr = AArray->Create(sizeof(float), length);
    float* rgbs      = AArray_GetData(rgbValue->rgbArr, float);

    char colors[sizeof("colors") + 4];
    for (int i = 0; i < length; i++)
    {
        sprintf(colors, "colors%d:", i);
        rgbs[i] = ReadFloat(buffer, range, line, colors);
    }

    length                = ReadInt(buffer, range, line, "timelineCount:");

    rgbValue->timelineArr = AArray->Create(sizeof(float), length);
    float* timelines      = AArray_GetData(rgbValue->timelineArr, float);

    char timeline[sizeof("timeline") + 4];
    for (int i = 0; i < length; i++)
    {
        sprintf(timeline, "timeline%d:", i);
        timelines[i] = ReadFloat(buffer, range, line, timeline);
    }
}


#define ARGS            buffer, range, line
#define ReadActive()    ReadBool(ARGS, "active:")
#define ReadString(str) ReadFindString(ARGS, str)


static void Init(char* filePath, ParticleEmitterData* outEmitterData)
{
    long  size;
    char* buffer = AFileTool->CreateDataFromRes(filePath, &size);

    ArrayRange range[1] = {0, (int) size - 1};
    ArrayRange line [1];

    ABufferReader->ReadLine(ARGS);

    ReadString("Delay");
    LoadRangedValue(ARGS, outEmitterData->delayValue, ReadActive());

    // use second
    outEmitterData->delayValue->lowMax /= 1000.0f;
    outEmitterData->delayValue->lowMin /= 1000.0f;


    ReadString("Duration");
    LoadRangedValue(ARGS, outEmitterData->durationValue, true);

    // use second
    outEmitterData->durationValue->lowMax /= 1000.0f;
    outEmitterData->durationValue->lowMin /= 1000.0f;

    ReadString("Count");
    outEmitterData->minParticleCount = ReadInt(ARGS, "min:");
    outEmitterData->maxParticleCount = ReadInt(ARGS, "max:");

    ReadString("Emission");
    LoadScaledValue(ARGS, outEmitterData->emissionValue, true);

    ReadString("Life");
    LoadScaledValue(ARGS, outEmitterData->lifeValue, true);

    // use second
    outEmitterData->lifeValue->highMax /= 1000.0f;
    outEmitterData->lifeValue->highMin /= 1000.0f;

    ReadString("Life Offset");
    LoadScaledValue(ARGS, outEmitterData->lifeOffsetValue, ReadActive());

    // use second
    outEmitterData->lifeOffsetValue->highMax /= 1000.0f;
    outEmitterData->lifeOffsetValue->highMin /= 1000.0f;

    ReadString("X Offset");
    LoadScaledValue(ARGS, outEmitterData->xOffsetValue, ReadActive());

    ReadString("Y Offset");
    LoadScaledValue(ARGS, outEmitterData->yOffsetValue, ReadActive());

    ReadString("Spawn Shape");
    ReadString("shape:");

    ReadString("Spawn Width");
    LoadScaledValue(ARGS, outEmitterData->spawnWidthValue, true);

    ReadString("Spawn Height");
    LoadScaledValue(ARGS, outEmitterData->spawnHeightValue, true);

    ReadString("Scale");
    LoadScaledValue(ARGS, outEmitterData->scaleValue, true);

    ReadString("Velocity");
    LoadScaledValue(ARGS, outEmitterData->velocityValue, ReadActive());

    ReadString("Angle");
    LoadScaledValue(ARGS, outEmitterData->angleValue, ReadActive());

    ReadString("Rotation");
    LoadScaledValue(ARGS, outEmitterData->rotationValue, ReadActive());

    ReadString("Wind");
    LoadScaledValue(ARGS, outEmitterData->windValue, ReadActive());

    ReadString("Gravity");
    LoadScaledValue(ARGS, outEmitterData->gravityValue, ReadActive());

    ReadString("Tint");
    LoadRGBValue(ARGS, outEmitterData->rgbValue);

    ReadString("Transparency");
    LoadScaledValue(ARGS, outEmitterData->transparencyValue, true);

    ReadString("Options");

    ReadBool(ARGS, "attached:");

    outEmitterData->isContinuous = ReadBool(ARGS, "continuous:");
    outEmitterData->isAligned    = ReadBool(ARGS, "aligned:");
    outEmitterData->isAdditive   = ReadBool(ARGS, "additive:");

    ReadBool(ARGS, "behind:");
    ReadBool(ARGS, "premultipliedAlpha:");

    ReadString("Image Path");

    free(buffer);
}


#undef ARGS
#undef ReadActive
#undef ReadString


static void Release(ParticleEmitterData* data)
{
    free(data->lifeOffsetValue->scalingArr);
    free(data->lifeOffsetValue->timelineArr);
    data->lifeOffsetValue->scalingArr    = NULL;
    data->lifeOffsetValue->timelineArr   = NULL;

    free(data->lifeValue->scalingArr);
    free(data->lifeValue->timelineArr);
    data->lifeValue->scalingArr          = NULL;
    data->lifeValue->timelineArr         = NULL;

    free(data->emissionValue->scalingArr);
    free(data->emissionValue->timelineArr);
    data->emissionValue->scalingArr      = NULL;
    data->emissionValue->timelineArr     = NULL;

    free(data->scaleValue->scalingArr);
    free(data->scaleValue->timelineArr);
    data->scaleValue->scalingArr         = NULL;
    data->scaleValue->timelineArr        = NULL;

    free(data->rotationValue->scalingArr);
    free(data->rotationValue->timelineArr);
    data->rotationValue->scalingArr      = NULL;
    data->rotationValue->timelineArr     = NULL;

    free(data->velocityValue->scalingArr);
    free(data->velocityValue->timelineArr);
    data->velocityValue->scalingArr      = NULL;
    data->velocityValue->timelineArr     = NULL;

    free(data->angleValue->scalingArr);
    free(data->angleValue->timelineArr);
    data->angleValue->scalingArr         = NULL;
    data->angleValue->timelineArr        = NULL;

    free(data->windValue->scalingArr);
    free(data->windValue->timelineArr);
    data->windValue->scalingArr          = NULL;
    data->windValue->timelineArr         = NULL;

    free(data->gravityValue->scalingArr);
    free(data->gravityValue->timelineArr);
    data->gravityValue->scalingArr       = NULL;
    data->gravityValue->timelineArr      = NULL;

    free(data->transparencyValue->scalingArr);
    free(data->transparencyValue->timelineArr);
    data->transparencyValue->scalingArr  = NULL;
    data->transparencyValue->timelineArr = NULL;

    free(data->xOffsetValue->scalingArr);
    free(data->xOffsetValue->timelineArr);
    data->xOffsetValue->scalingArr       = NULL;
    data->xOffsetValue->timelineArr      = NULL;

    free(data->yOffsetValue->scalingArr);
    free(data->yOffsetValue->timelineArr);
    data->yOffsetValue->scalingArr       = NULL;
    data->yOffsetValue->timelineArr      = NULL;

    free(data->spawnWidthValue->scalingArr);
    free(data->spawnWidthValue->timelineArr);
    data->spawnWidthValue->scalingArr    = NULL;
    data->spawnWidthValue->timelineArr   = NULL;

    free(data->spawnHeightValue->scalingArr);
    free(data->spawnHeightValue->timelineArr);
    data->spawnHeightValue->scalingArr   = NULL;
    data->spawnHeightValue->timelineArr  = NULL;

    free(data->rgbValue->rgbArr);
    free(data->rgbValue->timelineArr);
    data->rgbValue->rgbArr               = NULL;
    data->rgbValue->timelineArr          = NULL;
}


static ParticleEmitterData* Create(char* filePath)
{
    ParticleEmitterData* emitterData = (ParticleEmitterData*) malloc(sizeof(ParticleEmitterData));
    Init(filePath, emitterData);

    return emitterData;
}


struct AParticleEmitterData AParticleEmitterData[1] =
{
    Create,
    Init,
    Release,
    RandomLowValue,
    RandomHighValue,
    GetScale,
    GetRGB,
};
