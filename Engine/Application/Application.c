/**
 *
 *  Created on: 2013-9-4
 *  Author: scott.cgi
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


static struct timespec now;
static struct timespec last;


static void Init()
{
	AToolkit  ->Init();
	APhysics  ->Init();
	AExtension->Init();
	AAudio    ->Init();

    AComponent->Init(AApplication->component);

    // entry called
    ApplicationMain();

    // check callback setting
    ALogA(AApplication->callbacks->OnReady       != NULL, "AApplication->callbacks->OnReady       must set");
    ALogA(AApplication->callbacks->OnPause       != NULL, "AApplication->callbacks->OnPause       must set");
    ALogA(AApplication->callbacks->OnResume      != NULL, "AApplication->callbacks->OnResume      must set");
    ALogA(AApplication->callbacks->OnResized     != NULL, "AApplication->callbacks->OnResized     must set");
    ALogA(AApplication->callbacks->OnGetSaveData != NULL, "AApplication->callbacks->OnGetSaveData must set");
    ALogA(AApplication->callbacks->OnSetSaveData != NULL, "AApplication->callbacks->OnSetSaveData must set");

    // start clock
    clock_gettime(CLOCK_MONOTONIC, &last);
}


static void Loop()
{
    clock_gettime(CLOCK_MONOTONIC, &now);
    float deltaTime = (now.tv_nsec - last.tv_nsec) * 0.000000001 + (now.tv_sec - last.tv_sec);
    last            =  now;

//--------------------------------------------------------------------------------------------------

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	AScheduler   ->Update(deltaTime);
	ACoroutine   ->Update(deltaTime);
	ATween       ->Update(deltaTime);
	APhysicsWorld->Update(deltaTime);
	AAudio       ->Update(deltaTime);

	// root update
	AComponent   ->Update(AApplication->component, deltaTime);

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


static void Touch(Array(InputTouch*)* touchData)
{
    AComponent->SendMessageToChildren
    (
        AApplication->component,
        AApplication,
        application_msg_on_touch,
        touchData
    );
}


struct AApplication AApplication[1] =
{
	{
		.callbacks =
        {
            {
               .OnReady       = NULL,
               .OnPause       = NULL,
               .OnResume      = NULL,
               .OnResized     = NULL,
               .OnGetSaveData = NULL,
               .OnSetSaveData = NULL,
            }
        },

		.Init    = Init,
		.Loop    = Loop,
        .GLReady = GLReady,
        .Resized = Resized,
        .Pause   = Pause,
        .Resume  = Resume,
        .Touch   = Touch,
	}
};

