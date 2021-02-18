/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2014-5-30
 * Update   : 2019-1-18
 * Author   : scott.cgi
 */


#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Toolkit/Utils/ArrayIntSet.h"
#include "Engine/Physics/PhysicsCollision.h"


static ArrayIntSet(PhysicsBody*) bodyInWorldSet[1] = AArrayIntSet_Init(PhysicsBody*, 20);


static void Update(float deltaSeconds)
{
    for (int i = 0; i < bodyInWorldSet->elementList->size; ++i)
    {
        PhysicsBody* body = AArrayList_Get(bodyInWorldSet->elementList, i, PhysicsBody*);

        if (body->state != PhysicsBodyState_Freeze)
        {
            // test collision
            for (int fromIndex = i + 1; fromIndex < bodyInWorldSet->elementList->size; ++fromIndex)
            {
                PhysicsBody* otherBody = AArrayList_Get(bodyInWorldSet->elementList, fromIndex, PhysicsBody*);

                if
                (
                    otherBody->state != PhysicsBodyState_Freeze &&
                    // check collisionGroup whether has same bit
                    (body->collisionGroup & otherBody->collisionGroup) == 0 &&
                    APhysicsCollision->TestCollision(body, otherBody)
                )
                {
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

        if (body->state != PhysicsBodyState_Freeze && body->state != PhysicsBodyState_Fixed)
        {
            // after test collision can update motion
            APhysicsBody->Update(body, deltaSeconds);
        }
    }
}



static PhysicsBody* AddBody(PhysicsShape shape, Array(float)* vertexArr)
{
    PhysicsBody* body = APhysicsBody->Create(shape, vertexArr);
    AArrayIntSet->TryAdd(bodyInWorldSet, (intptr_t) body);

    return body;
}


static void DestroyBody(PhysicsBody* body)
{
    AArrayIntSet->TryRemove(bodyInWorldSet, (intptr_t) body);
    free(body);
}


struct APhysicsWorld APhysicsWorld[1] =
{{
    {0.0f, 0.0f},
    AddBody,
    DestroyBody,
    Update,
}};
