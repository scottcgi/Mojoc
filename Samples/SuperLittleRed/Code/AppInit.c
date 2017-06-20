/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-1-20
 * Author : scott.cgi
 * Version: 0.1
 */

#include <stdio.h>
#include <string.h>
#include <Engine/Graphics/OpenGL/Sprite.h>
#include <Engine/Toolkit/Utils/TweenTool.h>

#include "Engine/Audio/Platform/Audio.h"
#include "Engine/Graphics/Graphics.h"
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Application/Application.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Extension/Font.h"
#include "Engine/Toolkit/Utils/Coroutine.h"
#include "Engine/Graphics/OpenGL/Camera.h"
#include "Engine/Graphics/OpenGL/GLTool.h"

#include "GameMap.h"
#include "Hero.h"
#include "GameActor.h"
#include "EnemyAI.h"
#include "HUD.h"
#include "Tool.h"
#include "UI.h"
#include "GameData.h"
#include "AudioTool.h"


static Font*      font;
static FontText*  loadingText;
static Scheduler* loadingScheduler;
static Sprite*    logoSprite = NULL;


static void LoadingUpdate(Scheduler* scheduler, float deltaSeconds)
{
    AFont->Draw(font);

    if (scheduler->isCancel)
    {
        ASprite->Release(logoSprite);
        free(logoSprite);
        logoSprite = NULL;
    }
    else if (logoSprite != NULL)
    {
        ASprite_Draw(logoSprite);
    }
}


#define INIT_FUNCTION_COUNT 6


static void (*Inits[INIT_FUNCTION_COUNT])();
static int  progressSize = INIT_FUNCTION_COUNT + ENEMY_KINDS_COUNT * 5;


static void LoadingRun(Coroutine* coroutine)
{
    static int progress = 0;

//--------------------------------------------------------------------------------------------------

    ACoroutine_Begin();

    ATweenTool->AddFadeTo (1.0f, 2.5f)->SetRelative(false)->RunActions(logoSprite->drawable);
    ACoroutine_YieldSeconds(2.5f);

    ATweenTool->AddFadeTo (0.0f, 1.5f)->SetRelative(false)->RunActions(logoSprite->drawable);
    ACoroutine_YieldSeconds(1.5f);

    for (; progress < progressSize; progress++)
    {
        if (progress < INIT_FUNCTION_COUNT)
        {
            Inits[progress]();
        }
        else
        {
             AEnemyAI->CreateCache((progress - INIT_FUNCTION_COUNT) / 5);
        }

        char buff[5];
        sprintf(buff, "%d%%", (progress + 1) * 100 / progressSize);
        AFont->SetString(loadingText, buff);

        ADrawable_SetPositionX
        (
            loadingText->drawable,
            loadingText->drawable->width / 2
        );

        ACoroutine_YieldFrames(1);
    }

    ACoroutine_YieldSeconds(1.0f);

    ACoroutine_End();

//--------------------------------------------------------------------------------------------------

    loadingScheduler->isCancel = true;
    AFont->Reuse(font);
    AHUD->Run();
}


static void OnReady()
{
    glEnable   (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    AGraphics->SetUseVAO      (true);
    AGraphics->SetUseMapBuffer(false);

//--------------------------------------------------------------------------------------------------

    memcpy
    (
        Inits,
        (
            (void (*[])())
            {
                AGameMap->Init,
                AHero->Init,
                AGameActor->Init,
                AHUD->Init,
                AUI->Init,
                AEnemyAI->Init,
            }
        ),
        INIT_FUNCTION_COUNT * sizeof(void*)
    );

    font        = AFont->Get("Font/hp.atlas");
    loadingText = AFont->GetText(font);
    loadingText->alignment = FontTextAlignment_HorizontalRight;

    ATool->globalScaleX = AGLTool->screenWidth  / SCREEN_DESIGN_WIDTH;
    ATool->globalScaleY = AGLTool->screenHeight / SCREEN_DESIGN_HEIGHT;

    ADrawable_SetScale2
    (
        loadingText->drawable,
        ATool->globalScaleX,
        ATool->globalScaleY
    );

    logoSprite       = ASprite->CreateWithFile("Texture/Logo.png");
    ADrawable_SetOpacity(logoSprite->drawable, 0.0f);

    loadingScheduler = AScheduler->Schedule(LoadingUpdate, 0.0f);
    ACoroutine->StartCoroutine(LoadingRun);

    AMath_RandomSeedByTime();
    AAudioTool->StartUIBG();
}


static void OnResized(int width, int height)
{
    ACamera->right  =  AGLTool->screenRatio;
    ACamera->left   = -AGLTool->screenRatio;
    ACamera->bottom = -1.0f;
    ACamera->top    =  1.0f;
    ACamera->near   =  1.0f;
    ACamera->far    =  11.0f;
    ACamera->SetOrtho();

    ACamera->eyeX   =  0.0f;
    ACamera->eyeY   =  0.0f;
    ACamera->eyeZ   =  2.0f;

    ACamera->lookX  =  0.0f;
    ACamera->lookY  =  0.0f;
    ACamera->lookZ  = -1.0f;

    ACamera->upX    =  0.0f;
    ACamera->upY    =  1.0f;
    ACamera->upZ    =  0.0f;
    
    ACamera->SetLookAt();
}


static void OnPause()
{
    AAudio->SetLoopPause();
}


static void OnResume()
{
    AAudio->SetLoopResume();
}


static void OnDestroy()
{

}


static void OnSaveData(void** outSaveData, int* outLength)
{
    *outSaveData = AGameData;
    *outLength   = sizeof(AGameData);
}


static void OnInitWithSavedData(void* savedData, int length)
{
    memcpy(AGameData, savedData, length);
}


void Application_Main()
{
    AApplication->callbacks->OnReady             = OnReady;
    AApplication->callbacks->OnPause             = OnPause;
    AApplication->callbacks->OnResume            = OnResume;
    AApplication->callbacks->OnDestroy           = OnDestroy;
    AApplication->callbacks->OnResized           = OnResized;
    AApplication->callbacks->OnSaveData          = OnSaveData;
    AApplication->callbacks->OnInitWithSavedData = OnInitWithSavedData;
}