/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2014-5-30
 * Update: 2019-1-18
 * Author: scott.cgi
 */


#ifndef PHYSICS_WORLD_H
#define  PHYSICS_WORLD_H


#include "Engine/Physics/PhysicsBody.h"
#include "Engine/Toolkit/Math/Vector.h"


/**
 * Control and set up physics world.
 */
struct APhysicsWorld
{
    Vector2 gravity;

    /**
     * Create body and add in PhysicsWorld.
     */
    PhysicsBody* (*AddBody)    (PhysicsShape shape, Array(float)* vertexArr);

    /**
     * Remove PhysicsBody from world and free it.
     * 
     * important: after DestroyBody the body will be invalidated.
     */
    void         (*DestroyBody)(PhysicsBody* body);

    /**
     * Called every frame by loop.
     */
    void         (*Update)     (float deltaSeconds);
};


extern struct APhysicsWorld APhysicsWorld[1];


#endif
