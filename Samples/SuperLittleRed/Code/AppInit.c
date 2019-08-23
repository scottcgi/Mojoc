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
 * Since    : 2013-1-20
 * Update   : 2019-2-16
 * Author   : scott.cgi
 */


#include <stdio.h>
#include <string.h>

#include "Engine/Toolkit/Utils/TweenTool.h"
#include "Engine/Audio/Platform/Audio.h"
#include "Engine/Application/Application.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Application/Platform/SystemInfo.h"
#include "Engine/Extension/Font.h"
#include "Engine/Toolkit/Utils/Coroutine.h"
#include "Engine/Graphics/OpenGL/Camera.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Graphics/Graphics.h"
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Utils/FileTool.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/Sprite.h"

#include "GameMap.h"
#include "Hero.h"
#include "GameActor.h"
#include "EnemyAI.h"
#include "HUD.h"
#include "Tool.h"
#include "UI.h"
#include "GameData.h"
#include "AudioTool.h"
#include "ADTool.h"
#include "Config.h"


static const char* saveDataFileName = "MojocSupperLittleRed";
static int         progressSize     = AppInit_FunctionsCount + Enemy_KindsNum * Enemy_EachKindInitNum;
static GLbitfield  clearBits        = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
static Font*       font;
static FontText*   loadingText;
static Scheduler*  loadingScheduler;
static Sprite*     logoSprite;


typedef void (*InitFunction)(void);
static InitFunction InitFunctions[AppInit_FunctionsCount];


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


static void LoadingRun(Coroutine* coroutine)
{
    static int progress = 0;

    ACoroutine_Begin();

    ATweenTool->AddFadeTo(1.0f, 2.5f)->SetEaseType(TweenEaseType_BounceOut)->RunActions(logoSprite->drawable);
    ACoroutine_YieldSeconds(3.5f);

    ATweenTool->AddFadeTo(-1.0f, 0.8f)->RunActions(logoSprite->drawable);
    ACoroutine_YieldSeconds(0.8f);

    for (; progress < progressSize; ++progress)
    {
        if (progress < AppInit_FunctionsCount)
        {
            InitFunctions[progress]();
        }
        else
        {
             AEnemyAI->CreateCache((progress - AppInit_FunctionsCount) / Enemy_EachKindInitNum);
        }

        char buff[5];
        sprintf(buff, "%d%%", (progress + 1) * 100 / progressSize);
        AFont->SetString(loadingText, buff);

        ADrawable_SetPositionX
        (
            loadingText->drawable,
            loadingText->drawable->width / 2
        );

        ACoroutine_YieldFrames(0);
    }

    ACoroutine_YieldSeconds(1.0f);
    ACoroutine_End();

    loadingScheduler->isCancel = true;
    AFont->Release(font);
    AHUD->Run();
}


static void InitLanguageCode()
{
    char outLanguageCode[2];
    ASystemInfo->GetLanguageCode(outLanguageCode);

    if (outLanguageCode[0] == 'z' && outLanguageCode[1] == 'h')
    {
        ATool->languageCode = LanguageCode_zh;
    }
}


static void InitWithSavedData()
{
    long  size;
    void* data = AFileTool->CreateDataFromRelative(saveDataFileName, &size);

    if (data != NULL)
    {
        memcpy(AGameData, data, (size_t) size);
        free(data);
    }

    ++AGameData->playGameCount;
}                                                              


static void OnReady()
{
    glEnable   (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    AGraphics->SetUseVAO      (true);
    AGraphics->SetUseMapBuffer(true);

    memcpy
    (
        InitFunctions,
        (InitFunction[AppInit_FunctionsCount])
        {
            InitLanguageCode,
            InitWithSavedData,
            AGameMap->Init,
            AHero->Init,
            AGameActor->Init,
            AHUD->Init,
            AUI->Init,
            AEnemyAI->Init,
        },
        AppInit_FunctionsCount * sizeof(void*)
    );

    ATool->globalScaleX = AGLTool->screenWidth  / Screen_DesignWidth;
    ATool->globalScaleY = AGLTool->screenHeight / Screen_DesignHeight;

    font        = AFont->Get("Font/hp.atlas");
    loadingText = AFont->GetText(font);
    loadingText->alignment = FontTextAlignment_HorizontalRight;
    ADrawable_SetScale2(loadingText->drawable, ATool->globalScaleX, ATool->globalScaleY);

    logoSprite = ASprite->CreateWithFile("Texture/Logo.png");
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


static void OnSaveData(void* param)
{
    AFileTool->WriteDataToRelative(saveDataFileName, AGameData, sizeof(struct AGameData));
}


static void Update(Component* component, float deltaSeconds)
{
    glClear(clearBits);
}



void Application_MainImpl()
{
    AApplication->rootComponent->defaultState->Update = Update;

    AApplication->callbacks->OnReady    = OnReady;
    AApplication->callbacks->OnPause    = OnPause;
    AApplication->callbacks->OnResume   = OnResume;
    AApplication->callbacks->OnDestroy  = OnDestroy;
    AApplication->callbacks->OnResized  = OnResized;
    AApplication->callbacks->OnSaveData = OnSaveData;
}

