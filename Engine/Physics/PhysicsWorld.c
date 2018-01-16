/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2014-5-30
 * Author: scott.cgi
 */

#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Toolkit/Utils/ArrayIntSet.h"
#include "Engine/Physics/PhysicsCollision.h"


static ArrayIntSet(PhysicsBody*) bodySet[1] = AArrayIntSet_Init(PhysicsBody*, 20);


//----------------------------------------------------------------------------------------------------------------------


static inline void UpdateMotion(PhysicsBody* body, float deltaSeconds)
{
    // cache v0
    float vx         = body->velocityX;
    float vy         = body->velocityY;

    // get final velocity in x and y direction
    // v1 = at + v0
    body->velocityX += (body->accelerationX + APhysicsWorld->gravity.x) * deltaSeconds;
    body->velocityY += (body->accelerationY + APhysicsWorld->gravity.y) * deltaSeconds;

    // get delta distance in x and y indirection
    // s = (v0 + v1) * t / 2
    float dx         = (body->velocityX + vx) * deltaSeconds * 0.5f;
    float dy         = (body->velocityY + vy) * deltaSeconds * 0.5f;

    // increase x and y distance
    body->positionX += dx;
    body->positionY += dy;

    body->rotationZ  = AMath_Atan2(dx, dy);

    APhysicsBody->UpdateMotion(body, deltaSeconds);

    if (AMath_TestFloatEqual(body->velocityX, 0.0f) && AMath_TestFloatEqual(body->velocityY, 0.0f))
    {
        APhysicsBody_SetState(body, PhysicsBodyState_Sleeping);
    }
}


static void Update(float deltaSeconds)
{
    for (int i = 0; i < bodySet->elementList->size; i++)
    {
        PhysicsBody* body = AArrayList_Get(bodySet->elementList, i, PhysicsBody*);

        if (APhysicsBody_CheckState(body, PhysicsBodyState_NoCollision) == false)
        {
            // test collision
            for (int fromIndex = i + 1; fromIndex < bodySet->elementList->size; fromIndex++)
            {
                PhysicsBody* otherBody = AArrayList_Get(bodySet->elementList, fromIndex, PhysicsBody*);

                if
                (
                    APhysicsBody_CheckState(otherBody, PhysicsBodyState_NoCollision) == false &&
                    (body->collisionGroup & otherBody->collisionGroup)               == 0     &&
                    APhysicsCollision->TestCollision(body, otherBody)
                )
                {
                    APhysicsBody_ClearAndSetState(body,      PhysicsBodyState_Sleeping, PhysicsBodyState_Collision);
                    APhysicsBody_ClearAndSetState(otherBody, PhysicsBodyState_Sleeping, PhysicsBodyState_Collision);

                    if (body->OnCollision != NULL)
                    {
                        body->OnCollision(body, otherBody, deltaSeconds);
                    }

                    if (otherBody->OnCollision != NULL)
                    {
                        otherBody->OnCollision(otherBody, body, deltaSeconds);
                    }
                }
            }
        }

        if (APhysicsBody_CheckState(body, PhysicsBodyState_NoMotion) == false)
        {
            // after test collision can update motion
            UpdateMotion(body, deltaSeconds);
        }
    }
}



static PhysicsBody* AddBody(PhysicsShape shape, Array(float)* vertexArr)
{
    PhysicsBody* body = APhysicsBody->Create(shape, vertexArr);
    AArrayIntSet->TryAdd(bodySet, (intptr_t) body);

    return body;
}


static void DestroyBody(PhysicsBody* body)
{
    AArrayIntSet->TryRemove(bodySet, (intptr_t) body);
    free(body);
}


struct APhysicsWorld APhysicsWorld[1] =
{
    {
        .AddBody     = AddBody,
        .DestroyBody = DestroyBody,
        .Update      = Update,
    }
};
