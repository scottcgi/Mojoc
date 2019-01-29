/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-9-4
 * Update: 2019-1-7
 * Author: scott.cgi
 */


#include "Engine/Audio/Platform/Audio.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Application/Application.h"
#include "Engine/Graphics/Graphics.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Toolkit/Utils/Tween.h"
#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Extension/Extension.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Toolkit/Utils/Coroutine.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/FileTool.h"


static const char* saveDataFileName = "MojocSaveDataFile";
static struct      timespec now;
static struct      timespec last;


#define CheckCallback(callback) \
    ALog_A((callback) != NULL, #callback " in Application_MainImpl must be set !")



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
    CheckCallback(AApplication->callbacks->OnInitWithSavedData);

    long  size;
    void* data = AFileTool->CreateDataFromRelative(saveDataFileName, &size);

    if (data != NULL)
    {
        AApplication->callbacks->OnInitWithSavedData(data, (size_t) size);
        free(data);
    }

    // start clock
    clock_gettime(CLOCK_MONOTONIC, &last);
}


#undef CheckCallback


static void Loop()
{
    clock_gettime(CLOCK_MONOTONIC, &now);
    float deltaSeconds = (float) ((now.tv_nsec - last.tv_nsec) * 0.000000001 + (now.tv_sec - last.tv_sec));
    last              =  now;

    AScheduler   ->Update(deltaSeconds);
    ACoroutine   ->Update(deltaSeconds);
    ATween       ->Update(deltaSeconds);
    APhysicsWorld->Update(deltaSeconds);
    AAudio       ->Update(deltaSeconds);

    // root update
    AComponent->Update(AApplication->rootComponent, deltaSeconds);

    // rendering
    ADrawable->RenderQueue();
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
    AAudio->Release();
    AApplication->callbacks->OnDestroy();
}


static void Touch(int fingerId, float pixelX, float pixelY, InputTouchType inputTouchType)
{
    AComponent->SendMessage
    (
        AApplication->rootComponent,
        AApplication,
        ComponentMsg_OnTouch,
        AArray_Make(InputTouch*, 1, AInput->SetTouch(fingerId, pixelX, pixelY, inputTouchType))
    );
}


static void Touches(int fingerIds[], float pixelXs[], float pixelYs[], int touchCount, InputTouchType inputTouchType)
{
    InputTouch* touches[touchCount];

    for (int i = 0; i < touchCount; ++i)
    {
        touches[i] = AInput->SetTouch(fingerIds[i], pixelXs[i], pixelYs[i], inputTouchType);
    }

    AComponent->SendMessage
    (
        AApplication->rootComponent,
        AApplication,
        ComponentMsg_OnTouch,
        (Array[1]) {touches, touchCount}
    );
}


static void SaveData()
{
    void*  outSaveData;
    size_t outSize;
    AApplication->callbacks->OnSaveData(&outSaveData, &outSize);
    AFileTool->WriteDataToRelative(saveDataFileName, outSaveData, outSize);
}


struct AApplication AApplication[1] =
{{
     .callbacks =
     {{
         .OnReady             = NULL,
         .OnPause             = NULL,
         .OnResume            = NULL,
         .OnDestroy           = NULL,
         .OnResized           = NULL,
         .OnSaveData          = NULL,
         .OnInitWithSavedData = NULL,
     }},

     .Init     = Init,
     .Loop     = Loop,
     .Ready    = Ready,
     .Resized  = Resized,
     .Pause    = Pause,
     .Resume   = Resume,
     .Destroy  = Destroy,
     .Touch    = Touch,
     .Touches  = Touches,
     .SaveData = SaveData,
}};

