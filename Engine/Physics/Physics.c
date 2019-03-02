/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2014-6-11
 * Update: 2019-1-18
 * Author: scott.cgi
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
