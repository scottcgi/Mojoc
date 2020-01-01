/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2014-5-30
 * Update   : 2019-1-18
 * Author   : scott.cgi
 */


#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H


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
