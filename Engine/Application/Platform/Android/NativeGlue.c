/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2016-1-28
 * Author: scott.cgi
 */

#include "Engine/Toolkit/Platform/Platform.h"


//----------------------------------------------------------------------------------------------------------------------
#ifdef IS_PLATFORM_ANDROID
//----------------------------------------------------------------------------------------------------------------------


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
    MainThread_OnFirstResized,
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
 * because two threads will modify mainThreadCallback.
 */
static volatile MainThreadCallback mainThreadCallback = MainThread_OnWait;
static bool     isOnFirstResized                      = true;

//----------------------------------------------------------------------------------------------------------------------


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
                    AApplication->Touch
                    (
                        AArray_Make
                        (
                            InputTouch*, 1,
                            AInput->SetTouch
                            (
                                AMotionEvent_getPointerId(event, 0),
                                AMotionEvent_getX        (event, 0),
                                AMotionEvent_getY        (event, 0),
                                InputTouchType_Down
                            )
                        )
                    );

                    break;
                }

                // not first pointer down
                case AMOTION_EVENT_ACTION_POINTER_DOWN:
                {
                    int indexDown = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

                    AApplication->Touch
                    (
                        AArray_Make
                        (
                            InputTouch*, 1,
                            AInput->SetTouch
                            (
                                AMotionEvent_getPointerId(event, indexDown),
                                AMotionEvent_getX        (event, indexDown),
                                AMotionEvent_getY        (event, indexDown),
                                InputTouchType_Down
                            )
                        )
                    );

                    break;
                }

                // first pinter up
                case AMOTION_EVENT_ACTION_UP:
                {
                    AApplication->Touch
                    (
                        AArray_Make
                        (
                            InputTouch*, 1,
                            AInput->SetTouch
                            (
                                AMotionEvent_getPointerId(event, 0),
                                AMotionEvent_getX        (event, 0),
                                AMotionEvent_getY        (event, 0),
                                InputTouchType_Up
                            )
                        )
                    );

                    break;
                }


                // not first pointer up
                case AMOTION_EVENT_ACTION_POINTER_UP:
                {
                    int indexUp = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

                    AApplication->Touch
                    (
                        AArray_Make
                        (
                            InputTouch*, 1,
                            AInput->SetTouch
                            (
                                AMotionEvent_getPointerId(event, indexUp),
                                AMotionEvent_getX        (event, indexUp),
                                AMotionEvent_getY        (event, indexUp),
                                InputTouchType_Up
                            )
                        )
                    );

                    break;
                }


                case AMOTION_EVENT_ACTION_MOVE:
                {
                    int count = AMotionEvent_getPointerCount(event);
                    InputTouch* touches[count];

                    for (int i = 0; i < count; i++)
                    {
                        InputTouch* touch = AInput->SetTouch
                                            (
                                                AMotionEvent_getPointerId(event, i),
                                                AMotionEvent_getX        (event, i),
                                                AMotionEvent_getY        (event, i),
                                                InputTouchType_Move
                                            );

                        touches[i]        = touch;
                    }

                    AApplication->Touch
                    (
                        (Array[]) {touches, count}
                    );

                    break;
                }


                case AMOTION_EVENT_ACTION_CANCEL:
                {
                    int count = AMotionEvent_getPointerCount(event);
                    InputTouch* touches[count];

                    for (int i = 0; i < count; i++)
                    {
                        InputTouch* touch = AInput->SetTouch
                                            (
                                                AMotionEvent_getPointerId(event, i),
                                                AMotionEvent_getX        (event, i),
                                                AMotionEvent_getY        (event, i),
                                                InputTouchType_Cancel
                                            );

                        touches[i]        = touch;
                    }

                    AApplication->Touch
                    (
                        (Array[]) {touches, count}
                    );

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


static void* ThreadRun(void* param)
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
                // call in main thread
                AEGLTool->DestroyEGL(&eglDisplay, &eglContext, &eglSurface);
                AApplication->Destroy();
                return NULL;

            case MainThread_OnPause: // sometimes before resized
                // call in main thread
                AApplication->Pause();
                mainThreadCallback = MainThread_OnWait;
                continue;

            case MainThread_OnResume:
                // call in main thread
                AApplication->Resume();

                if (isOnFirstResized)
                {
                    mainThreadCallback = MainThread_OnWait;
                }
                else
                {
                    mainThreadCallback = MainThread_OnLoop;
                }
                break;

            case MainThread_OnFirstResized:
                // we need create EGL and use openGL in one thread
                // call in main thread
                AEGLTool->CreateEGL(nativeWindow, &eglDisplay, &eglContext, &eglSurface, &eglConfig);

                // EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
                // guaranteed to be accepted by ANativeWindow_SetBuffersGeometry()
                // As soon as we picked a EGLConfig, we can safely reconfigure the
                // ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID
                eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &eglFormat);
                ANativeWindow_setBuffersGeometry(nativeWindow,  0, 0, eglFormat);

                AApplication->GLReady
                (
                    ANativeWindow_getWidth (nativeWindow),
                    ANativeWindow_getHeight(nativeWindow)
                );

                mainThreadCallback = MainThread_OnLoop;
                break;

            case MainThread_OnResized:
                // call in main thread
                AEGLTool->ResetSurface(nativeWindow, eglDisplay, eglContext, eglConfig, &eglSurface);
                ANativeWindow_setBuffersGeometry(nativeWindow, 0, 0, eglFormat);
                AApplication->Resized(ANativeWindow_getWidth(nativeWindow), ANativeWindow_getHeight(nativeWindow));
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
    nativeWindow = window;

    if (isOnFirstResized)
    {
        isOnFirstResized   = false;
        mainThreadCallback = MainThread_OnFirstResized;
    }
    else
    {
        mainThreadCallback = MainThread_OnResized;
    }
}


static void OnNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window)
{
    ALog_D("ANativeActivity OnNativeWindowRedrawNeeded");
    mainThreadCallback = MainThread_OnLoop;
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


//----------------------------------------------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------------------------------------------
