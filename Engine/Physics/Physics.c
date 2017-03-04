/*
 *
 *
 *  Created on: 2014-6-11
 *      Author: scott.cgi
 */

#include "Engine/Physics/Physics.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Toolkit/Platform/Log.h"



static void Init()
{
	APhysicsWorld->gravity.y = -1.5f;
}


struct APhysics APhysics[1] =
{
	Init
};
