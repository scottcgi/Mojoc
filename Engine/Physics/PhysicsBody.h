/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2014-6-3
 * Author : scott.cgi
 * Version: 0.0.0
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
    PhysicsShape_Length,
}
PhysicsShape;


/**
 * If contains 'is' the state can set and clear
 * else the state will automatically set and clear
 */
typedef enum
{
    PhysicsBodyState_Null          = 0,

    /**
     * Auto set no motion can collision, collision will wake up
     */
    PhysicsBodyState_Sleeping      = 1,

    /**
     * Auto set whether has collision
     */
    PhysicsBodyState_Collision     = 1 << 1,

//----------------------------------------------------------------------------------------------------------------------

    /**
     * No motion can collision
     */
    PhysicsBodyState_IsFixed       = 1 << 2,

    /**
     * No motion no collision forever
     */
    PhysicsBodyState_IsFreeze      = 1 << 3,


//----------------------------------------------------------------------------------------------------------------------


    PhysicsBodyState_NoMotion      = PhysicsBodyState_Sleeping |
                                     PhysicsBodyState_IsFixed  |
                                     PhysicsBodyState_IsFreeze,

   PhysicsBodyState_NoCollision    = PhysicsBodyState_IsFreeze,

}
PhysicsBodyState;


/**
 * Recommended cache and reused
 */
typedef struct PhysicsBody PhysicsBody;
struct  PhysicsBody
{
    UserData     userData[1];

    /**
     * Default -1 used to identify PhysicsBody
     */
    int          userId;

    float        positionX;
    float        positionY;
    float        velocityX;
    float        velocityY;
    float        accelerationX;
    float        accelerationY;
    float        rotationZ;

    PhysicsShape shape;

    /**
     * Hold born vertices, not modify
     */
    Array(float) vertexArr  [1];

    /**
     * The vertices after transformed
     */
    Array(float) positionArr[1];

    /**
     * Hold PhysicsBodyState
     */
    int          state;

    /**
     * Pow of 2, default 0
     * body can collision between different collisionGroup
     */
    int          collisionGroup;

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


static inline bool APhysicsBody_CheckState(PhysicsBody* physicsBody, PhysicsBodyState checkState)
{
    return ABitwise_Check(physicsBody->state, checkState);
}


static inline void APhysicsBody_SetState(PhysicsBody* physicsBody, PhysicsBodyState setState)
{
    ABitwise_Set(physicsBody->state, setState);
}


static inline void APhysicsBody_SetOnlyState(PhysicsBody* physicsBody, PhysicsBodyState setOnlyState)
{
    ABitwise_SetOnly(physicsBody->state, setOnlyState);
}


static inline void APhysicsBody_ClearState(PhysicsBody* physicsBody, PhysicsBodyState clearState)
{
    ABitwise_Clear(physicsBody->state, clearState);
}


static inline void APhysicsBody_ClearAndSetState(PhysicsBody* physicsBody, PhysicsBodyState clearState, PhysicsBodyState setState)
{
    ABitwise_ClearAndSet(physicsBody->state, clearState, setState);
}


//----------------------------------------------------------------------------------------------------------------------


static inline bool APhysicsBody_CheckCollisionGroup(PhysicsBody* physicsBody,  int checkCollisionGroup)
{
    return ABitwise_Check(physicsBody->collisionGroup, checkCollisionGroup);
}


static inline void APhysicsBody_SetCollisionGroup(PhysicsBody* physicsBody,    int setCollisionGroup)
{
    ABitwise_Set(physicsBody->collisionGroup, setCollisionGroup);
}


static inline void APhysicsBody_SetOnlyCollisionGroup(PhysicsBody* physicsBody, int setOnlycollisionGroup)
{
    ABitwise_SetOnly(physicsBody->collisionGroup, setOnlycollisionGroup);
}


static inline void APhysicsBody_ClearCollisionGroup(PhysicsBody* physicsBody,   int clearCollisionGroup)
{
    ABitwise_Clear(physicsBody->collisionGroup, clearCollisionGroup);
}


#endif
