/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2014-7-23
 * Author : scott.cgi
 * Version: 0.0.0
 */

#ifndef PHYSICS_COLLISION_H
#define PHYSICS_COLLISION_H


#include <stdbool.h>
#include "Engine/Physics/PhysicsBody.h"


struct APhysicsCollision
{
    bool (*TestCollision)(PhysicsBody* bodyA, PhysicsBody* bodyB);
};


extern struct APhysicsCollision APhysicsCollision[1];


#endif 
