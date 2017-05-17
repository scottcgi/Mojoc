/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-1-27
 * Author : scott.cgi
 * Version: 0.1
 */


#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H


#include "Engine/Graphics/OpenGL/Mesh.h"
#include "Engine/Extension/Particle/ParticleEmitterData.h"
#include "Engine/Extension/Particle/Particle.h"
#include "Engine/Graphics/OpenGL/Texture.h"
#include "Engine/Toolkit/Utils/Array.h"


typedef struct
{
    /**
     * Emitter emission position x
     */
    float                emissionX;

    /**
     * Emitter emission position y
     */
    float                emissionY;

    /**
     * Emitter emission angle
     */
    float                emissionAngle;

	int                  emission;
	int                  emissionDiff;
	float                emissionDelta;

	float                lifeOffset;
	float                lifeOffsetDiff;

	float                life;
	float                lifeDiff;

	float                duration;
	float                durationTimer;

	float                delay;
	float                delayTimer;

	int                  activeCount;
	Array(Particle)*     particleArr;
	ParticleEmitterData* emitterData;
    Mesh                 mesh[1];
}
ParticleEmitter;


struct AParticleEmitter
{
	/**
	 * Create new ParticleEmitter that allocate memory and need to free it manually
	 */
	ParticleEmitter* (*Create)   (ParticleEmitterData* emitterData, Texture* texture);

	/**
	 * Initialize ParticleEmitter that memory has already allocated
	 */
	void             (*Init)     (ParticleEmitterData* emitterData, Texture* texture, ParticleEmitter* outParticleEmitter);

	/**
	 * Free members memory that allocate by malloc and reset some property values
	 */
	void             (*Release)  (ParticleEmitter*     emitter);

	/**
	 * Called per frame
	 */
	void             (*Update)   (ParticleEmitter*     emitter,     float deltaSeconds);

	/**
	 * Restart ParticleEmitter to the initial state
	 */
	void             (*Restart)  (ParticleEmitter*     emitter);
};


extern struct AParticleEmitter AParticleEmitter[1];


static inline void AParticleEmitter_Draw(ParticleEmitter* emitter)
{
	AMesh_Draw(emitter->mesh);
}


#endif
