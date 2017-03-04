/*
 *
 *
 *  Created on: 2016-1-28
 *      Author: scott.cgi
 */


#include <pthread.h>
#include <android/asset_manager.h>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#include <stdbool.h>
#include <stdio.h>

#include "Engine/Graphics/OpenGL/Head/egl.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Application/Application.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Application/Event.h"

static const char* saveDataFileName = "NDKSaveDataFile";

ANativeActivity* nativeActivity;

enum
{
	looper_id_input,
	looper_id_sensor,
};


typedef enum
{
	main_thread_on_null,
	main_thread_on_wait,
	main_thread_on_resized,
	main_thread_on_pause,
    main_thread_on_resume,
	main_thread_on_first_resized,
	main_thread_on_destroy,
}
MainThreadCallback;

//--------------------------------------------------------------------------------------------------

static struct
{
    EGLDisplay         display;
    EGLSurface         surface;
    EGLContext         context;
    EGLConfig          config;
	EGLint             format;

	AConfiguration*    assetConfig;
	ANativeWindow*     window;
	ALooper*           looper;
	AInputQueue*       inputQueue;

	MainThreadCallback mainThreadCallback;
}
AData[1] =
{
	{
		.mainThreadCallback = main_thread_on_wait,
	}
};

//--------------------------------------------------------------------------------------------------


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
					AApplicationOnTouch
					(
						AArrayMake
						(
							EventTouchPoint, 1,
							{
								AGLToolToGLX(AMotionEvent_getX(event, 0)),
								AGLToolToGLY(AMotionEvent_getY(event, 0)),
								AMotionEvent_getPointerId     (event, 0),
								event_touch_down,
							}
						)
					);
				}
				break;


				// not first pointer down
				case AMOTION_EVENT_ACTION_POINTER_DOWN:
				{
					int indexDown = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

					AApplicationOnTouch
					(
						AArrayMake
						(
							EventTouchPoint, 1,
							{
								AGLToolToGLX(AMotionEvent_getX(event, indexDown)),
								AGLToolToGLY(AMotionEvent_getY(event, indexDown)),
								AMotionEvent_getPointerId     (event, indexDown),
								event_touch_down,
							}
						)
					);
				}
				break;


				// first pinter up
				case AMOTION_EVENT_ACTION_UP:
				{
					AApplicationOnTouch
					(
						AArrayMake
						(
							EventTouchPoint, 1,
							{
								AGLToolToGLX(AMotionEvent_getX(event, 0)),
								AGLToolToGLY(AMotionEvent_getY(event, 0)),
								AMotionEvent_getPointerId     (event, 0),
								event_touch_up,
							}
						)
					);
				}
				break;


				// not first pointer up
				case AMOTION_EVENT_ACTION_POINTER_UP:
				{
					int indexUp = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

					AApplicationOnTouch
					(
						AArrayMake
						(
							EventTouchPoint, 1,
							{
								AGLToolToGLX(AMotionEvent_getX(event, indexUp)),
								AGLToolToGLY(AMotionEvent_getY(event, indexUp)),
								AMotionEvent_getPointerId     (event, indexUp),
								event_touch_up,
							}
						)
					);
				}
				break;

				case AMOTION_EVENT_ACTION_MOVE:
				{
					int count = AMotionEvent_getPointerCount(event);
					EventTouchPoint points[count];

					for (int i = 0; i < count; i++)
					{
						points[i].x    = AGLToolToGLX(AMotionEvent_getX(event, i));
						points[i].y    = AGLToolToGLY(AMotionEvent_getY(event, i));
						points[i].id   = AMotionEvent_getPointerId     (event, i);
						points[i].type = event_touch_move;
					}

					AApplicationOnTouch
					(
						(Array[]) {points, count}
					);
				}
				break;


				case AMOTION_EVENT_ACTION_CANCEL:
				{
					int count = AMotionEvent_getPointerCount(event);
					EventTouchPoint points[count];

					for (int i = 0; i < count; i++)
					{
						points[i].x    = AGLToolToGLX(AMotionEvent_getX(event, i));
						points[i].y    = AGLToolToGLY(AMotionEvent_getY(event, i));
						points[i].id   = AMotionEvent_getPointerId     (event, i);
						points[i].type = event_touch_cancel;
					}

					AApplicationOnTouch
					(
						(Array[]) {points, count}
					);
				}
				break;

				default:
					return 0;
			}

			return 1;
		}

		case AINPUT_EVENT_TYPE_KEY:
		{
		}

    }

    // default dispatching
    return 0;
}

static int LooperOnInputEvent(int fd, int events, void* data)
{
	AInputEvent* event;

	while (AInputQueue_getEvent(AData->inputQueue, &event) >= 0)
	{
		if (AInputQueue_preDispatchEvent(AData->inputQueue, event) == 0)
		{
			AInputQueue_finishEvent(AData->inputQueue, event, OnInputEvent(event));
		}
	}

	return 1;
}


static void* ThreadRun(void* param)
{
	AApplication->Init();

    AData->looper = ALooper_prepare(0);

    struct timespec now;
    struct timespec last;
    // start clock
    clock_gettime(CLOCK_MONOTONIC, &last);

//--------------------------------------------------------------------------------------------------

	while (true)
    {
    	clock_gettime(CLOCK_MONOTONIC, &now);
    	float deltaTime = (now.tv_nsec - last.tv_nsec) * 0.000000001 + (now.tv_sec - last.tv_sec);
		last            =  now;

        if (AData->mainThreadCallback == main_thread_on_null)
        {
            // handle event
            ALooper_pollAll(0, NULL, NULL, NULL);

            // application main loop
            AApplication->Loop(deltaTime);

            // render buffer
            eglSwapBuffers(AData->display, AData->surface);
        }
        else
        {
            switch (AData->mainThreadCallback)
            {
                case main_thread_on_destroy:
                    // call in main thread
                    AGLTool->DestroyEGL(&AData->display, &AData->context, &AData->surface);
                    return NULL;

                case main_thread_on_pause: // sometimes before resized
                    // call in main thread
                    AApplication->callbacks->OnPause();
                    AData->mainThreadCallback = main_thread_on_wait;
                    continue;

                case main_thread_on_resume:
                    // call in main thread
                    AApplication->callbacks->OnResume();
                    AData->mainThreadCallback = main_thread_on_null;
                    break;

                case main_thread_on_first_resized:
                    // we need create EGL and use openGL in one thread
                    // call in main thread
                    AGLTool->CreateEGL(AData->window, &AData->display, &AData->context, &AData->surface, &AData->config);

                    // EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
                    // guaranteed to be accepted by ANativeWindow_SetBuffersGeometry()
                    // As soon as we picked a EGLConfig, we can safely reconfigure the
                    // ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID
                    eglGetConfigAttrib              (AData->display, AData->config, EGL_NATIVE_VISUAL_ID, &AData->format);
                    ANativeWindow_setBuffersGeometry(AData->window,  0, 0, AData->format);
                    AApplicationOnResized(ANativeWindow_getWidth(AData->window), ANativeWindow_getHeight(AData->window));

                    AApplicationOnGLReady();
                    AApplication->callbacks->OnCreated();
                    AData->mainThreadCallback = main_thread_on_null;
                    break;

                case main_thread_on_resized:
                    // call in main thread
                    AGLTool->ResetSurface(AData->window, AData->display, AData->context, AData->config, &AData->surface);
                    ANativeWindow_setBuffersGeometry(AData->window, 0, 0, AData->format);
                    AApplicationOnResized(ANativeWindow_getWidth(AData->window), ANativeWindow_getHeight(AData->window));
                    AData->mainThreadCallback = main_thread_on_null;
                    break;

                case main_thread_on_wait:
                    continue;
            }
        }
    }

	return NULL;
}

//--------------------------------------------------------------------------------------------------

static inline void SaveData(ANativeActivity* activity)
{
    void* outSaveData;
    int   outSize;
    AApplication->callbacks->OnGetSaveData(&outSaveData, &outSize);

//--------------------------------------------------------------------------------------------------

    char filePath[100];
    sprintf(filePath, "%s/%s", activity->internalDataPath, saveDataFileName);

    FILE* f = fopen(filePath, "wb");
    fwrite(outSaveData, outSize, 1, f);
    fclose(f);
}


static void OnStart(ANativeActivity* activity)
{
	ALogD("NativeActivity OnStart");
}

static void OnResume(ANativeActivity* activity)
{
	ALogD("NativeActivity OnResume");
    AData->mainThreadCallback = main_thread_on_resume;
}

static void* OnSaveInstanceState(ANativeActivity* activity, size_t* outSize)
{
	ALogD("NativeActivity OnSaveInstanceState");
    SaveData(activity);
    *outSize = 0;
	return NULL;
}

static void OnPause(ANativeActivity* activity)
{
	ALogD("NativeActivity OnPause");
	AData->mainThreadCallback = main_thread_on_pause;
}

static void OnStop(ANativeActivity* activity)
{
	ALogD("NativeActivity OnStop");
}

static void OnDestroy(ANativeActivity* activity)
{
	ALogD("NativeActivity OnDestroy");
	AData->mainThreadCallback = main_thread_on_destroy;
}

static void OnWindowFocusChanged(ANativeActivity* activity, int hasFocus)
{
	ALogD("NativeActivity OnWindowFocusChanged");
}

static void OnNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
	ALogD("NativeActivity OnNativeWindowCreated");
	AData->window = window;
}

static void OnNativeWindowResized(ANativeActivity* activity, ANativeWindow* window)
{
	ALogD("NativeActivity OnNativeWindowResized");
	AData->window = window;

	static bool isFirst = true;
	if (isFirst)
	{
		isFirst = false;
		AData->mainThreadCallback = main_thread_on_first_resized;
	}
	else
	{
		AData->mainThreadCallback = main_thread_on_resized;
	}
}

static void OnNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window)
{
	ALogD("NativeActivity OnNativeWindowRedrawNeeded");
	AData->mainThreadCallback = main_thread_on_null;
}

static void OnNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
	ALogD("NativeActivity OnNativeWindowDestroyed");
	AData->mainThreadCallback = main_thread_on_wait;
}

static void OnInputQueueCreated(ANativeActivity* activity, AInputQueue* inputQueue)
{
	ALogD("NativeActivity OnInputQueueCreated");
	AData->inputQueue = inputQueue;
    AInputQueue_attachLooper(inputQueue, AData->looper, looper_id_input, LooperOnInputEvent, NULL);
}

static void OnInputQueueDestroyed(ANativeActivity* activity, AInputQueue* inputQueue)
{
	ALogD("NativeActivity OnInputQueueDestroyed");
    AInputQueue_detachLooper(inputQueue);
}

static void OnContentRectChanged(ANativeActivity* activity, const ARect* rect)
{
	ALogD("NativeActivity OnContentRectChanged");
}

static void OnConfigurationChanged(ANativeActivity* activity)
{
	ALogD("NativeActivity OnConfigurationChanged");
    AConfiguration_fromAssetManager(AData->assetConfig, activity->assetManager);
}

static void OnLowMemory(ANativeActivity* activity)
{
	ALogD("NativeActivity OnLowMemory");
}

//--------------------------------------------------------------------------------------------------

void ANativeActivityOnCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
	ALogD("ANativeActivityOnCreate Start");

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
    activity->callbacks->onConfigurationChanged     = OnConfigurationChanged;
    activity->callbacks->onLowMemory                = OnLowMemory;

    ApplicationMain();

    ALogA(AApplication->callbacks->OnCreated     != NULL, "AApplication->callbacks->OnCreated     must set");
    ALogA(AApplication->callbacks->OnPause       != NULL, "AApplication->callbacks->OnPause       must set");
    ALogA(AApplication->callbacks->OnResume      != NULL, "AApplication->callbacks->OnResume      must set");
    ALogA(AApplication->callbacks->OnResized     != NULL, "AApplication->callbacks->OnResized     must set");
    ALogA(AApplication->callbacks->OnGetSaveData != NULL, "AApplication->callbacks->OnGetSaveData must set");
    ALogA(AApplication->callbacks->OnSetSaveData != NULL, "AApplication->callbacks->OnSetSaveData must set");

//--------------------------------------------------------------------------------------------------

    char filePath[100];
    sprintf(filePath, "%s/%s", activity->internalDataPath, saveDataFileName);

    FILE* f = fopen(filePath, "rb");
    if (f != NULL)
    {
        fseek(f, 0, SEEK_END);
        long  length = ftell(f);
        char* data   = malloc(length);
        fseek(f, 0, SEEK_SET);

        fread(data, length, 1, f);
        AApplication->callbacks->OnSetSaveData(data, length);

        free(data);
        fclose(f);
    }

//--------------------------------------------------------------------------------------------------

    AData->assetConfig = AConfiguration_new();
    AConfiguration_fromAssetManager(AData->assetConfig, activity->assetManager);
    nativeActivity     = activity;

	pthread_t      thread[1];
	pthread_attr_t attr  [1];
	pthread_attr_init          (attr);
	pthread_attr_setdetachstate(attr,   PTHREAD_CREATE_DETACHED);
	pthread_create             (thread, attr, ThreadRun, NULL);
	pthread_attr_destroy       (attr);
}