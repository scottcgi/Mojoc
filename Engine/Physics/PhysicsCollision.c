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
 * Since    : 2014-7-23
 * Update   : 2019-1-18
 * Author   : scott.cgi
 */


#include "Engine/Physics/PhysicsCollision.h"


typedef enum
{
    PhysicsShape_PolygonPolygon = PhysicsShape_Polygon,
    PhysicsShape_PolygonLine    = PhysicsShape_Polygon | PhysicsShape_Line,
    PhysicsShape_LineLine       = PhysicsShape_Line,
    PhysicsShape_PolygonPoint   = PhysicsShape_Polygon | PhysicsShape_Point,
}
PhysicsShapeCollision;


static bool TestCollision(PhysicsBody* bodyA, PhysicsBody* bodyB)
{
    PhysicsShapeCollision shapeCollision = (PhysicsShapeCollision) (bodyA->shape | bodyB->shape);

    switch (shapeCollision)
    {
        case PhysicsShape_PolygonPolygon:
            return AMath->TestPolygonPolygon(bodyA->transformedVertexArr, bodyB->transformedVertexArr);

        case PhysicsShape_PolygonLine:
            // only consider line vertex in polygon
            if (bodyA->shape == PhysicsShape_Line)
            {
                return AMath->TestPolygonABStrict(bodyA->transformedVertexArr, bodyB->transformedVertexArr);
            }
            else
            {
                return AMath->TestPolygonABStrict(bodyB->transformedVertexArr, bodyA->transformedVertexArr);
            }

        case PhysicsShape_LineLine:
            return AMath->TestLineLine(bodyA->transformedVertexArr, bodyB->transformedVertexArr);

        case PhysicsShape_PolygonPoint:
            // only consider point in polygon
            if (bodyA->shape == PhysicsShape_Polygon)
            {
                return AMath->TestPolygonPoint
                       (
                           bodyA->transformedVertexArr,
                           AArray_Get(bodyB->transformedVertexArr, 0, float),
                           AArray_Get(bodyB->transformedVertexArr, 1, float)
                       );
            }
            else
            {
                return AMath->TestPolygonPoint
                        (
                            bodyB->transformedVertexArr,
                            AArray_Get(bodyA->transformedVertexArr, 0, float),
                            AArray_Get(bodyA->transformedVertexArr, 1, float)
                        );
            }
        default:
            ALog_A
            (
                false,
                "APhysicsCollision cannot test collision between shape %d and %d",
                bodyA->shape,
                bodyB->shape
            );
    }

    return false;
}


struct APhysicsCollision APhysicsCollision[1] =
{{
    TestCollision,
}};
