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


static bool OnMessage(Component* component, void* sender, int subject, void* extraData)
{
	if (sender == AApplication)
	{
		switch (subject)
		{
			case application_msg_on_gl_ready:
			{
				AGraphics->Init();
			}
			break;

			case application_msg_on_touch:
			{
				AComponent->SendMessageToChildren(AApplication->component, AApplication, subject, extraData);
			}
			break;

			case application_msg_on_resized:
			{
                int width  = ((int*) extraData)[0];
                int height = ((int*) extraData)[1];

                AGLTool     ->SetSize             (width, height);
                AApplication->callbacks->OnResized(width, height);
			}
			break;
		}
	}

	return false;
}

static void Init()
{
	AToolkit  ->Init();
	APhysics  ->Init();
	AExtension->Init();
	AAudio    ->Init();

    AComponent->Init(AApplication->component);
    AApplication->component->defualtState->OnMessage = OnMessage;
}


static void Loop(float deltaTime)
{
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

struct AApplication AApplication[1] =
{
	{
		.callbacks =
        {
            {
               .OnCreated     = NULL,
               .OnPause       = NULL,
               .OnResume      = NULL,
               .OnResized     = NULL,
               .OnGetSaveData = NULL,
               .OnSetSaveData = NULL,
            }
        },
		.Init = Init,
		.Loop = Loop,
	}
};

