/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2014-6-3
 * UPdate: 2019-1-18
 * Author: scott.cgi
 */


#include <string.h>
#include "Engine/Physics/PhysicsBody.h"
#include "Engine/Toolkit/Platform/Log.h"


static inline PhysicsBody* CreateBody(Array(float)* vertexArr, PhysicsShape shape)
{
    int          size = sizeof(float) * vertexArr->length;
    PhysicsBody* body = (PhysicsBody*) malloc(sizeof(PhysicsBody) + (size << 1)); // NOLINT(hicpp-signed-bitwise)

    body->vertexArr->length   = vertexArr->length;
    body->vertexArr->data     = (char*) body + sizeof(PhysicsBody);
    memcpy(body->vertexArr->data, vertexArr->data, (size_t) size);

    body->positionArr->length = vertexArr->length;
    body->positionArr->data   = (char*) body->vertexArr->data + size;
    memcpy(body->positionArr->data, vertexArr->data, (size_t) size);

    
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


static void UpdateMotion(PhysicsBody* body, float deltaSeconds)
{
    float  cos       = AMath_Cos(body->rotationZ);
    float  sin       = AMath_Sin(body->rotationZ);
    float* positions = AArray_GetData(body->positionArr, float);
    float* vertices  = AArray_GetData(body->vertexArr,   float);

    for (int i = 0; i < body->positionArr->length; i += 2)
    {
        float x = vertices[i];
        float y = vertices[i + 1];

        positions[i]     = x * cos - y * sin + body->positionX;
        positions[i + 1] = x * sin + y * cos + body->positionY;
    }
}


struct APhysicsBody APhysicsBody[1] =
{
    Create,
    UpdateMotion,
};
