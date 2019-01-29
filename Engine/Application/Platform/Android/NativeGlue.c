/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2016-1-28
 * Update: 2019-1-7
 * Author: scott.cgi
 */


#include "Engine/Toolkit/Platform/Platform.h"


#ifdef IS_PLATFORM_ANDROID


#include <pthread.h>
#include <android/asset_manager.h>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#include <stdbool.h>
#include <stdio.h>

#include "Engine/Application/Application.h"
#include "Engine/Toolkit/Utils/FileTool.h"
#include "Engine/Graphics/OpenGL/Platform/EGLTool.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Application/Input.h"


enum
{
    LooperId_Input,
    LooperId_Sensor,
};


typedef volatile enum
{
    MainThread_OnLoop,
    MainThread_OnWait,
    MainThread_OnResized,
    MainThread_OnPause,
    MainThread_OnResume,
    MainThread_OnNeedCreateEGL,
    MainThread_OnDestroy,
}
MainThreadCallback;


//----------------------------------------------------------------------------------------------------------------------


ANativeActivity*      nativeActivity;
AConfiguration*        nativeActivityConfig;

static EGLDisplay     eglDisplay;
static EGLSurface     eglSurface;
static EGLContext     eglContext;
static EGLConfig       eglConfig;
static EGLint         eglFormat;

static ANativeWindow* nativeWindow;
static ALooper*       looper;
static AInputQueue*   inputQueue;

/**
 * The volatile make sure not optimized by the compiler,
 * because two threads one set and another get,
 * but look at one thread, it seems never modified.
 */
static volatile MainThreadCallback mainThreadCallback = MainThread_OnWait;
static          bool               isNeedCreateEGL    = true;
static          bool               isPaused           = true;


//----------------------------------------------------------------------------------------------------------------------


static void inline Touch(AInputEvent* event, int pointerIndex, InputTouchType inputTouchType)
{
    AApplication->Touch
    (
        AMotionEvent_getPointerId(event, pointerIndex),
        AMotionEvent_getX        (event, pointerIndex),
        AMotionEvent_getY        (event, pointerIndex),
        inputTouchType
    );
}


static void inline Touches(AInputEvent* event, InputTouchType inputTouchType)
{
    int  count = (int) AMotionEvent_getPointerCount(event);
    int  fingerIds[count];
    float pixelXs [count];
    float pixelYs [count];

    for (int i = 0; i < count; ++i)
    {
        fingerIds[i] = AMotionEvent_getPointerId(event, i);
        pixelXs[i]  = AMotionEvent_getX        (event, i);
        pixelYs[i]  = AMotionEvent_getY        (event, i);
    }

    AApplication->Touches(fingerIds, pixelXs, pixelYs, count, inputTouchType);
}


/**
 * Process the next input event
 */
static inline int32_t OnInputEvent(AInputEvent* event)
{
    switch (AInputEvent_getType(event))
    {
        case AINPUT_EVENT_TYPE_MOTION:
        {
            int32_t action = AMotionEvent_getAction(event);

            switch (action & AMOTION_EVENT_ACTION_MASK)
            {
                // first pointer down
                case AMOTION_EVENT_ACTION_DOWN:
                {
                    Touch(event, 0, InputTouchType_Down);
                    break;
                }

                // not first pointer down
                case AMOTION_EVENT_ACTION_POINTER_DOWN:
                {
                    Touch
                    (
                        event,
                        (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT,
                        InputTouchType_Down
                    );
                    break;
                }

                // first pinter up
                case AMOTION_EVENT_ACTION_UP:
                {
                    Touch(event, 0, InputTouchType_Up);
                    break;
                }


                // not first pointer up
                case AMOTION_EVENT_ACTION_POINTER_UP:
                {
                    Touch
                    (
                        event,
                        (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT,
                        InputTouchType_Up
                    );
                    break;
                }


                case AMOTION_EVENT_ACTION_MOVE:
                {
                    Touches(event, InputTouchType_Move);
                    break;
                }


                case AMOTION_EVENT_ACTION_CANCEL:
                {
                    Touches(event, InputTouchType_Cancel);
                    break;
                }

                default:
                    return 0;
            }

            return 1;
        }

        case AINPUT_EVENT_TYPE_KEY:
            break;
    }

    // default dispatching
    return 0;
}


static int LooperOnInputEvent(int fd, int events, void* data)
{
    AInputEvent* event;

    while (AInputQueue_getEvent(inputQueue, &event) >= 0)
    {
        if (AInputQueue_preDispatchEvent(inputQueue, event) == 0)
        {
            AInputQueue_finishEvent(inputQueue, event, OnInputEvent(event));
        }
    }

    return 1;
}


static void* ThreadRun(void* params)
{
    looper = ALooper_prepare(0);

    while (true)
    {
        switch (mainThreadCallback)
        {
            case MainThread_OnLoop:
                // handle event
                ALooper_pollAll(0, NULL, NULL, NULL);

                // application main loop
                AApplication->Loop();

                // render buffer
                eglSwapBuffers(eglDisplay, eglSurface);
                continue;

            case MainThread_OnDestroy:
                AEGLTool->DestroyEGL(&eglDisplay, &eglContext, &eglSurface);
                AApplication->Destroy();
                return NULL;

            case MainThread_OnPause: // sometimes before resized
                AApplication->Pause();
                mainThreadCallback = MainThread_OnWait;
                continue;

            case MainThread_OnResume:
                AApplication->Resume();
                mainThreadCallback = MainThread_OnWait;
                break;

            case MainThread_OnNeedCreateEGL:
                // we need create EGL and use openGL in one thread
                AEGLTool->CreateEGL(nativeWindow, &eglDisplay, &eglContext, &eglSurface, &eglConfig);

                // EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
                // guaranteed to be accepted by ANativeWindow_SetBuffersGeometry()
                // As soon as we picked a EGLConfig, we can safely reconfigure the
                // ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID
                eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &eglFormat);
                ANativeWindow_setBuffersGeometry(nativeWindow,  0, 0, eglFormat);

                AApplication->Ready
                (
                    ANativeWindow_getWidth (nativeWindow),
                    ANativeWindow_getHeight(nativeWindow)
                );
                mainThreadCallback = MainThread_OnLoop;
                break;

            case MainThread_OnResized:
                AEGLTool->ResetSurface(nativeWindow, eglDisplay, eglContext, eglConfig, &eglSurface);
                ANativeWindow_setBuffersGeometry(nativeWindow, 0, 0, eglFormat);
                AApplication->Resized
                (
                    ANativeWindow_getWidth(nativeWindow),
                    ANativeWindow_getHeight(nativeWindow)
                );
                mainThreadCallback = MainThread_OnLoop;
                break;

            case MainThread_OnWait:
                break;
        }
    }

    return NULL;
}


//----------------------------------------------------------------------------------------------------------------------


static void OnStart(ANativeActivity* activity)
{
    ALog_D("ANativeActivity OnStart");
}


static void OnResume(ANativeActivity* activity)
{
    ALog_D("ANativeActivity OnResume");
    mainThreadCallback = MainThread_OnResume;
    isPaused           = false;
}


static void* OnSaveInstanceState(ANativeActivity* activity, size_t* outSaveSize)
{
    ALog_D("ANativeActivity OnSaveInstanceState");
    *outSaveSize = 0;
    AApplication->SaveData();
    return NULL;
}


static void OnPause(ANativeActivity* activity)
{
    ALog_D("ANativeActivity OnPause");
    mainThreadCallback = MainThread_OnPause;
    isPaused           = true;
}


static void OnStop(ANativeActivity* activity)
{
    ALog_D("ANativeActivity OnStop");
}


static void OnDestroy(ANativeActivity* activity)
{
    ALog_D("ANativeActivity OnDestroy");
    mainThreadCallback = MainThread_OnDestroy;
}


static void OnWindowFocusChanged(ANativeActivity* activity, int hasFocus)
{
    ALog_D("ANativeActivity OnWindowFocusChanged");
}


static void OnNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
    ALog_D("ANativeActivity OnNativeWindowCreated");
    nativeWindow = window;
}


static void OnNativeWindowResized(ANativeActivity* activity, ANativeWindow* window)
{
    ALog_D("ANativeActivity OnNativeWindowResized");

    // when start app and from back to front screen

    if (isNeedCreateEGL == false)
    {
        mainThreadCallback = MainThread_OnResized;
        ALog_D("ANativeActivity in OnNativeWindowResized do MainThread_OnResized");
    }
    else
    {
        isNeedCreateEGL    = false;
        mainThreadCallback = MainThread_OnNeedCreateEGL;
        ALog_D("ANativeActivity in OnNativeWindowResized do MainThread_OnNeedCreateEGL");
    }
}


static void OnNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window)
{
    ALog_D("ANativeActivity OnNativeWindowRedrawNeeded");
}


static void OnNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
    ALog_D("ANativeActivity OnNativeWindowDestroyed");
    mainThreadCallback = MainThread_OnWait;
}


static void OnInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
    ALog_D("ANativeActivity OnInputQueueCreated");
    inputQueue = queue;
    AInputQueue_attachLooper(inputQueue, looper, LooperId_Input, LooperOnInputEvent, NULL);
}


static void OnInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
    ALog_D("ANativeActivity OnInputQueueDestroyed");
    AInputQueue_detachLooper(queue);
}


static void OnContentRectChanged(ANativeActivity* activity, const ARect* rect)
{
    ALog_D("ANativeActivity OnContentRectChanged");

    // when start app and lock/unlock screen

    if (isPaused == false)
    {
        if (isNeedCreateEGL == false)
        {
            if (mainThreadCallback != MainThread_OnNeedCreateEGL)
            {
                mainThreadCallback = MainThread_OnResized;
                ALog_D("ANativeActivity in OnContentRectChanged do MainThread_OnResized");
            }
        }
    }
}


static void OnConfigurationChanged(ANativeActivity* activity)
{
    ALog_D("ANativeActivity OnConfigurationChanged");
    AConfiguration_fromAssetManager(nativeActivityConfig, activity->assetManager);
}


static void OnLowMemory(ANativeActivity* activity)
{
    ALog_D("ANativeActivity OnLowMemory");
}


//----------------------------------------------------------------------------------------------------------------------


void ANativeActivity_OnCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
    ALog_D("ANativeActivity_OnCreate Start");
    nativeActivity                                  = activity;
    activity->callbacks->onStart                    = OnStart;
    activity->callbacks->onResume                   = OnResume;
    activity->callbacks->onSaveInstanceState        = OnSaveInstanceState;
    activity->callbacks->onPause                    = OnPause;
    activity->callbacks->onStop                     = OnStop;
    activity->callbacks->onDestroy                  = OnDestroy;
    activity->callbacks->onWindowFocusChanged       = OnWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated      = OnNativeWindowCreated;
    activity->callbacks->onNativeWindowResized      = OnNativeWindowResized;
    activity->callbacks->onNativeWindowRedrawNeeded = OnNativeWindowRedrawNeeded;
    activity->callbacks->onNativeWindowDestroyed    = OnNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated        = OnInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed      = OnInputQueueDestroyed;
    activity->callbacks->onContentRectChanged       = OnContentRectChanged;
    activity->callbacks->onConfigurationChanged      = OnConfigurationChanged;
    activity->callbacks->onLowMemory                = OnLowMemory;

    AApplication->Init();

//----------------------------------------------------------------------------------------------------------------------

    nativeActivityConfig = AConfiguration_new();
    AConfiguration_fromAssetManager(nativeActivityConfig, activity->assetManager);

    pthread_t      thread[1];
    pthread_attr_t attr  [1];
    pthread_attr_init          (attr);
    pthread_attr_setdetachstate(attr,   PTHREAD_CREATE_DETACHED);
    pthread_create             (thread, attr, ThreadRun, NULL);
    pthread_attr_destroy       (attr);
}


#endif
