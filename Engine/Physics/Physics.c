/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The Mojoc game engine is a serious project with extreme code pursuit,
 * and will continue to iterate.
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
