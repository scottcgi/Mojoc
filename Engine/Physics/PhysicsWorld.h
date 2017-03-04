/*
 *
 *
 *  Created on: 2014-5-30
 *      Author: scott.cgi
 */

#ifndef physics_world_h
#define physics_world_h

#include "Mojoc/Toolkit/Define/Rect.h"
#include "Mojoc/Physics/PhysicsBody.h"
#include "Mojoc/Toolkit/Math/Vector.h"

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
	void         (*Update)     (float deltaTime);
};

extern struct APhysicsWorld APhysicsWorld[1];

#endif
