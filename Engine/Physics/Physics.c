/*
 *
 *
 *  Created on: 2014-6-11
 *      Author: scott.cgi
 */

#include "Mojoc/Physics/Physics.h"
#include "Mojoc/Physics/PhysicsWorld.h"
#include "Mojoc/Toolkit/Platform/Log.h"



static void Init()
{
	APhysicsWorld->gravity.y = -1.5f;
}


struct APhysics APhysics[1] =
{
	Init
};
