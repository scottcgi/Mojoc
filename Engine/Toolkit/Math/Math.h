/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2013-1-24
 * Author : scott.cgi
 */

#ifndef math_h
#define math_h


#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <stdbool.h>
#include "Engine/Toolkit/Utils/Array.h"


struct AMath
{
	/**
	 * Test polygon contains point, true inside or false outside
	 * one vertex contains pair of x, y
	 */
	bool  (*TestPolygonPoint)(Array(float)* vertexArr, float x, float y);

	/**
	 * Rotate 2D point pair of x, y by angle
	 */
	void  (*RotatePoints2)   (Array(float)* pointArr, float angle, Array(float)* outRotatedPointArr);
};


extern struct AMath AMath[1];


//--------------------------------------------------------------------------------------------------


#define golden_ratio     0.618033988749894


/**
 * PI / 180.0
 */
#define degree_to_radian 0.017453292519943


/**
 * 180.0f / PI
 */
#define radian_to_degree 57.29577951308232


#define	math_pi		     3.141592653589793


#define math_2pi         6.283185307179586


#define math_pi2         1.570796326794897


//--------------------------------------------------------------------------------------------------


/**
 * Random float in range [0.0, 1.0]
 */
static inline float AMathRandom()
{
	return rand() / (double) RAND_MAX;
}


/**
 * Random integer in range [from, to]
 */
static inline int AMathRandomInt(int from, int to)
{
	return (from) + rand() % ((to) - (from) + 1);
}


/**
 * Random float in range [from, to]
 */
static inline float AMathRandomFloat(float from, float to)
{
	return from + AMathRandom() * (to - from);
}


/**
 * Random seed by system time
 */
static inline void AMathRandomSeedByTime()
{
	srand((unsigned) time(NULL));
}


/**
 * Convert degree to radian
 */
static inline float AMathToRadian(float degree)
{
	return (degree) * degree_to_radian;
}


/**
 * Convert radian to degree
 */
static inline float AMathToDegree(float radian)
{
	return (radian) * radian_to_degree;
}


/**
 * Cos by degree
 */
static inline float AMathCos(float degree)
{
	return cosf(AMathToRadian(degree));
}


/**
 * Sin by degree
 */
static inline float AMathSin(float degree)
{
	return sinf(AMathToRadian(degree));
}


/**
 * Degree by atan2
 */
static inline float AMathAtan2(float x, float y)
{
	return 	AMathToDegree(atan2f(y, x));
}


/**
 * Degree by acosf
 */
static inline float AMathAcos(float ratio)
{
	return AMathToDegree(acosf(ratio));
}


/**
 * Degree by asinf
 */
static inline float AMathAsin(float ratio)
{
	return AMathToDegree(asinf(ratio));
}


/**
 * Min in x and y, macro can use generic parameter
 */
#define AMathMin(x, y) \
	(((x) < (y)) ? (x) : (y))


/**
 * Max in a and b, macro can use generic parameter
 */
#define AMathMax(x, y) \
	(((x) > (y)) ? (x) : (y))


/**
 * Clamp x in min and max, macro can use generic parameter
 */
#define AMathClamp(x, min, max) \
	(AMathMin((max), AMathMax((x), (min))))


/**
 * Compare float value equals
 */
static inline bool AMathTestFloatEqual(float x, float y)
{
	return fabsf((x) - (y)) <= FLT_EPSILON;
}


/**
 * Get x sign bit only for little-endian
 * if x >= 0 then  1
 * if x <  0 then -1
 * macro can use generic parameter
 */
#define AMathSignBit(x) \
	(((signed char*) &(x))[sizeof(x) - 1] >> 7 | 1)


#endif