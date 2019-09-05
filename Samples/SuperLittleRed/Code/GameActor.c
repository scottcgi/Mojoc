/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2014-6-23
 * Update   : 2019-2-16
 * Author   : scott.cgi
 */


#include "Engine/Application/Application.h"
#include "Engine/Application/Scheduler.h"

#include "GameActor.h"
#include "Hero.h"
#include "GameMap.h"


static FontText* fpsText;
static FontText* drawCallsText;


static void UpdateAfter(Component* component, float deltaSeconds)
{
    AFont->Draw(AGameActor->hpFont);
    AFont->Draw(AGameActor->talkFont);
}


static void UpdateFPS(Scheduler* scheduler, float deltaSeconds)
{
    AFont->SetInt(fpsText, (int) (1.0f / deltaSeconds));
    AFont->SetInt(drawCallsText, ADrawable->GetDrawCalls());
}


static void Init()
{
    AComponent->Init(AGameActor->component);
    AGameActor->component->curState->UpdateAfter = UpdateAfter;

    AGameActor->talkFont = AFont->Get("Font/TalkChar.atlas");
    AGameActor->hpFont   = AFont->Get("Font/hp.atlas");

    #ifdef APP_SHOW_FPS_AND_DRAW_CALL
    fpsText = AFont->GetText(AGameActor->hpFont);
    ADrawable_SetPosition2(fpsText->drawable,  -0.05f, 0.9f);
    AFont->SetInt(fpsText, 1000000);

    drawCallsText = AFont->GetText(AGameActor->hpFont);
    ADrawable_SetPosition2(drawCallsText->drawable, 0.05f, 0.9f);
    AFont->SetInt(drawCallsText, 1000000);

    AScheduler->Schedule(UpdateFPS, 0.5f);
    #endif

}


static void Run()
{
    AApplication_AddChild(AGameActor->component, 40);
}


struct AGameActor AGameActor[1] =
{{
    .Init = Init,
    .Run  = Run,
}};
