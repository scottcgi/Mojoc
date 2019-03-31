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
 * The project Mojoc is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2014-5-30
 * Update: 2019-1-18
 * Author: scott.cgi
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
