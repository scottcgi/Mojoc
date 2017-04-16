/*
 *
 *
 *  Created on: 2013-1-22
 *  Author: scott.cgi
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
	ParticleRangedValue get_only delayValue       [1];
	ParticleScaledValue get_only lifeOffsetValue  [1];
	ParticleRangedValue get_only durationValue    [1];
	ParticleScaledValue get_only lifeValue        [1];
	ParticleScaledValue get_only emissionValue    [1];
	ParticleScaledValue get_only scaleValue       [1];
	ParticleScaledValue get_only rotationValue    [1];
	ParticleScaledValue get_only velocityValue    [1];
	ParticleScaledValue get_only angleValue       [1];
	ParticleScaledValue get_only windValue        [1];
	ParticleScaledValue get_only gravityValue     [1];
	ParticleScaledValue get_only transparencyValue[1];
	ParticleScaledValue get_only xOffsetValue     [1];
	ParticleScaledValue get_only yOffsetValue     [1];
	ParticleScaledValue get_only spawnWidthValue  [1];
	ParticleScaledValue get_only spawnHeightValue [1];
	ParticleRGBValue    get_only rgbValue         [1];

	int                 get_only  minParticleCount;
	int                 get_only  maxParticleCount;

	/**
	 * The emitter restart when timeover
	 */
	bool                get_only  isContinuous;

	/**
	 * The particle angle is added to the rotation
	 */
	bool                 get_only isAligned;

	/**
	 * The color blend additive
	 */
	bool                 get_only isAdditive;
}
ParticleEmitterData;


struct AParticleEmitterData
{
	ParticleEmitterData* (*Create)         (const char* filePath);
    void                 (*Init)           (const char* filePath, ParticleEmitterData* out_param particleEmitterData);
    void                 (*Release)        (ParticleEmitterData*  particleEmitterData);

    float                (*RandomLowValue) (ParticleRangedValue*  rangedValue);
    float                (*RandomHighValue)(ParticleScaledValue*  scaledValue);
    float                (*GetScale)       (ParticleScaledValue*  scaledValue, float percent);
    void                 (*GetRGB)         (ParticleRGBValue*     rgbValue,    float percent, float out_param rgb[3]);
};


extern struct AParticleEmitterData AParticleEmitterData[1];


#endif
