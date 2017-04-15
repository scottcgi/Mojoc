/*
 *
 *
 *  Created on: 2014-5-30
 *      Author: scott.cgi
 */


#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Toolkit/Utils/ArrayIntMap.h"
#include "Engine/Physics/PhysicsCollision.h"


static ArrayIntMap(bodyPtr, PhysicsBody*) bodyMap[1] = AArrayIntMapInit(PhysicsBody*, 20);


//--------------------------------------------------------------------------------------------------


static inline void UpdateMotion(PhysicsBody* body, float deltaTime)
{
	// cache v0
	float vx         = body->velocityX;
	float vy         = body->velocityY;

	// get final velocity in x and y direction
	// v1 = at + v0
	body->velocityX += (body->accelerationX + APhysicsWorld->gravity.x) * deltaTime;
	body->velocityY += (body->accelerationY + APhysicsWorld->gravity.y) * deltaTime;

	// get delta distance in x and y indirection
	// s = (v0 + v1) * t / 2
	float dx         = (body->velocityX + vx) * deltaTime * 0.5f;
	float dy         = (body->velocityY + vy) * deltaTime * 0.5f;

	// increase x and y distance
	body->positionX += dx;
	body->positionY += dy;

	body->rotationZ  = AMathAtan2(dx, dy);

	APhysicsBody->UpdateMotion(body, deltaTime);

	if (AMathTestFloatEqual(body->velocityX, 0.0f) && AMathTestFloatEqual(body->velocityY, 0.0f))
	{
		APhysicsBodySetState(body, physics_body_state_sleeping);
	}
}


static void Update(float deltaTime)
{
	for (int i = 0; i < bodyMap->arrayList->size; i++)
	{
		PhysicsBody* body = AArrayIntMapGetAt(bodyMap, i, PhysicsBody*);

        if (APhysicsBodyCheckState(body, physics_body_state_no_collision) == false)
        {
            // test collision
            for (int fromIndex = i + 1; fromIndex < bodyMap->arrayList->size; fromIndex++)
            {
                PhysicsBody* otherBody = AArrayIntMapGetAt(bodyMap, fromIndex, PhysicsBody*);

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
                        body->OnCollision(body, otherBody, deltaTime);
                    }

                    if (otherBody->OnCollision != NULL)
                    {
                        otherBody->OnCollision(otherBody, body, deltaTime);
                    }
                }
            }
        }

        if (APhysicsBodyCheckState(body, physics_body_state_no_motion) == false)
        {
            // after test collision can update motion
            UpdateMotion(body, deltaTime);
        }
	}
}



static PhysicsBody* AddBody(PhysicsShape shape, Array(float)* vertexArr)
{
	PhysicsBody* body = APhysicsBody->Create(shape, vertexArr);
	AArrayIntMapPut(bodyMap, body, body);

	return body;
}


static void DestroyBody(PhysicsBody* body)
{
    AArrayIntMap->TryRemove(bodyMap, (intptr_t) body);
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
