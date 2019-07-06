/*
 * Copyright (c) 2017-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2014-6-23
 * Author : scott.cgi
 */

#include "Engine/Application/Application.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Extension/Particle/ParticleEmitter.h"

#include "GameActor.h"
#include "Hero.h"
#include "GameMap.h"


static FontText* fpsText;


static void UpdateAfter(Component* component, float deltaSeconds)
{
    AFont->Draw(AGameActor->hpFont);
    AFont->Draw(AGameActor->talkFont);
}


static void UpdateFPS(Scheduler* scheduler, float deltaSeconds)
{
    AFont->SetInt(fpsText, (int) (1.0f / deltaSeconds));
}


static void Init()
{
    AComponent->Init(AGameActor->component);
    AGameActor->component->curState->UpdateAfter = UpdateAfter;

    AGameActor->talkFont = AFont->Get("Font/TalkChar.atlas");
    AGameActor->hpFont   = AFont->Get("Font/hp.atlas");

//  fpsText              = AFont->GetText(AGameActor->hpFont);
//  ADrawable_SetPositionY(fpsText->drawable, -0.9f);
//  AScheduler->Schedule(UpdateFPS, 1.0f, NULL);
}


static void Run()
{
    AApplication_AddChild(AGameActor->component, 40);
}


struct AGameActor AGameActor[1] =
{
    {
        .Init = Init,
        .Run  = Run,
    }
};
