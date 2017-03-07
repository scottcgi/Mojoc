/*
 *
 *
 *  Created on: 2013-1-21
 *  Author: scott.cgi
 */

#include <malloc.h>

#include "Engine/Extension/Particle/Particle.h"
#include "Engine/Toolkit/Platform/Log.h"



static void Reset(Particle* particle)
{
	Drawable* drawable = particle->subMesh->drawable;

	ADrawableSetScale2   (drawable, 1.0f, 1.0f);
	ADrawableSetPosition2(drawable, 0.0f, 0.0f);
	ADrawableSetRotationZ(drawable, 0.0f);
}

static void Init(SubMesh* subMesh, Particle* outParticle)
{
	outParticle->isActive = false;
	outParticle->subMesh  = subMesh;

	ADrawableSetInVisible(subMesh->drawable);
}

static Particle* Create(SubMesh* subMesh)
{
	Particle* particle = (Particle*) malloc(sizeof(Particle));
	Init(subMesh, particle);

	return particle;
}


struct AParticle AParticle[1] =
{
	Create,
	Init,
	Reset,
};
