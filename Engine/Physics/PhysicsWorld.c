/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2014-5-30
 * Author: scott.cgi
 */


#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Toolkit/Utils/ArrayIntSet.h"
#include "Engine/Physics/PhysicsCollision.h"


static ArrayIntSet(PhysicsBody*) bodySet[1] = AArrayIntSetInit(PhysicsBody*, 20);


//--------------------------------------------------------------------------------------------------


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

	body->rotationZ  = AMathAtan2(dx, dy);

	APhysicsBody->UpdateMotion(body, deltaSeconds);

	if (AMathTestFloatEqual(body->velocityX, 0.0f) && AMathTestFloatEqual(body->velocityY, 0.0f))
	{
		APhysicsBodySetState(body, physics_body_state_sleeping);
	}
}


static void Update(float deltaSeconds)
{
	for (int i = 0; i < bodySet->elementList->size; i++)
	{
		PhysicsBody* body = AArrayListGet(bodySet->elementList, i, PhysicsBody*);

        if (APhysicsBodyCheckState(body, physics_body_state_no_collision) == false)
        {
            // test collision
            for (int fromIndex = i + 1; fromIndex < bodySet->elementList->size; fromIndex++)
            {
                PhysicsBody* otherBody = AArrayListGet(bodySet->elementList, fromIndex, PhysicsBody*);

                if
                (
                    APhysicsBodyCheckState(otherBody, physics_body_state_no_collision) == false &&
                    (body->collisionGroup & otherBody->collisionGroup)                 == 0     &&
                    APhysicsCollision->TestCollision(body, otherBody)
                )
                {
                    APhysicsBodyClearAndSetState(body,      physics_body_state_sleeping, physics_body_state_collision);
                    APhysicsBodyClearAndSetState(otherBody, physics_body_state_sleeping, physics_body_state_collision);

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

        if (APhysicsBodyCheckState(body, physics_body_state_no_motion) == false)
        {
            // after test collision can update motion
            UpdateMotion(body, deltaSeconds);
        }
	}
}



static PhysicsBody* AddBody(PhysicsShape shape, Array(float)* vertexArr)
{
	PhysicsBody* body = APhysicsBody->Create(shape, vertexArr);
    AArrayIntSet->Add(bodySet, (intptr_t) body);

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
