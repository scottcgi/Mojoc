/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2014-6-3
 * Update: 2019-1-18
 * Author: scott.cgi
 */


#ifndef PHYSICS_BODY_H
#define  PHYSICS_BODY_H


#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/HeaderUtils/Bitwise.h"
#include "Engine/Toolkit/Utils/Array.h"
#include "Engine/Toolkit/HeaderUtils/UserData.h"


typedef enum
{
    PhysicsShape_NULL    = 0,
    PhysicsShape_Polygon = 1,
    PhysicsShape_Line    = 1 << 2,
    PhysicsShape_Point   = 1 << 3,
}
PhysicsShape;


typedef enum
{
    /**
     * Not add in physics world yet.
     */
    PhysicsBodyState_OutsideWorld,

    /**
     * Can motion can collision.
     */
    PhysicsBodyState_Normal,

    /**
     * No motion can collision.
     */
    PhysicsBodyState_Fixed,

    /**
     * No motion no collision.
     */
    PhysicsBodyState_Freeze,
}
PhysicsBodyState;


typedef struct PhysicsBody PhysicsBody;
struct  PhysicsBody
{
    UserData         userData[1];

    /**
     * Used to identify PhysicsBody, default -1.
     */
    int              userId;

    float             positionX;
    float             positionY;
    float             velocityX;
    float             velocityY;
    float             accelerationX;
    float             accelerationY;
    float             rotationZ;

    PhysicsShape     shape;

    /**
     * PhysicsBody current state.
     */
    PhysicsBodyState state;

    /**
     * Pow of 2, default 0.
     * body can collision between different collisionGroup (no same bit).
     */
    int              collisionGroup;

    /**
     * Store born vertices.
     */
    Array(float)      vertexArr[1];

    /**
     * The vertices after transformed.
     */
    Array(float)      transformedVertexArr[1];

    /**
     * When body collision callback.
     */
    void (*OnCollision)(PhysicsBody* self, PhysicsBody* other, float deltaSeconds);
};


/**
 * Control PhysicsBody.
 */
struct APhysicsBody
{
    /**
     * Create body with shape and vertices.
     *
     * the vertexArr will copy into body,
     * and the body's vertexArr and transformedVertexArr are same when init,
     * and all data create by one malloc.
     *
     * if shape not support will return NULL.
     */
    PhysicsBody* (*Create)       (PhysicsShape shape, Array(float)* vertexArr);

    /**
     * Reset body's transformedVertexArr to vertexArr.
     */
    void         (*ResetVertices)(PhysicsBody* body);

    /**
     * Simulate body motion and update transformedVertexArr by position, rotation.
     */
    void         (*Update)       (PhysicsBody* body, float deltaSeconds);

};


extern struct APhysicsBody APhysicsBody[1];


//----------------------------------------------------------------------------------------------------------------------


/**
 *  Check physicsBody whether has same bit in collisionGroup.
 */
static inline bool APhysicsBody_CheckCollisionGroup(PhysicsBody* physicsBody, int collisionGroup)
{
    return ABitwise_Check(physicsBody->collisionGroup, collisionGroup); // NOLINT(hicpp-signed-bitwise)
}


/**
 * Add collisionGroup to physicsBody.
 */
static inline void APhysicsBody_AddCollisionGroup(PhysicsBody* physicsBody, int collisionGroup)
{
    ABitwise_Add(physicsBody->collisionGroup, collisionGroup);
}


/**
 * Set collisionGroup to physicsBody.
 */
static inline void APhysicsBody_SetCollisionGroup(PhysicsBody* physicsBody, int collisionGroup)
{
    ABitwise_Set(physicsBody->collisionGroup, collisionGroup);
}


/**
 * Clear collisionGroup in physicsBody.
 */
static inline void APhysicsBody_ClearCollisionGroup(PhysicsBody* physicsBody, int collisionGroup)
{
    ABitwise_Clear(physicsBody->collisionGroup, collisionGroup);  // NOLINT(hicpp-signed-bitwise)
}


#endif
