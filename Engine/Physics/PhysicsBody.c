/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2014-6-3
 * Author: scott.cgi
 */

#include <string.h>
#include "Engine/Physics/PhysicsBody.h"
#include "Engine/Toolkit/Platform/Log.h"


static inline void Init(PhysicsBody* body)
{
    AUserData_Init(body->userData);

	body->userId         =  -1;
	body->positionX      =  0.0f;
	body->positionY      =  0.0f;
	body->velocityX      =  0.0f;
	body->velocityY      =  0.0f;
	body->accelerationX  =  0.0f;
	body->accelerationY  =  0.0f;
	body->rotationZ      =  0.0f;
    body->shape          = -1;
    body->state          =  0;
    body->collisionGroup =  0;
    body->OnCollision    =  NULL;
}


static inline PhysicsBody* CreateWithPolygon(Array(float)* vertexArr)
{
	ALog_A(vertexArr->length >= 6, "physics_shape_polygon vertex length (each contains x, y) must more than 6");

	int          size         = sizeof(float) * vertexArr->length;
	PhysicsBody* body         = (PhysicsBody*) malloc(sizeof(PhysicsBody) + (size << 1));

	Init(body);

	body->shape               = physics_shape_polygon;
	body->vertexArr->length   = vertexArr->length;
	body->vertexArr->data     = (char*) body + sizeof(PhysicsBody);
	memcpy(body->vertexArr->data,  vertexArr->data, size);

	body->positionArr->length = vertexArr->length;
	body->positionArr->data   = (char*) body->vertexArr->data + size;
	memcpy(body->positionArr->data, vertexArr->data, size);

	return body;
}


static inline PhysicsBody* CreateWithLine(Array(float)* vertexArr)
{
	ALog_A(vertexArr->length == 4, "physics_shape_line vertex length must 4");

	int          size         = sizeof(float) * vertexArr->length;
	PhysicsBody* body         = (PhysicsBody*) malloc(sizeof(PhysicsBody) + (size << 1));

	Init(body);

	body->shape               = physics_shape_line;
	body->vertexArr->length   = vertexArr->length;
	body->vertexArr->data     = (char*) body + sizeof(PhysicsBody);
	memcpy(body->vertexArr->data,  vertexArr->data, size);


	body->positionArr->length = vertexArr->length;
	body->positionArr->data   = (char*) body->vertexArr->data + size;
	memcpy(body->positionArr->data, vertexArr->data, size);


	return body;
}


static PhysicsBody* Create(PhysicsShape shape, Array(float)* vertexArr)
{
	switch (shape)
	{
		case physics_shape_polygon:
			return CreateWithPolygon(vertexArr);

		case physics_shape_line:
			return CreateWithLine(vertexArr);

		default:
			ALog_A(false, "PhysicsBody create with unknown shape = %d", shape);
			break;
	}

	return NULL;
}


static void UpdateMotion2D(PhysicsBody* body, float deltaSeconds)
{
	float  cos       = AMathCos(body->rotationZ);
	float  sin       = AMathSin(body->rotationZ);
	float* positions = AArray_GetData(body->positionArr, float);
	float* vertices  = AArray_GetData(body->vertexArr,   float);

	for (int i = 0; i < body->positionArr->length; i += 2)
	{
		float x          = vertices[i];
		float y          = vertices[i + 1];

		positions[i]     = x * cos - y * sin + body->positionX;
		positions[i + 1] = x * sin + y * cos + body->positionY;
	}
}


struct APhysicsBody APhysicsBody[1] =
{
	Create,
	UpdateMotion2D,
};
