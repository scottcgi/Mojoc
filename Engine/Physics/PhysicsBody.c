/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License.
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub.
 * This Mojoc game engine is a serious project with extreme code pursuit, and will continue to iterate.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2014-6-3
 * Update : 2019-1-18
 * Author : scott.cgi
 */


#include <string.h>
#include "Engine/Physics/PhysicsBody.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Physics/PhysicsWorld.h"


static inline PhysicsBody* CreateBody(Array(float)* vertexArr, PhysicsShape shape)
{
    int          size = sizeof(float) * vertexArr->length;
    PhysicsBody* body = malloc(sizeof(PhysicsBody) + (size << 1)); // NOLINT(hicpp-signed-bitwise)

    body->vertexArr->length = vertexArr->length;
    body->vertexArr->data   = (char*) body + sizeof(PhysicsBody);
    memcpy(body->vertexArr->data, vertexArr->data, (size_t) size);

    body->transformedVertexArr->length = vertexArr->length;
    body->transformedVertexArr->data   = (char*) body->vertexArr->data + size;
    memcpy(body->transformedVertexArr->data, vertexArr->data, (size_t) size);

    AUserData_Init(body->userData);

    body->userId         = -1;
    body->positionX      = 0.0f;
    body->positionY      = 0.0f;
    body->velocityX      = 0.0f;
    body->velocityY      = 0.0f;
    body->accelerationX  = 0.0f;
    body->accelerationY  = 0.0f;
    body->rotationZ      = 0.0f;
    body->shape          = shape;
    body->state          = PhysicsBodyState_OutsideWorld;
    body->collisionGroup = 0;
    body->OnCollision    = NULL;

    return body;
}


static inline PhysicsBody* CreateWithPolygon(Array(float)* vertexArr)
{
    ALog_A(vertexArr->length >= 6, "PhysicsShape_Polygon vertexArr length must more than 6 (vertex contains x, y).");
    return CreateBody(vertexArr, PhysicsShape_Polygon);
}


static inline PhysicsBody* CreateWithLine(Array(float)* vertexArr)
{
    ALog_A(vertexArr->length == 4, "PhysicsShape_Line vertexArr length must equal 4 (vertex contains x, y).");
    return CreateBody(vertexArr, PhysicsShape_Line);
}


static inline PhysicsBody* CreateWithPoint(Array(float)* vertexArr)
{
    ALog_A(vertexArr->length == 2, "PhysicsShape_Point vertexArr length must equal 2 (vertex contains x, y).");
    return CreateBody(vertexArr, PhysicsShape_Point);
}


static PhysicsBody* Create(PhysicsShape shape, Array(float)* vertexArr)
{
    switch (shape)
    {
        case PhysicsShape_NULL:
            break;

        case PhysicsShape_Polygon:
            return CreateWithPolygon(vertexArr);

        case PhysicsShape_Line:
            return CreateWithLine(vertexArr);

        case PhysicsShape_Point:
            return CreateWithPoint(vertexArr);

        default:
            ALog_E("APhysicsBody Create with unknown shape = %d", shape);
            break;
    }

    return NULL;
}


static void ResetVertices(PhysicsBody* body)
{
    memcpy
    (
        body->transformedVertexArr->data,
        body->vertexArr->data,
        body->transformedVertexArr->length * sizeof(float)
    );
}


static void Update(PhysicsBody* body, float deltaSeconds)
{
    // cache v0
    float vx           = body->velocityX;
    float vy           = body->velocityY;

    // get final velocity in x and y direction
    // v1 = at + v0
    body->velocityX   += (body->accelerationX + APhysicsWorld->gravity.x) * deltaSeconds;
    body->velocityY   += (body->accelerationY + APhysicsWorld->gravity.y) * deltaSeconds;

    // get delta distance in x and y indirection
    // s = (v0 + v1) * t / 2
    float dx           = (body->velocityX + vx) * deltaSeconds * 0.5f;
    float dy           = (body->velocityY + vy) * deltaSeconds * 0.5f;

    // increase x and y distance
    body->positionX   += dx;
    body->positionY   += dy;
    body->rotationZ    = AMath_Atan2(dx, dy);
                      
    float  cosRZ       = AMath_Cos(body->rotationZ);
    float  sinRZ       = AMath_Sin(body->rotationZ);
    float* vertices    = body->vertexArr->data;
    float* transformed = body->transformedVertexArr->data;

    for (int i = 0; i < body->transformedVertexArr->length; i += 2)
    {
        float x = vertices[i];
        float y = vertices[i + 1];

        transformed[i]     = x * cosRZ - y * sinRZ + body->positionX;
        transformed[i + 1] = x * sinRZ + y * cosRZ + body->positionY;
    }

//  if (AMath_TestFloatEqual(body->velocityX, 0.0f) && AMath_TestFloatEqual(body->velocityY, 0.0f))
    {
        // stop motion
    }
}


struct APhysicsBody APhysicsBody[1] =
{
    Create,
    ResetVertices,
    Update,
};
