/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2014-5-30
 * Author : scott.cgi
 * Version: 0.0.0
 */


#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H


#include "Engine/Toolkit/Head/Rect.h"
#include "Engine/Physics/PhysicsBody.h"
#include "Engine/Toolkit/Math/Vector.h"


struct APhysicsWorld
{
    Vector2 gravity;
    Rect    range;

    /**
     * Add body in PhysicsWorld with shape and vertex
     * the vertexArr will copy into Body
     */
    PhysicsBody* (*AddBody)    (PhysicsShape shape, Array(float)* vertexArr);

    /**
     * Destroy PhysicsBody from world
     */
    void         (*DestroyBody)(PhysicsBody* body);

    /**
     * Every frame update PhysicsBody
     */
    void         (*Update)     (float deltaSeconds);
};


extern struct APhysicsWorld APhysicsWorld[1];


#endif
