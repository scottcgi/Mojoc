/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-1-21
 * Author : scott.cgi
 * Version: 0.0.0
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
