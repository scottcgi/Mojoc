/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-9-4
 * Author : scott.cgi
 * Version: 0.0.0
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


static void Init()
{
    AToolkit  ->Init();
    APhysics  ->Init();
    AExtension->Init();
    AAudio    ->Init();

    AComponent->Init(AApplication->component);

    // entry called
    Application_Main();

    // check callback setting
    ALog_A(AApplication->callbacks->OnReady             != NULL, "AApplication->callbacks->OnReady             must be set");
    ALog_A(AApplication->callbacks->OnPause             != NULL, "AApplication->callbacks->OnPause             must be set");
    ALog_A(AApplication->callbacks->OnResume            != NULL, "AApplication->callbacks->OnResume            must be set");
    ALog_A(AApplication->callbacks->OnDestroy           != NULL, "AApplication->callbacks->OnDestroy           must be set");
    ALog_A(AApplication->callbacks->OnResized           != NULL, "AApplication->callbacks->OnResized           must be set");
    ALog_A(AApplication->callbacks->OnSaveData          != NULL, "AApplication->callbacks->OnSaveData          must be set");
    ALog_A(AApplication->callbacks->OnInitWithSavedData != NULL, "AApplication->callbacks->OnInitWithSavedData must be set");

//----------------------------------------------------------------------------------------------------------------------

    int   length;
    void* data = AFileTool->CreateDataFromDir((char*) saveDataFileName, &length);

    if (data != NULL)
    {
        AApplication->callbacks->OnInitWithSavedData(data, length);
        free(data);
    }

//----------------------------------------------------------------------------------------------------------------------

    // start clock
    clock_gettime(CLOCK_MONOTONIC, &last);
}


static void Loop()
{
    clock_gettime(CLOCK_MONOTONIC, &now);
    float deltaSeconds = (now.tv_nsec - last.tv_nsec) * 0.000000001 + (now.tv_sec - last.tv_sec);
    last               =  now;

//----------------------------------------------------------------------------------------------------------------------

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    AScheduler   ->Update(deltaSeconds);
    ACoroutine   ->Update(deltaSeconds);
    ATween       ->Update(deltaSeconds);
    APhysicsWorld->Update(deltaSeconds);
    AAudio       ->Update(deltaSeconds);

    // root update
    AComponent->Update(AApplication->component, deltaSeconds);

    // rendering
    ADrawable->RenderQueue();
}


static void Resized(int width, int height)
{
    // set the OpenGL viewport to the same size as the surface.
    glViewport(0, 0, width, height);

    AGLTool     ->SetSize             (width, height);
    AApplication->callbacks->OnResized(width, height);
}


static void GLReady(int width, int height)
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


static void Touch(Array(InputTouch*)* touchData)
{
    AComponent->SendMessage
    (
        AApplication->component,
        AApplication,
        ComponentMsg_OnTouch,
        touchData
    );
}


static void SaveData()
{
    void* outSaveData;
    int   outSize;
    AApplication->callbacks->OnSaveData(&outSaveData, &outSize);

    AFileTool->WriteDataToDir((char*) saveDataFileName, outSaveData, outSize);
}


struct AApplication AApplication[1] =
{
    {
        .callbacks =
        {
            {
               .OnReady             = NULL,
               .OnPause             = NULL,
               .OnResume            = NULL,
               .OnDestroy           = NULL,
               .OnResized           = NULL,
               .OnSaveData          = NULL,
               .OnInitWithSavedData = NULL,
            }
        },

        .Init     = Init,
        .Loop     = Loop,
        .GLReady  = GLReady,
        .Resized  = Resized,
        .Pause    = Pause,
        .Resume   = Resume,
        .Destroy  = Destroy,
        .Touch    = Touch,
        .SaveData = SaveData,
    }
};

