/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2014-7-23
 * Update : 2019-1-18
 * Author : scott.cgi
 */


#ifndef PHYSICS_COLLISION_H
#define PHYSICS_COLLISION_H


#include <stdbool.h>
#include "Engine/Physics/PhysicsBody.h"


/**
 * Control physics collision.
 */
struct APhysicsCollision
{
    bool (*TestCollision)(PhysicsBody* bodyA, PhysicsBody* bodyB);
};


extern struct APhysicsCollision APhysicsCollision[1];


#endif 
