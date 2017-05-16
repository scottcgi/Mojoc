/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-1-24
 * Author : scott.cgi
 * Version: 0.1
 */


#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Math/Math.h"


static bool TestPolygonPoint(Array(float)* vertexArr, float x, float y)
{
	int    preIndex   = vertexArr->length - 2;
	bool   inside     = false;
	float* vertexData = AArray_GetData(vertexArr, float);

	for (int i = 0; i < vertexArr->length; i += 2)
	{
		float vertexY = vertexData[i + 1];
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


static void RotatePoints2(Array(float)* pointArr, float angle, Array(float)* outRotatedPointArr)
{
	ALog_A(outRotatedPointArr->length >= pointArr->length, "outRotatedPointArr length must larger than pointArr");

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
