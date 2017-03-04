/*
 *
 *
 *  Created on: 2013-1-21
 *  Author: scott.cgi
 */

#ifndef particle_h
#define particle_h


#include <stdbool.h>


#include "Mojoc/Graphics/OpenGL/SubMesh.h"


typedef struct
{
	bool  	 isActive;

	float 	 life;
	float    currentLife;

	float 	 scale;
	float    scaleDiff;

	float 	 rotationZ;
	float    rotationDiff;

	float    velocity;
	float    velocityDiff;

	float    angle;
	float    angleDiff;

	float    transparency;
	float    transparencyDiff;

	float 	 wind;
	float    windDiff;

	float    gravity;
	float    gravityDiff;
	SubMesh* subMesh;
}
Particle;


struct AParticle
{
	/**
	 * Create new Particle that allocate memory and need to free it manually
	 */
	Particle* (*Create)(SubMesh* subMesh);

	/**
	 * Initialize Particle that memory has already allocated
	 */
	void      (*Init)  (SubMesh* subMesh, Particle* outParticle);

	/**
	 * Reset Particle property values to the born
	 */
	void      (*Reset) (Particle* particle);
};

extern struct AParticle AParticle[1];

#endif
