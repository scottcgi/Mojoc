/*
 *
 *
 *  Created on: 2013-1-27
 *  Author: scott.cgi
 */

#ifndef particle_emitter_h
#define particle_emitter_h


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

    Mesh                 get_only mesh[1];

	int                  get_only emission;
	int                  get_only emissionDiff;
	float                get_only emissionDelta;

	float                get_only lifeOffset;
	float                get_only lifeOffsetDiff;

	float                get_only life;
	float                get_only lifeDiff;

	float                get_only duration;
	float                get_only durationTimer;

	float                get_only delay;
	float                get_only delayTimer;

	int                  get_only activeCount;
	Array(Particle)*     get_only particleArr;
	ParticleEmitterData* get_only emitterData;
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
	void             (*Init)     (ParticleEmitterData* emitterData, Texture* texture, ParticleEmitter* out_param particleEmitter);

	/**
	 * Free members memory that allocate by malloc and reset some property values
	 */
	void             (*Release)  (ParticleEmitter*     emitter);

	/**
	 * Called per frame
	 */
	void             (*Update)   (ParticleEmitter*     emitter,     float    deltaTime);

	/**
	 * Restart ParticleEmitter to the initial state
	 */
	void             (*Restart)  (ParticleEmitter*     emitter);
};


extern struct AParticleEmitter AParticleEmitter[1];


static inline Drawable* AParticleEmitterGetDrawable(ParticleEmitter* emitter)
{
	return emitter->mesh->drawable;
}


static inline void AParticleEmitterDraw(ParticleEmitter* emitter)
{
	AMeshDraw(emitter->mesh);
}


#endif
