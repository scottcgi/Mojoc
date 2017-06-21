/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-1-27
 * Author : scott.cgi
 * Version: 0.1
 */


#include <math.h>
#include "Engine/Extension/Particle/ParticleEmitter.h"
#include "Engine/Toolkit/Head/Struct.h"
#include "Engine/Graphics/OpenGL/GLTool.h"


static void Restart(ParticleEmitter* emitter)
{
    emitter->delayTimer                = 0.0f;
    emitter->durationTimer             = 0.0f;
    emitter->emissionDelta             = 0.0f;

    ParticleEmitterData* emitterData   = emitter->emitterData;

    ParticleRangedValue* delayValue    = emitterData->delayValue;
    emitter->delay                     = delayValue->isActive ?
                                               AParticleEmitterData->RandomLowValue(delayValue) : 0.0f;

    ParticleRangedValue* durationValue = emitterData->durationValue;
    emitter->duration                  = AParticleEmitterData->RandomLowValue(durationValue);


    ParticleScaledValue* emissionValue = emitterData->emissionValue;
    emitter->emission                  = (int) AParticleEmitterData->RandomLowValue(emissionValue->rangedValue);
    emitter->emissionDiff              = (int) AParticleEmitterData->RandomHighValue(emissionValue);

    if (!emissionValue->rangedValue->isActive)
    {
        emitter->emissionDiff -= emitter->emission;
    }


    ParticleScaledValue* lifeValue     = emitterData->lifeValue;
    emitter->life                      = AParticleEmitterData->RandomLowValue(lifeValue->rangedValue);
    emitter->lifeDiff                  = AParticleEmitterData->RandomHighValue(lifeValue);

    if (!lifeValue->rangedValue->isActive)
    {
        emitter->lifeDiff -= emitter->life;
    }

    ParticleScaledValue* lifeOffsetValue = emitterData->lifeOffsetValue;

    if (lifeOffsetValue->rangedValue->isActive)
    {
        emitter->lifeOffset          = AParticleEmitterData->RandomLowValue(lifeOffsetValue->rangedValue);
        emitter->lifeOffsetDiff      = AParticleEmitterData->RandomHighValue(lifeOffsetValue);
        if (!lifeOffsetValue->isRelative)
        {
            emitter->lifeOffsetDiff -= emitter->lifeOffset;
        }
    }
}


static inline void ActivateParticle(ParticleEmitter* emitter, Particle* particle)
{
    particle->isActive               = true;
    Drawable*            drawable    = particle->subMesh->drawable;
    ParticleEmitterData* emitterData = emitter->emitterData;

    ParticleScaledValue* lifeValue   = emitterData->lifeValue;
    float percent                    = emitter->delayTimer / emitter->duration;
    float activeLife                 = emitter->life + emitter->lifeDiff *
                                       AParticleEmitterData->GetScale(lifeValue, percent);

    particle->life = particle->currentLife = activeLife;

    ParticleScaledValue* lifeOffsetValue = emitterData->lifeOffsetValue;
    if (lifeOffsetValue->rangedValue->isActive)
    {
        float offsetTime = emitter->lifeOffset + emitter->lifeOffsetDiff *
                           AParticleEmitterData->GetScale(lifeOffsetValue, percent);

        particle->life = particle->currentLife = particle->life - offsetTime;
    }


    ParticleScaledValue* velocityValue = emitterData->velocityValue;
    if (velocityValue->rangedValue->isActive)
    {
         particle->velocity             = AParticleEmitterData->RandomLowValue(velocityValue->rangedValue);
        particle->velocityDiff         = AParticleEmitterData->RandomHighValue(velocityValue);

        if (!velocityValue->isRelative)
        {
            particle->velocityDiff    -= particle->velocity;
        }
    }

    ParticleScaledValue* angleValue = emitterData->angleValue;
    if (angleValue->rangedValue->isActive)
    {
        particle->angle             = AParticleEmitterData->RandomLowValue(angleValue->rangedValue);
        particle->angleDiff         = AParticleEmitterData->RandomHighValue(angleValue);

        if (!angleValue->isRelative)
        {
            particle->angleDiff    -= particle->angle;
        }
    }


    float width                     = AGLTool_ToScreenWidth(drawable->width);
    ParticleScaledValue* scaleValue = emitterData->scaleValue;
    particle->scale                 = AParticleEmitterData->RandomLowValue(scaleValue->rangedValue) / width;
    particle->scaleDiff             = AParticleEmitterData->RandomHighValue(scaleValue) / width;

    if (!scaleValue->isRelative)
    {
        particle->scaleDiff -= particle->scale;
    }

    float scale = particle->scale + particle->scaleDiff * AParticleEmitterData->GetScale(scaleValue, 0.0f);
    ADrawable_SetScale2(drawable, scale, scale);

    ParticleScaledValue* rotationValue = emitterData->rotationValue;
    if (rotationValue->rangedValue->isActive)
    {
        particle->rotationZ            = AParticleEmitterData->RandomLowValue(rotationValue->rangedValue);
        particle->rotationDiff         = AParticleEmitterData->RandomHighValue(rotationValue);

        if (!rotationValue->isRelative)
        {
            particle->rotationDiff -= particle->rotationZ;
        }

        float rotationZ = particle->rotationZ + particle->rotationDiff * AParticleEmitterData->GetScale(rotationValue, 0.0f);
        if (emitter->emitterData->isAligned)
        {
            rotationZ += particle->angle + particle->angleDiff * AParticleEmitterData->GetScale(angleValue, 0.0f);
        }

        ADrawable_SetRotationZ(drawable, rotationZ);
    }


    ParticleScaledValue* windValue = emitterData->windValue;
    if (windValue->rangedValue->isActive)
    {
        particle->wind     = AParticleEmitterData->RandomLowValue(windValue->rangedValue);
        particle->windDiff = AParticleEmitterData->RandomHighValue(windValue);

        if (!windValue->isRelative)
        {
            particle->windDiff -= particle->wind;
        }
    }

    ParticleScaledValue* gravityValue = emitterData->gravityValue;
    if (gravityValue->rangedValue->isActive)
    {
        particle->gravity             = AParticleEmitterData->RandomLowValue(gravityValue->rangedValue);
        particle->gravityDiff         = AParticleEmitterData->RandomHighValue(gravityValue);

        if (!gravityValue->isRelative)
        {
            particle->gravityDiff    -= particle->gravity;
        }
    }

    float rgb[3];
    AParticleEmitterData->GetRGB(emitterData->rgbValue, percent, rgb);
    ADrawable_SetRGB(drawable, rgb[0], rgb[1], rgb[2]);

    ParticleScaledValue* transparencyValue = emitterData->transparencyValue;
    particle->transparency                 = AParticleEmitterData->RandomLowValue(transparencyValue->rangedValue);
    particle->transparencyDiff             = AParticleEmitterData->RandomHighValue(transparencyValue) - particle->transparency;

    ADrawable_SetOpacity  (drawable, particle->transparency);
    ADrawable_SetPosition2(drawable, emitter->emissionX, emitter->emissionY);
    ADrawable_SetVisible  (drawable);
}


static inline void AddParticles(ParticleEmitter* emitter, int count)
{
    emitter->activeCount += count;

    for (int i = 0; i < emitter->particleArr->length; i++)
    {
        Particle* particle = AArray_GetPtr(emitter->particleArr, i, Particle);
        if (particle->isActive == false)
        {
            ActivateParticle(emitter, particle);
            count--;

            if (count == 0)
            {
                break;
            }
        }
    }
}


static inline void UpdateParticle(ParticleEmitter* emitter, Particle* particle, float deltaSeconds)
{
    particle->currentLife -= deltaSeconds;
    Drawable* drawable     = particle->subMesh->drawable;

    if (particle->currentLife < 0)
    {
        particle->isActive = false;
        AParticle->Reset(particle);
        emitter->activeCount--;
        return;
    }

    ParticleEmitterData* emitterData = emitter->emitterData;
    float                percent     = 1.0f - particle->currentLife / particle->life;


    ParticleScaledValue* scaleValue  = emitterData->scaleValue;
    if (scaleValue->rangedValue->isActive)
    {
        float scale = particle->scale + particle->scaleDiff * AParticleEmitterData->GetScale(scaleValue, percent);
        ADrawable_SetScale2(drawable, scale, scale);
    }

    float angle = 0.0f;

    ParticleScaledValue* velocityValue = emitterData->velocityValue;
    if (velocityValue->rangedValue->isActive)
    {
        // velocity is in one second
        float velocity  = (particle->velocity + particle->velocityDiff * AParticleEmitterData->GetScale(velocityValue, percent)) * deltaSeconds;
        float velocityX = velocity;
        float velocityY = velocity;

        ParticleScaledValue* angleValue = emitterData->angleValue;
        if (angleValue->rangedValue->isActive)
        {
            angle = particle->angle + particle->angleDiff * AParticleEmitterData->GetScale(angleValue, percent) + emitter->emissionAngle;

            velocityX *= AMath_Cos(angle);
            velocityY *= AMath_Sin(angle);
        }

        ParticleScaledValue* windValue = emitterData->windValue;
        if (windValue->rangedValue->isActive)
        {
            velocityX += (particle->wind + particle->windDiff * AParticleEmitterData->GetScale(windValue, percent)) * deltaSeconds;
        }

        ParticleScaledValue* gravityValue = emitterData->gravityValue;
        if (gravityValue->rangedValue->isActive)
        {
            velocityY += (particle->gravity + particle->gravityDiff * AParticleEmitterData->GetScale(gravityValue, percent)) * deltaSeconds;
        }

        ADrawable_SetPosition2
        (
            drawable,
            drawable->positionX + AGLTool_ToGLWidth (velocityX),
            drawable->positionY + AGLTool_ToGLHeight(velocityY)
        );
    }

    ParticleScaledValue* rotationValue = emitterData->rotationValue;
    if (rotationValue->rangedValue->isActive)
    {
        float rotationZ = particle->rotationZ + particle->rotationDiff * AParticleEmitterData->GetScale(rotationValue, percent);
        if (emitter->emitterData->isAligned)
        {
            rotationZ += angle;
        }

        ADrawable_SetRotationZ(drawable, rotationZ);
    }

    float rgb[3];
    AParticleEmitterData->GetRGB(emitterData->rgbValue, percent, rgb);

    ADrawable_SetRGBA
    (
        drawable,
        rgb[0],
        rgb[1],
        rgb[2],
        particle->transparency + particle->transparencyDiff * AParticleEmitterData->GetScale(emitterData->transparencyValue, percent)
    );
}


static void Render(Drawable* drawable)
{
    Mesh*            mesh    = AStruct_GetParent2(drawable, Mesh);
    ParticleEmitter* emitter = AStruct_GetParent2(mesh,     ParticleEmitter);

    if (emitter->emitterData->isAdditive)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
    
    AMesh->Render(drawable);

    if (emitter->emitterData->isAdditive)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}


static void Update(ParticleEmitter* emitter, float deltaSeconds)
{
    for (int i = 0; i < emitter->particleArr->length; i++)
    {
        Particle* particle = AArray_GetPtr(emitter->particleArr, i, Particle);
        if (particle->isActive)
        {
            UpdateParticle(emitter, particle, deltaSeconds);
        }
    }

    if (emitter->delayTimer < emitter->delay)
    {
        emitter->delayTimer += deltaSeconds;
        return;
    }

    if (emitter->durationTimer < emitter->duration)
    {
        emitter->durationTimer += deltaSeconds;
    }
    else
    {
        if (emitter->emitterData->isContinuous)
        {
            AParticleEmitter->Restart(emitter);
        }

        return;
    }

    ParticleEmitterData* emitterData = emitter->emitterData;

    // particle number per second
    float emissionNum = emitter->emission + emitter->emissionDiff *
                        AParticleEmitterData->GetScale
                        (
                            emitterData->emissionValue,
                            emitter->delayTimer / emitter->duration
                        );

    if (emissionNum > 0)
    {
        // one particle need time duration one second
        float emissionTime = 1.0f / emissionNum;

        if (emitter->emissionDelta < emissionTime)
        {
            emitter->emissionDelta += deltaSeconds;
        }
        else
        {
            int a                  = (int) (emitter->emissionDelta / emissionTime);
            int b                  = emitterData->maxParticleCount - emitter->activeCount;
            int count              = AMath_Min(a, b);
            emitter->emissionDelta = fmodf(emitter->emissionDelta, emissionTime);

            if (count > 0)
            {
                AddParticles(emitter, count);
            }
        }
    }
    else
    {
        emitter->emissionDelta += deltaSeconds;
    }

    if (emitter->activeCount < emitter->emitterData->minParticleCount)
    {
        AddParticles(emitter, emitterData->minParticleCount - emitter->activeCount);
    }

    AMesh_Draw(emitter->mesh);
}


static void Release(ParticleEmitter* emitter)
{
    AMesh->Release(emitter->mesh);

    free(emitter->particleArr);
    emitter->particleArr = NULL;
}


static void Init(ParticleEmitterData* emitterData, Texture* texture, ParticleEmitter* outEmitter)
{
    outEmitter->emitterData   = emitterData;
    outEmitter->activeCount   = 0;

    outEmitter->emissionX     = 0.0f;
    outEmitter->emissionY     = 0.0f;
    outEmitter->emissionAngle = 0.0f;

    Mesh* mesh                = outEmitter->mesh;

    AMesh->Init(texture, mesh);
    mesh->drawable->Render    = Render;

    int max                   = emitterData->maxParticleCount;
    ArrayList*  children      = mesh->childList;

    AArrayList->SetCapacity(children, max);
    outEmitter->particleArr   = AArray->Create(sizeof(Particle), max);

    for (int i = 0; i < max; i++)
    {
        Quad quad[1];
        AQuad->Init(texture->width, texture->height, quad);

        AParticle->Init
        (
             AMesh->AddChildWithQuad(mesh, quad),
             AArray_GetPtr(outEmitter->particleArr, i, Particle)
        );
    }

    AMesh->GenerateBuffer(mesh);

    Restart(outEmitter);
}


static ParticleEmitter* Create(ParticleEmitterData* emitterData, Texture* texture)
{
    ParticleEmitter* emitter = (ParticleEmitter*) malloc(sizeof(ParticleEmitter));
    Init(emitterData, texture, emitter);

    return emitter;
}


struct AParticleEmitter AParticleEmitter[1] =
{
    Create,
    Init,
    Release,
    Update,
    Restart,
};
