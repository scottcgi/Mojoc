/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2013-1-24
 * Update   : 2019-1-18
 * Author   : scott.cgi
 */


#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Math/Math.h"


/**
 * Test polygon contains 2D point(x, y).
 *
 * first  — foreach vector(preX - curX, preY - curY) of polygon
 * second — check the point's y on the y area of vector
 * third  — cross product between vector(x - curX, y - curY) and vector(preX - curX, preY - curY)
 * fourth — get the result is (x - curX) * (preY - curY) - (y - curY) * (preX - curX) = 0
 * then   —
 * 
 * if result zero     means point on the vector
 * if result positive means point on the right of vector
 * if result negative means point on the left  of vector
 */
#define TestPolygonXY(polygon, x, y, pointOnRight, pointOnLeft)                     \
    float* points   = polygon->data;                                                \
    int    preIndex = polygon->length - 2;                                          \
                                                                                    \
    for (int i = 0; i < polygon->length; i += 2)                                    \
    {                                                                               \
        float curY  = points[i        + 1];                                         \
        float preY  = points[preIndex + 1];                                         \
                                                                                    \
        if ((curY < y && preY >= y) || (preY < y && curY >= y))                     \
        {                                                                           \
            float curX = points[i];                                                 \
                                                                                    \
            if (curX + (y - curY) / (preY - curY) * (points[preIndex] - curX) <= x) \
            {                                                                       \
                pointOnRight;                                                       \
            }                                                                       \
            else                                                                    \
            {                                                                       \
                pointOnLeft;                                                        \
            }                                                                       \
        }                                                                           \
                                                                                    \
        preIndex = i;                                                               \
    }                                                                               \




static bool TestPolygonPoint(Array(float)* polygon, float x, float y)
{
    bool inside = false;

    TestPolygonXY
    (
        polygon,
        x,
        y,
        // point on the right of polygon vector
        inside = !inside,
    );

    return inside;
}


static bool TestPolygonAB(Array(float)* polygonA, Array(float)* polygonB)
{
    bool inside = false;
    
    for (int i = 0; i < polygonA->length; i += 2)
    {
        float x = AArray_Get(polygonA, i,     float);
        float y = AArray_Get(polygonA, i + 1, float);

        TestPolygonXY
        (
            polygonB,
            x,
            y,
            // point on the right of polygon vector
            inside = !inside,
        );

        if (inside)
        {
            return true;
        }
    }

    return inside;
}


static bool TestPolygonPolygon(Array(float)* polygonA, Array(float)* polygonB)
{
    return TestPolygonAB(polygonA, polygonB) || TestPolygonAB(polygonB, polygonA);
}


static bool TestPolygonABStrict(Array(float)* polygonA, Array(float)* polygonB)
{
    int leftCount  = 0;
    int rightCount = 0;

    for (int i = 0; i < polygonA->length; i += 2)
    {
        float x = AArray_Get(polygonA, i,     float);
        float y = AArray_Get(polygonA, i + 1, float);

        TestPolygonXY
        (
            polygonB,
            x,
            y,
            // count point on the right of polygon vector
            ++rightCount,
            // count point on the left  of polygon vector
            ++leftCount
        );

        if (rightCount % 2 != 0)
        {
            return true;
        }
    }

    return rightCount != 0 && leftCount == rightCount;
}


static bool TestPolygonPolygonStrict(Array(float)* polygonA, Array(float)* polygonB)
{
    return TestPolygonABStrict(polygonA, polygonB) || TestPolygonABStrict(polygonB, polygonA);
}


static bool TestLineAB(Array(float)* lineA, Array(float)* lineB)
{
    bool flags[2]  = {false, false};

    for (int i = 0; i < lineA->length; i += 2)
    {
        float x = AArray_Get(lineA, i,     float);
        float y = AArray_Get(lineA, i + 1, float);

        TestPolygonXY
        (
            lineB,
            x,
            y,
            // flag point on the right of line vector
            flags[(unsigned int) i >> 1] = true,
            // flag point on the left  of line vector
            flags[(unsigned int) i >> 1] = true
        );
    }

    // test lineA two points both sides of lineB
    return flags[0] && flags[1];
}


static bool TestLineLine(Array(float)* lineA, Array(float)* lineB)
{
    return TestLineAB(lineA, lineB) || TestLineAB(lineB, lineA);
}


static void RotatePoints(Array(float)* pointArr, float angle, Array(float)* outRotatedPointArr)
{
    ALog_A
    (
        outRotatedPointArr->length >= pointArr->length,
        "AMath RotatePoints2 outRotatedPointArr length must larger than pointArr."
    );

    float  cos  = AMath_Cos(angle);
    float  sin  = AMath_Sin(angle);
    float* arr1 = (float*) pointArr->data;
    float* arr2 = (float*) outRotatedPointArr->data;

    for (int i = 0; i < pointArr->length; i += 2)
    {
        float x = arr1[i];
        float y = arr1[i + 1];

        arr2[i]     = x * cos - y * sin;
        arr2[i + 1] = x * sin + y * cos;
    }
}


struct AMath AMath[1] =
{{
    TestPolygonPoint,
    TestPolygonAB,
    TestPolygonPolygon,
    TestPolygonABStrict,
    TestPolygonPolygonStrict,
    TestLineAB,
    TestLineLine,
    RotatePoints,
}};
