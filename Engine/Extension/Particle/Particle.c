/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2013-1-21
 * Author: scott.cgi
 */

#include <stdlib.h>
#include "Engine/Extension/Particle/Particle.h"


static void Reset(Particle* particle)
{
	Drawable* drawable = particle->subMesh->drawable;

	ADrawable_SetScale2   (drawable, 1.0f, 1.0f);
	ADrawable_SetPosition2(drawable, 0.0f, 0.0f);
	ADrawable_SetRotationZ(drawable, 0.0f);
}


static void Init(SubMesh* subMesh, Particle* outParticle)
{
	outParticle->isActive = false;
	outParticle->subMesh  = subMesh;

	ADrawable_SetInVisible(subMesh->drawable);
}


struct AParticle AParticle[1] =
{
	Init,
	Reset,
};
