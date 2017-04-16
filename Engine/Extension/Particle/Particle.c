/*
 *
 *
 *  Created on: 2013-1-21
 *  Author: scott.cgi
 */

#include <stdlib.h>
#include "Engine/Extension/Particle/Particle.h"


static void Reset(Particle* particle)
{
	Drawable* drawable = particle->subMesh->drawable;

	ADrawableSetScale2   (drawable, 1.0f, 1.0f);
	ADrawableSetPosition2(drawable, 0.0f, 0.0f);
	ADrawableSetRotationZ(drawable, 0.0f);
}


static void Init(SubMesh* subMesh, Particle* out_param particle)
{
	particle->isActive = false;
	particle->subMesh  = subMesh;

	ADrawableSetInVisible(subMesh->drawable);
}


struct AParticle AParticle[1] =
{
	Init,
	Reset,
};
