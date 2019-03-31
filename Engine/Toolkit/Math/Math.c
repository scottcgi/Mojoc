/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The Mojoc game engine is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2013-1-24
 * Update: 2019-1-18
 * Author: scott.cgi
 */


#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Math/Math.h"


static bool TestPolygonPoint(Array(float)* pointArr, float x, float y)
{
    bool   inside   = false;
    int    preIndex = pointArr->length - 2;
    float* points   = pointArr->data;

    for (int i = 0; i < pointArr->length; i += 2)
    {
        float pointY  = points[i + 1];
        float preY    = points[preIndex + 1];

        // whether point on the y area of vector
        if ((pointY < y && preY >= y) || (preY < y && pointY >= y))
        {
            float pointX = points[i];

            // cross product between vector (x - pointX, y - pointY) and (preX - pointX, preY - pointY)
            // result is (x - pointX) * (preY - pointY) - (y - pointY) * (preX - pointX)
            // if result zero means point (x, y) on the vector (preX - pointX, preY - pointY)
            // if result positive means point on the right of vector
            // if result negative means point on the left  of vector
            if (pointX + (y - pointY) / (preY - pointY) * (points[preIndex] - pointX) <= x)
            {
                // point on the right
                inside = !inside;
            }
        }

        preIndex = i;
    }

    return inside;
}


static void RotatePoints2(Array(float)* pointArr, float angle, Array(float)* outRotatedPointArr)
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
{
    TestPolygonPoint,
    RotatePoints2,
};
