/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2014-6-3
 * Author: scott.cgi
 */

#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H


#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Head/Bitwise.h"
#include "Engine/Toolkit/Utils/Array.h"
#include "Engine/Toolkit/Head/UserData.h"


typedef enum
{
    PhysicsShape_Polygon = 1,
    PhysicsShape_Line    = 1 << 2,
    PhysicsShape_Point   = 1 << 3,
}
PhysicsShape;


typedef enum
{
    /**
     * Can motion can collision
     */
    PhysicsBodyState_Normal,

    /**
     * No motion can collision
     */
    PhysicsBodyState_Fixed,

    /**
     * No motion no collision
     */
    PhysicsBodyState_Freeze,
}
PhysicsBodyState;


/**
 * Recommended cache and reused
 */
typedef struct PhysicsBody PhysicsBody;
struct  PhysicsBody
{
    UserData         userData[1];

    /**
     * Default -1 used to identify PhysicsBody
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
     * PhysicsBody current state
     */
    PhysicsBodyState state;

    /**
     * Pow of 2, default 0
     * body can collision between different collisionGroup (no identical bits)
     */
    int              collisionGroup;

    /**
     * Hold born vertices, do not modify
     */
    Array(float)      vertexArr  [1];

    /**
     * The vertices after transformed
     */
    Array(float)      positionArr[1];

    /**
     * When body collision callback
     */
    void (*OnCollision)(PhysicsBody* self, PhysicsBody* other, float deltaSeconds);
};


struct APhysicsBody
{
    /**
     * Create body with shape by vertices
     * if shape not support will return NULL
     */
    PhysicsBody* (*Create)      (PhysicsShape shape, Array(float)* vertexArr);

    /**
     * Update body shape motion
     */
    void         (*UpdateMotion)(PhysicsBody* body, float deltaSeconds);

};


extern struct APhysicsBody APhysicsBody[1];


//----------------------------------------------------------------------------------------------------------------------


static inline bool APhysicsBody_CheckCollisionGroup(PhysicsBody* physicsBody, int checkCollisionGroup)
{
    return ABitwise_Check(physicsBody->collisionGroup, checkCollisionGroup);
}


static inline void APhysicsBody_AddCollisionGroup(PhysicsBody* physicsBody, int addCollisionGroup)
{
    ABitwise_Add(physicsBody->collisionGroup, addCollisionGroup);
}


static inline void APhysicsBody_SetCollisionGroup(PhysicsBody* physicsBody, int setCollisionGroup)
{
    ABitwise_Set(physicsBody->collisionGroup, setCollisionGroup);
}


static inline void APhysicsBody_ClearCollisionGroup(PhysicsBody* physicsBody, int clearCollisionGroup)
{
    ABitwise_Clear(physicsBody->collisionGroup, clearCollisionGroup);
}


#endif
