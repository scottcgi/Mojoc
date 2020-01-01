/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2013-9-4
 * Update   : 2019-1-7
 * Author   : scott.cgi
 */


#include "Engine/Graphics/Graphics.h"
#include "Engine/Audio/Platform/Audio.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Application/Application.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Toolkit/Utils/Tween.h"
#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Extension/Extension.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Toolkit/Utils/Coroutine.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/Thread.h"


static struct timespec now;
static struct timespec last;


#define CheckCallback(callback) \
    ALog_A((callback) != NULL, #callback " in Application_MainImpl cannot NULL")


static void Init()
{
    AToolkit  ->Init();
    APhysics  ->Init();
    AExtension->Init();
    AAudio    ->Init();
    AComponent->Init(AApplication->rootComponent);

    // entry called
    Application_MainImpl();

    CheckCallback(AApplication->callbacks->OnReady);
    CheckCallback(AApplication->callbacks->OnPause);
    CheckCallback(AApplication->callbacks->OnResume);
    CheckCallback(AApplication->callbacks->OnDestroy);
    CheckCallback(AApplication->callbacks->OnResized);
    CheckCallback(AApplication->callbacks->OnSaveData);

    // start clock
    clock_gettime(CLOCK_MONOTONIC, &last);
}


#undef CheckCallback


static void Loop()
{
    clock_gettime(CLOCK_MONOTONIC, &now);
    float deltaSeconds = (float) ((now.tv_nsec - last.tv_nsec) * 0.000000001 + (now.tv_sec - last.tv_sec));
    last               =  now;

    APhysicsWorld->Update(deltaSeconds);
    ATween       ->Update(deltaSeconds);
    AScheduler   ->Update(deltaSeconds);
    ACoroutine   ->Update(deltaSeconds);
    AAudio       ->Update(deltaSeconds);

    // root update
    AComponent->Update(AApplication->rootComponent, deltaSeconds);

    // rendering
    ADrawable->Render();
}


static void Resized(int width, int height)
{
    AGLTool->SetSize(width, height);
    AApplication->callbacks->OnResized(width, height);
}


static void Ready(int width, int height)
{
    Resized(width, height);
    AGraphics->Init();
    AApplication->callbacks->OnReady();
}


static void Pause()
{
    AApplication->callbacks->OnPause();
}


static void Resume()
{
    AApplication->callbacks->OnResume();
    // restart clock
    clock_gettime(CLOCK_MONOTONIC, &last);
}


static void Destroy()
{
    AApplication->callbacks->OnDestroy();
}


static void Touch(int fingerID, float pixelX, float pixelY, InputTouchType inputTouchType)
{
    AComponent->SendMessage
    (
        AApplication->rootComponent,
        AApplication,
        ComponentMsg_OnTouch,
        AArray_Make(InputTouch*, 1, AInput->SetTouch(fingerID, pixelX, pixelY, inputTouchType))
    );
}


static void Touches(int fingerIDs[], float pixelXs[], float pixelYs[], int touchesCount, InputTouchType inputTouchType)
{
    InputTouch* touches[touchesCount];

    for (int i = 0; i < touchesCount; ++i)
    {
        touches[i] = AInput->SetTouch(fingerIDs[i], pixelXs[i], pixelYs[i], inputTouchType);
    }

    AComponent->SendMessage
    (
        AApplication->rootComponent,
        AApplication,
        ComponentMsg_OnTouch,
        (Array(InputTouch*)[1]) {touches, touchesCount}
    );
}


void* SaveDataRun(void* param)
{
    AApplication->callbacks->OnSaveData(param);
    return NULL;
}

static void SaveData(void* param)
{
    AThread->StartThread(SaveDataRun, param);
}


struct AApplication AApplication[1] =
{{
     .callbacks =
     {{
         .OnReady    = NULL,
         .OnPause    = NULL,
         .OnResume   = NULL,
         .OnDestroy  = NULL,
         .OnResized  = NULL,
         .OnSaveData = NULL,
     }},

     .Init           = Init,
     .Loop           = Loop,
     .Ready          = Ready,
     .Resized        = Resized,
     .Pause          = Pause,
     .Resume         = Resume,
     .Destroy        = Destroy,
     .Touch          = Touch,
     .Touches        = Touches,
     .SaveData       = SaveData,
}};

