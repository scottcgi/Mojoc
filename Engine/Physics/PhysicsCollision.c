/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2014-7-23
 * Update: 2019-1-18
 * Author: scott.cgi
 */


#include "Engine/Physics/PhysicsCollision.h"
#include "Engine/Toolkit/Platform/Log.h"


/**
 * Test polygonA each vertex in polygonB, return true inside or false outside.
 * can test through and cross each others.
 */
static inline bool TestPolygonPolygonFull(Array(float)* polygonA, Array(float)* polygonB)
{
    int leftCount  = 0;
    int rightCount = 0;

    for (int i = 0; i < polygonA->length; i += 2)
    {
        float x        = AArray_Get(polygonA, i,     float);
        float y        = AArray_Get(polygonA, i + 1, float);

        int   preIndex = polygonB->length - 2;

        // test polygonB contains vertex
        for (int j = 0; j < polygonB->length; j += 2)
        {
            float vertexY = AArray_Get(polygonB, j        + 1, float);
            float preY    = AArray_Get(polygonB, preIndex + 1, float);

            // whether point on the y area of vector
            if ((vertexY < y && preY >= y) || (preY < y && vertexY >= y))
            {
                float vertexX = AArray_Get(polygonB, j, float);

                // cross product between vector (x - vertexX, y - vertexY) and (preX - vertexX, preY - vertexY)
                // result is (x - vertexX) * (preY - vertexY) - (y - vertexY) * (preX - vertexX)
                // if result zero means point (x, y) on the vector (preX - vertexX, preY - vertexY)
                // if result positive means point on the right of vector
                // if result negative means point on the left  of vector
                if (vertexX + (y - vertexY) / (preY - vertexY) * (AArray_Get(polygonB, preIndex, float) - vertexX) <= x)
                {
                    ++rightCount;
                }
                else
                {
                    ++leftCount;
                }
            }

            preIndex = j;
        }

        if (rightCount % 2 != 0)
        {
            return true;
        }
    }

    return rightCount != 0 && leftCount == rightCount;
}


/**
 * Test polygonA each vertex in polygonB,return true inside or false outside.
 * not test through and cross each others.
 */
static inline bool TestPolygonPolygon(Array(float)* polygonA, Array(float)* polygonB)
{
    bool inside = false;

    for (int i = 0; i < polygonA->length; i += 2)
    {
        float x        = AArray_Get(polygonA, i,     float);
        float y        = AArray_Get(polygonA, i + 1, float);
        int   preIndex = polygonB->length - 2;

        // test polygonB contains vertex
        for (int j = 0; j < polygonB->length; j += 2)
        {
            float vertexY = AArray_Get(polygonB, j        + 1, float);
            float preY    = AArray_Get(polygonB, preIndex + 1, float);

            // whether point on the y area of vector
            if ((vertexY < y && preY >= y) || (preY < y && vertexY >= y))
            {
                float vertexX = AArray_Get(polygonB, j, float);

                // cross product between vector (x - vertexX, y - vertexY) and (preX - vertexX, preY - vertexY)
                // result is (x - vertexX) * (preY - vertexY) - (y - vertexY) * (preX - vertexX)
                // if result zero means point (x, y) on the vector (preX - vertexX, preY - vertexY)
                // if result positive means point on the right of vector
                // if result negative means point on the left  of vector
                if (vertexX + (y - vertexY) / (preY - vertexY) * (AArray_Get(polygonB, preIndex, float) - vertexX) <= x)
                {
                    // point on the right
                    inside = !inside;
                }
            }

            preIndex = j;
        }

        if (inside)
        {
            return true;
        }
    }

    return inside;
}


/**
 * Test one lineA intersect lineB.
 */
static inline bool TestLineLine(Array(float)* lineA, Array(float)* lineB)
{
    int   flag[2]  = {0, 0};
    float vertexX1 = AArray_Get(lineB, 0, float);
    float vertexX2 = AArray_Get(lineB, 2, float);
    float vertexY1 = AArray_Get(lineB, 1, float);
    float vertexY2 = AArray_Get(lineB, 3, float);

    for (int i = 0; i < 4; i += 2)
    {
        float x = AArray_Get(lineA, i,     float);
        float y = AArray_Get(lineA, i + 1, float);

        // whether point on the y area of vector
        if ((vertexY1 < y && vertexY2 >= y) || (vertexY2 < y && vertexY1 >= y))
        {
            // cross product between vector (x - vertexX, y - vertexY) and (preX - vertexX, preY - vertexY)
            // result is (x - vertexX) * (preY - vertexY) - (y - vertexY) * (preX - vertexX)
            // if result zero means point (x, y) on the vector (preX - vertexX, preY - vertexY)
            // if result positive means point on the right of vector
            // if result negative means point on the left  of vector
            if (vertexX1 + (y - vertexY1) / (vertexY2 - vertexY1) * (vertexX2 - vertexX1) <= x)
            {
                // right
                flag[(unsigned int) i >> 1] = 1; // NOLINT(hicpp-signed-bitwise)
            }
            else
            {
                // left
                flag[(unsigned int) i >> 1] = 2; // NOLINT(hicpp-signed-bitwise)
            }
        }
    }

    // test lineA two points both sides of lineB
    if (flag[0] + flag[1] == 3)
    {
        return true;
    }


    flag[0]   = 0;
    flag[1]   = 0;

    vertexX1 = AArray_Get(lineA, 0, float);
    vertexX2 = AArray_Get(lineA, 2, float);
    vertexY1 = AArray_Get(lineA, 1, float);
    vertexY2 = AArray_Get(lineA, 3, float);

    for (int i = 0; i < 4; i += 2)
    {
        float x = AArray_Get(lineB, i,     float);
        float y = AArray_Get(lineB, i + 1, float);

        // whether point on the y area of vector
        if ((vertexY1 < y && vertexY2 >= y) || (vertexY2 < y && vertexY1 >= y))
        {
            // cross product between vector (x - vertexX, y - vertexY) and (preX - vertexX, preY - vertexY)
            // result is (x - vertexX) * (preY - vertexY) - (y - vertexY) * (preX - vertexX)
            // if result zero means point (x, y) on the vector (preX - vertexX, preY - vertexY)
            // if result positive means point on the right of vector
            // if result negative means point on the left  of vector
            if (vertexX1 + (y - vertexY1) / (vertexY2 - vertexY1) * (vertexX2 - vertexX1) <= x)
            {
                // right
                flag[(unsigned int) i >> 1] = 1; // NOLINT(hicpp-signed-bitwise)
            }
            else
            {
                // left
                flag[(unsigned int) i >> 1] = 2; // NOLINT(hicpp-signed-bitwise)
            }
        }
    }

    // test lineB two points both sides of lineA
    return flag[0] + flag[1] == 3;
}


/**
 * Test polygon contains one point, return true inside or false outside.
 */
static inline bool TestPolygonPoint(Array(float)* polygon, Array(float)* point)
{
    bool   inside     = false;
    int    preIndex   = polygon->length - 2;
    float* vertexData = AArray_GetData(polygon, float);
    float  x          = AArray_Get    (point,   0, float);
    float  y          = AArray_Get    (point,   1, float);

    for (int i = 0; i < polygon->length; i += 2)
    {
        float vertexY = vertexData[i        + 1];
        float preY    = vertexData[preIndex + 1];

        // whether point on the y area of vector
        if ((vertexY < y && preY >= y) || (preY < y && vertexY >= y))
        {
            float vertexX = vertexData[i];

            // cross product between vector (x - vertexX, y - vertexY) and (preX - vertexX, preY - vertexY)
            // result is (x - vertexX) * (preY - vertexY) - (y - vertexY) * (preX - vertexX)
            // if result zero means point (x, y) on the vector (preX - vertexX, preY - vertexY)
            // if result positive means point on the right of vector
            // if result negative means point on the left  of vector
            if (vertexX + (y - vertexY) / (preY - vertexY) * (vertexData[preIndex] - vertexX) <= x)
            {
                // point on the right
                inside = !inside;
            }
        }

        preIndex = i;
    }

    return inside;
}


//----------------------------------------------------------------------------------------------------------------------


typedef enum
{
    PhysicsShape_PolygonPolygon = PhysicsShape_Polygon,
    PhysicsShape_PolygonLine    = PhysicsShape_Polygon | PhysicsShape_Line,  // NOLINT(hicpp-signed-bitwise)
    PhysicsShape_LineLine       = PhysicsShape_Line,
    PhysicsShape_PolygonPoint   = PhysicsShape_Polygon | PhysicsShape_Point, // NOLINT(hicpp-signed-bitwise)
}
PhysicsShapeCollision;


static bool TestCollision(PhysicsBody* bodyA, PhysicsBody* bodyB)
{
    PhysicsShapeCollision shapeCollision = (PhysicsShapeCollision) (bodyA->shape | bodyB->shape);

    switch (shapeCollision)
    {
        case PhysicsShape_PolygonPolygon:
            return TestPolygonPolygon(bodyA->transformedVertexArr, bodyB->transformedVertexArr) ||
                   TestPolygonPolygon(bodyB->transformedVertexArr, bodyA->transformedVertexArr);

        case PhysicsShape_PolygonLine:
            // only consider line vertex in polygon
            if (bodyA->shape == PhysicsShape_Line)
            {
                return TestPolygonPolygonFull(bodyA->transformedVertexArr, bodyB->transformedVertexArr);
            }
            else
            {
                return TestPolygonPolygonFull(bodyB->transformedVertexArr, bodyA->transformedVertexArr);
            }

        case PhysicsShape_LineLine:
            return TestLineLine(bodyA->transformedVertexArr, bodyB->transformedVertexArr);

        case PhysicsShape_PolygonPoint:
            // only consider point in polygon
            if (bodyA->shape == PhysicsShape_Polygon)
            {
                return TestPolygonPoint(bodyA->transformedVertexArr, bodyB->transformedVertexArr);
            }
            else
            {
                return TestPolygonPoint(bodyB->transformedVertexArr, bodyA->transformedVertexArr);
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
{
    TestCollision,
};
