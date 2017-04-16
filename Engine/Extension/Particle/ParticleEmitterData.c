/*
 *
 *
 *  Created on: 2013-1-22
 *  Author: scott.cgi
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



static inline void ReadFindString(char* buffer, ArrayRange* range, ArrayRange* line, const char* name)
{
	ABufferReader->ReadLine(buffer, range, line);
	bool isFound = ABufferReader->TryFindString(buffer, line, name);
	ALogA(isFound, "Can't find string = %s", name);
}


static inline bool ReadBool(char* buffer, ArrayRange* range, ArrayRange* line, const char* name)
{
	ReadFindString(buffer, range, line, name);
	return ABufferReader->TryFindString(buffer, line, "true");
}


static inline int ReadInt(char* buffer, ArrayRange* range, ArrayRange* line, const char* name)
{
	ReadFindString(buffer, range, line, name);
	return atoi(buffer + line->start);
}


static inline float ReadFloat(char* buffer, ArrayRange* range, ArrayRange* line, const char* name)
{
	ReadFindString(buffer, range, line, name);
	return atof(buffer + line->start);
}


static float RandomLowValue(ParticleRangedValue* rangedValue)
{
	return rangedValue->lowMin + (rangedValue->lowMax - rangedValue->lowMin) * AMathRandom();
}


static float RandomHighValue(ParticleScaledValue* scaledValue)
{
	return scaledValue->highMin + (scaledValue->highMax - scaledValue->highMin) * AMathRandom();
}


static float GetScale(ParticleScaledValue* scaledValue, float percent)
{
    int    endIndex  = -1;
    int    length    = scaledValue->timelineArr->length;
    float* timelines = AArrayGetData(scaledValue->timelineArr, float);

    // timeline is x axis points
    for (int i = 1; i < length; i++)
    {
        if (timelines[i] > percent)
        {
            endIndex = i; // find timeline after percent
            break;
        }
    }

    float* scalings = AArrayGetData(scaledValue->scalingArr, float);

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


static void GetRGB(ParticleRGBValue* rgbValue, float percent, float out_param rgb[3])
{
    int    startIndex   =  0;
    int    endIndex     = -1;
    int    length       = rgbValue->timelineArr->length;
    float* timelines    = AArrayGetData(rgbValue->timelineArr, float);

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

    float* rgbs     = AArrayGetData(rgbValue->rgbArr, float);

    rgb[0]          = rgbs[startIndex];
    rgb[1]          = rgbs[startIndex + 1];
    rgb[2]          = rgbs[startIndex + 2];

    if (endIndex == -1)
    {
        return;
    }

    float ratio     = (percent - startTime) / (timelines[endIndex] - startTime);
    endIndex       *= 3;

    rgb[0]         += (rgbs[endIndex]     - rgb[0]) * ratio;
    rgb[1]         += (rgbs[endIndex + 1] - rgb[1]) * ratio;
    rgb[2]         += (rgbs[endIndex + 2] - rgb[2]) * ratio;
}


static inline void LoadRangedValue(char* buffer, ArrayRange* range, ArrayRange* line, ParticleRangedValue* rangedValue, bool isActive)
{
	rangedValue->isActive = isActive;
	ALogD("active = %d", rangedValue->isActive);

	if (rangedValue->isActive)
	{
		rangedValue->lowMin = ReadFloat(buffer, range, line, "lowMin:");
		ALogD("lowMin = %f", rangedValue->lowMin);

		rangedValue->lowMax = ReadFloat(buffer, range, line, "lowMax:");
		ALogD("lowMax = %f", rangedValue->lowMax);
	}
}


static inline void LoadScaledValue(char* buffer, ArrayRange* range, ArrayRange* line, ParticleScaledValue* scaledValue, bool isActive)
{
	LoadRangedValue(buffer, range, line, scaledValue->rangedValue, isActive);

	if (scaledValue->rangedValue->isActive)
	{
		scaledValue->highMin  = ReadFloat(buffer, range, line, "highMin:");
		ALogD("highMin = %f", scaledValue->highMin);

		scaledValue->highMax  = ReadFloat(buffer, range, line, "highMax:");
		ALogD("highMax = %f", scaledValue->highMax);

		scaledValue->isRelative = ReadBool(buffer, range, line, "relative:");
		ALogD("relative = %d", scaledValue->isRelative);

		int length  = ReadInt(buffer, range, line, "scalingCount:");
		ALogD("scalingCount = %d", length);

		scaledValue->scalingArr = AArray->Create(sizeof(float), length);
		float* scalings         = AArrayGetData(scaledValue->scalingArr, float);

		char scaling[sizeof("scaling") + 4];
		for (int i = 0; i < length; i++)
		{
			sprintf(scaling, "scaling%d:", i);
			scalings[i] = ReadFloat(buffer, range, line, scaling);
			ALogD("scaling[%d] = %f", i, scalings[i]);
		}

		length = ReadInt(buffer, range, line, "timelineCount:");
		ALogD("timelineCount = %d", length);

		scaledValue->timelineArr = AArray->Create(sizeof(float), length);
		float* timelines         = AArrayGetData(scaledValue->timelineArr, float);

		char timeline[sizeof("timeline") + 4];
        for (int i = 0; i < length; i++)
        {
        	sprintf(timeline, "timeline%d:", i);
        	timelines[i] = ReadFloat(buffer, range, line, timeline);
			ALogD("timeline[%d] = %f", i, timelines[i]);
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
	int length = ReadInt(buffer, range, line, "colorsCount:");
	ALogD("colorsCount = %d", length);

	rgbValue->rgbArr = AArray->Create(sizeof(float), length);
	float* rgbs      = AArrayGetData(rgbValue->rgbArr, float);

	char colors[sizeof("colors") + 4];
    for (int i = 0; i < length; i++)
    {
    	sprintf(colors, "colors%d:", i);
    	rgbs[i] = ReadFloat(buffer, range, line, colors);
    	ALogD("colors[%d] = %f", i, rgbs[i]);
    }


    length = ReadInt(buffer, range, line, "timelineCount:");
	ALogD("timelineCount = %d", length);

	rgbValue->timelineArr = AArray->Create(sizeof(float), length);
	float* timelines      = AArrayGetData(rgbValue->timelineArr, float);

	char timeline[sizeof("timeline") + 4];
    for (int i = 0; i < length; i++)
    {
    	sprintf(timeline, "timeline%d:", i);
    	timelines[i] = ReadFloat(buffer, range, line, timeline);
		ALogD("timeline[%d] = %f", i, timelines[i]);
    }
}


#define args            buffer, range, line
#define ReadActive()    ReadBool(args, "active:")
#define ReadString(str) ReadFindString(args, str)


static void Init(const char* filePath, ParticleEmitterData* out_param emitterData)
{
	long  size;
	char* buffer = AFileTool->ReadBufferPlatform(filePath, &size);

	ArrayRange range[1] = {0, size - 1};
	ArrayRange line [1];

	ABufferReader->ReadLine(args);

	ALogD("delayValue");
	ReadString("Delay");
	LoadRangedValue(args, emitterData->delayValue, ReadActive());

	// use second
	emitterData->delayValue->lowMax /= 1000.0f;
	emitterData->delayValue->lowMin /= 1000.0f;


	ALogD("durationValue");
	ReadString("Duration");
	LoadRangedValue(args, emitterData->durationValue, true);

	// use second
	emitterData->durationValue->lowMax /= 1000.0f;
	emitterData->durationValue->lowMin /= 1000.0f;

	ReadString("Count");
	emitterData->minParticleCount = ReadInt(args, "min:");
	emitterData->maxParticleCount = ReadInt(args, "max:");
	ALogD
	(
		"countValue min = %d, max = %d",
		 emitterData->minParticleCount,
		 emitterData->maxParticleCount
	);

	ALogD("emissionValue");
	ReadString("Emission");
	LoadScaledValue(args, emitterData->emissionValue, true);

	ALogD("lifeValue");
	ReadString("Life");
	LoadScaledValue(args, emitterData->lifeValue, true);

	// use second
	emitterData->lifeValue->highMax /= 1000.0f;
	emitterData->lifeValue->highMin /= 1000.0f;

	ALogD("lifeOffsetValue");
	ReadString("Life Offset");
	LoadScaledValue(args, emitterData->lifeOffsetValue, ReadActive());

	// use second
	emitterData->lifeOffsetValue->highMax /= 1000.0f;
	emitterData->lifeOffsetValue->highMin /= 1000.0f;

	ALogD("xOffsetValue");
	ReadString("X Offset");
	LoadScaledValue(args, emitterData->xOffsetValue, ReadActive());

	ALogD("yOffsetValue");
	ReadString("Y Offset");
	LoadScaledValue(args, emitterData->yOffsetValue, ReadActive());

	ReadString("Spawn Shape");
	ReadString("shape:");

	ALogD("spawnWidthValue");
	ReadString("Spawn Width");
	LoadScaledValue(args, emitterData->spawnWidthValue, true);

	ALogD("spawnHeightValue");
	ReadString("Spawn Height");
	LoadScaledValue(args, emitterData->spawnHeightValue, true);

	ALogD("scaleValue");
	ReadString("Scale");
	LoadScaledValue(args, emitterData->scaleValue, true);

	ALogD("velocityValue");
	ReadString("Velocity");
	LoadScaledValue(args, emitterData->velocityValue, ReadActive());

	ALogD("angleValue");
	ReadString("Angle");
	LoadScaledValue(args, emitterData->angleValue, ReadActive());

	ALogD("rotationValue");
	ReadString("Rotation");
	LoadScaledValue(args, emitterData->rotationValue, ReadActive());

	ALogD("windValue");
	ReadString("Wind");
	LoadScaledValue(args, emitterData->windValue, ReadActive());

	ALogD("gravityValue");
	ReadString("Gravity");
	LoadScaledValue(args, emitterData->gravityValue, ReadActive());

	ALogD("ParticleRGBValue");
	ReadString("Tint");
	LoadRGBValue(args, emitterData->rgbValue);

	ALogD("transparencyValue");
	ReadString("Transparency");
	LoadScaledValue(args, emitterData->transparencyValue, true);

	ReadString("Options");
	ALogD("attached = %d", ReadBool(args, "attached:"));

	emitterData->isContinuous = ReadBool(args, "continuous:");
	emitterData->isAligned    = ReadBool(args, "aligned:");
	emitterData->isAdditive   = ReadBool(args, "additive:");
	ALogD
	(
		"continuous = %d, aligned = %d, additive = %d",
		emitterData->isContinuous,
		emitterData->isAligned,
		emitterData->isAdditive
	);

	ReadBool(args, "behind:");
	ReadBool(args, "premultipliedAlpha:");

	ReadString("Image Path");

	free(buffer);
}


#undef args
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


static ParticleEmitterData* Create(const char* filePath)
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
