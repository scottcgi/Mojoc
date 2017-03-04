/*
 *
 *
 *  Created on: 2014-7-23
 *      Author: scott.cgi
 */

#ifndef physics_collision_h
#define physics_collision_h

#include <stdbool.h>
#include "Mojoc/Physics/PhysicsBody.h"


struct APhysicsCollision
{
	bool (*TestCollision)(PhysicsBody* bodyA, PhysicsBody* bodyB);
};

extern struct APhysicsCollision APhysicsCollision[1];

#endif 
