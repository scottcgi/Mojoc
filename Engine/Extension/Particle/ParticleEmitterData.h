/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2013-1-22
 * Author : scott.cgi
 */

#ifndef particle_emitter_data_h
#define particle_emitter_data_h


#include <stdbool.h>
#include "Engine/Toolkit/Utils/Array.h"


typedef struct
{
	bool  isActive;
	float lowMin;
	float lowMax;
}
ParticleRangedValue;


typedef struct
{
    ParticleRangedValue rangedValue[1];
	float               highMin;
	float               highMax;
	bool                isRelative;
	Array(float)*       scalingArr;
	Array(float)*       timelineArr;
}
ParticleScaledValue;


typedef struct
{
	Array(float)* rgbArr;
	Array(float)* timelineArr;
}
ParticleRGBValue;


typedef struct
{
	ParticleRangedValue  delayValue       [1];
	ParticleScaledValue  lifeOffsetValue  [1];
	ParticleRangedValue  durationValue    [1];
	ParticleScaledValue  lifeValue        [1];
	ParticleScaledValue  emissionValue    [1];
	ParticleScaledValue  scaleValue       [1];
	ParticleScaledValue  rotationValue    [1];
	ParticleScaledValue  velocityValue    [1];
	ParticleScaledValue  angleValue       [1];
	ParticleScaledValue  windValue        [1];
	ParticleScaledValue  gravityValue     [1];
	ParticleScaledValue  transparencyValue[1];
	ParticleScaledValue  xOffsetValue     [1];
	ParticleScaledValue  yOffsetValue     [1];
	ParticleScaledValue  spawnWidthValue  [1];
	ParticleScaledValue  spawnHeightValue [1];
	ParticleRGBValue     rgbValue         [1];

	int                   minParticleCount;
	int                   maxParticleCount;

	/**
	 * The emitter restart when timeover
	 */
	bool                  isContinuous;

	/**
	 * The particle angle is added to the rotation
	 */
	bool                  isAligned;

	/**
	 * The color blend additive
	 */
	bool                  isAdditive;
}
ParticleEmitterData;


struct AParticleEmitterData
{
	ParticleEmitterData* (*Create)         (char* filePath);
    void                 (*Init)           (char* filePath, ParticleEmitterData* outParticleEmitterData);
    void                 (*Release)        (ParticleEmitterData*  particleEmitterData);

    float                (*RandomLowValue) (ParticleRangedValue*  rangedValue);
    float                (*RandomHighValue)(ParticleScaledValue*  scaledValue);
    float                (*GetScale)       (ParticleScaledValue*  scaledValue, float percent);
    void                 (*GetRGB)         (ParticleRGBValue*     rgbValue,    float percent, float outRGB[3]);
};


extern struct AParticleEmitterData AParticleEmitterData[1];


#endif
