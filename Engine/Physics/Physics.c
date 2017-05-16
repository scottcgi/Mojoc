/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2014-6-11
 * Author : scott.cgi
 * Version: 0.1
 */


#include "Engine/Physics/Physics.h"
#include "Engine/Physics/PhysicsWorld.h"


static void Init()
{
	APhysicsWorld->gravity.y = -1.5f;
}


struct APhysics APhysics[1] =
{
	Init
};
