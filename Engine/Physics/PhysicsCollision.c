/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2014-7-23
 * Author : scott.cgi
 * Version: 0.1
 */


#include "Engine/Physics/PhysicsCollision.h"
#include "Engine/Toolkit/Platform/Log.h"


/**
 * Test polygonA each vertex in polygonB, true inside or false outside
 * can test through and cross each others
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

            if ((vertexY < y && preY >= y) || (preY < y && vertexY >= y))
            {
                float vertexX = AArray_Get(polygonB, j, float);

                // cross product between vector (x - vertexX, y - vertexY) and (preX - vertexX, preY - vertexY)
                // result is (x - vertexX) * (preY - vertexY) - (y - vertexY) * (preX - vertexX)
                // if result zero means point (x, y) on vector (preX - vertexX, preY - vertexY)
                // if result positive means point on left  vector
                // if result negative means point on right vector
                if (vertexX + (y - vertexY) / (preY - vertexY) * (AArray_Get(polygonB, preIndex, float) - vertexX) <= x)
                {
                    leftCount++;
                }
                else
                {
                    rightCount++;
                }
            }

            preIndex = j;
        }

        if (leftCount % 2 != 0)
        {
            return true;
        }
    }

    return leftCount != 0 && leftCount == rightCount;
}


/**
 * Test polygonA each vertex in polygonB, true inside or false outside
 * not test through and cross each others
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

            if ((vertexY < y && preY >= y) || (preY < y && vertexY >= y))
            {
                float vertexX = AArray_Get(polygonB, j, float);

                // cross product between vector (x - vertexX, y - vertexY) and (preX - vertexX, preY - vertexY)
                // result is (x - vertexX) * (preY - vertexY) - (y - vertexY) * (preX - vertexX)
                // if result zero means point (x, y) on vector (preX - vertexX, preY - vertexY)
                // if result positive means point on left  vector
                // if result negative means point on right vector
                if (vertexX + (y - vertexY) / (preY - vertexY) * (AArray_Get(polygonB, preIndex, float) - vertexX) <= x)
                {
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
 * Test one lineA intersect lineB
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

        if ((vertexY1 < y && vertexY2 >= y) || (vertexY2 < y && vertexY1 >= y))
        {
            // cross product between vector (x - vertexX1, y - vertexY1) and (vertexX2 - vertexX1, vertexY2 - vertexY1)
            // result is (x - vertexX1) * (vertexY2 - vertexY1) - (y - vertexY1) * (vertexX2 - vertexX1)
            if (vertexX1 + (y - vertexY1) / (vertexY2 - vertexY1) * (vertexX2 - vertexX1) <= x)
            {
                flag[i >> 1] = 1;
            }
            else
            {
                flag[i >> 1] = 2;
            }
        }
    }

    // test lineA two points both sides of lineB
    if (flag[0] + flag[1] == 3)
    {
        return true;
    }


    flag[0]  = 0;
    flag[1]  = 0;

    vertexX1 = AArray_Get(lineA, 0, float);
    vertexX2 = AArray_Get(lineA, 2, float);
    vertexY1 = AArray_Get(lineA, 1, float);
    vertexY2 = AArray_Get(lineA, 3, float);

    for (int i = 0; i < 4; i += 2)
    {
        float x = AArray_Get(lineB, i,     float);
        float y = AArray_Get(lineB, i + 1, float);

        if ((vertexY1 < y && vertexY2 >= y) || (vertexY2 < y && vertexY1 >= y))
        {
            // cross product between vector (x - vertexX1, y - vertexY1) and (vertexX2 - vertexX1, vertexY2 - vertexY1)
            // result is (x - vertexX1) * (vertexY2 - vertexY1) - (y - vertexY1) * (vertexX2 - vertexX1)
            if (vertexX1 + (y - vertexY1) / (vertexY2 - vertexY1) * (vertexX2 - vertexX1) <= x)
            {
                flag[i >> 1] = 1;
            }
            else
            {
                flag[i >> 1] = 2;
            }
        }
    }

    // test lineB two points both sides of lineA
    return flag[0] + flag[1] == 3;
}


/**
 * Test polygon contains point, true inside or false outside
 */
static inline bool TestPolygonPoint(Array(float)* polygon, Array(float)* point)
{
    bool   inside     = false;
    int    preIndex   = polygon->length - 2;
    float* vertexData = AArray_GetData(polygon, float);
    float  x          = AArray_Get(polygon, 0,  float);
    float  y          = AArray_Get(polygon, 1,  float);

    for (int i = 0; i < polygon->length; i += 2)
    {
        float vertexY = vertexData[i        + 1];
        float preY    = vertexData[preIndex + 1];

        if ((vertexY < y && preY >= y) || (preY < y && vertexY >= y))
        {
            float vertexX = vertexData[i];

            // cross product between vector (x - vertexX, y - vertexY) and (preX - vertexX, preY - vertexY)
            // result is (x - vertexX) * (preY - vertexY) - (y - vertexY) * (preX - vertexX)
            // if result zero means point (x, y) on vector (preX - vertexX, preY - vertexY)
            // if result positive means point on left  vector
            // if result negative means point on right vector
            if (vertexX + (y - vertexY) / (preY - vertexY) * (vertexData[preIndex] - vertexX) <= x)
            {
                inside = !inside;
            }
        }

        preIndex = i;
    }

    return inside;
}


//----------------------------------------------------------------------------------------------------------------------


enum
{
    PhysicsShape_PolygonPloygon = PhysicsShape_Polygon | PhysicsShape_Polygon,
    PhysicsShape_PloygonLine    = PhysicsShape_Polygon | PhysicsShape_Line,
    PhysicsShape_LineLine       = PhysicsShape_Line    | PhysicsShape_Line,
    PhysicsShape_PloygonPoint   = PhysicsShape_Polygon | PhysicsShape_Point,
};


static bool TestCollision(PhysicsBody* bodyA, PhysicsBody* bodyB)
{
    switch (bodyA->shape | bodyB->shape)
    {
        case PhysicsShape_PolygonPloygon:
            return TestPolygonPolygon(bodyA->positionArr, bodyB->positionArr) || TestPolygonPolygon(bodyB->positionArr, bodyA->positionArr);

        case PhysicsShape_PloygonLine:
            // only consider line vertex in polygon
            if (bodyA->shape == PhysicsShape_Line)
            {
                return TestPolygonPolygonFull(bodyA->positionArr, bodyB->positionArr);
            }
            else
            {
                return TestPolygonPolygonFull(bodyB->positionArr, bodyA->positionArr);
            }

        case PhysicsShape_LineLine:
            return TestLineLine(bodyA->positionArr, bodyB->positionArr);

        case PhysicsShape_PloygonPoint:
            // only consider point in polygon
            if (bodyA->shape == PhysicsShape_Polygon)
            {
                return TestPolygonPoint(bodyA->positionArr, bodyB->positionArr);
            }
            else
            {
                return TestPolygonPoint(bodyB->positionArr, bodyA->positionArr);
            }
    }

    ALog_A(false, "APhysicsCollision can not test collision between shape %d and %d", bodyA->shape, bodyB->shape);

    return false;
}


struct APhysicsCollision APhysicsCollision[1] =
{
    TestCollision,
};
