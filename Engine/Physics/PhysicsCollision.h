/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2014-7-23
 * Update: 2019-1-18
 * Author: scott.cgi
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
