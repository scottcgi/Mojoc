/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2014-6-3
 * Author: scott.cgi
 */

#ifndef physics_body_h
#define physics_body_h


#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Head/Bitwise.h"
#include "Engine/Toolkit/Utils/Array.h"
#include "Engine/Toolkit/Head/UserData.h"


typedef enum
{
	physics_shape_polygon = 1,
	physics_shape_line    = 1 << 2,
	physics_shape_point   = 1 << 3,
	physics_shape_length,
}
PhysicsShape;


/**
 * If contains 'is' the state can set and clear
 * else the state will automatically set and clear
 */
typedef enum
{
	physics_body_state_null           = 0,

	/**
	 * Auto set no motion can collision, collision will wake up
	 */
	physics_body_state_sleeping       = 1,

	/**
	 * Auto set whether has collision
	 */
	physics_body_state_collision      = 1 << 1,

//--------------------------------------------------------------------------------------------------

	/**
	 * No motion can collision
	 */
	physics_body_state_is_fixed       = 1 << 2,

	/**
	 * No motion no collision forever
	 */
	physics_body_state_is_freeze      = 1 << 3,


//--------------------------------------------------------------------------------------------------


	physics_body_state_no_motion      = physics_body_state_sleeping  |
								        physics_body_state_is_fixed  |
								        physics_body_state_is_freeze,

   physics_body_state_no_collision    = physics_body_state_is_freeze,

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

    /**
     *
     */
	PhysicsShape shape;

    /**
     * Hold born vertices, not modify
     */
	Array(float) vertexArr   [1];

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


static inline bool APhysicsBodyCheckState(PhysicsBody* physicsBody, PhysicsBodyState checkState)
{
	return ABitwise_Check(physicsBody->state, checkState);
}


static inline void APhysicsBodySetState(PhysicsBody* physicsBody, PhysicsBodyState setState)
{
	ABitwise_Set(physicsBody->state, setState);
}


static inline void APhysicsBodySetOnlyState(PhysicsBody* physicsBody, PhysicsBodyState setOnlyState)
{
	ABitwise_SetOnly(physicsBody->state, setOnlyState);
}


static inline void APhysicsBodyClearState(PhysicsBody* physicsBody, PhysicsBodyState clearState)
{
	ABitwise_Clear(physicsBody->state, clearState);
}


static inline void APhysicsBodyClearAndSetState(PhysicsBody* physicsBody, PhysicsBodyState clearState, PhysicsBodyState setState)
{
	ABitwise_ClearAndSet(physicsBody->state, clearState, setState);
}


//--------------------------------------------------------------------------------------------------


static inline bool APhysicsBodyCheckCollisionGroup(PhysicsBody* physicsBody,  int checkCollisionGroup)
{
	return ABitwise_Check(physicsBody->collisionGroup, checkCollisionGroup);
}


static inline void APhysicsBodySetCollisionGroup(PhysicsBody* physicsBody,    int setCollisionGroup)
{
	ABitwise_Set(physicsBody->collisionGroup, setCollisionGroup);
}


static inline void APhysicsBodySetOnlyCollisionGroup(PhysicsBody* physicsBody, int setOnlycollisionGroup)
{
	ABitwise_SetOnly(physicsBody->collisionGroup, setOnlycollisionGroup);
}


static inline void APhysicsBodyClearCollisionGroup(PhysicsBody* physicsBody,   int clearCollisionGroup)
{
	ABitwise_Clear(physicsBody->collisionGroup, clearCollisionGroup);
}


#endif
