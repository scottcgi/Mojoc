/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-1-24
 * Author: scott.cgi
 */

#ifndef MATH_H
#define MATH_H


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


//----------------------------------------------------------------------------------------------------------------------


#define GOLDEN_RATIO     0.618033988749894


/**
 * PI / 180.0
 */
#define DEGREE_TO_RADIAN 0.017453292519943


/**
 * 180.0f / PI
 */
#define RADIAN_TO_DEGREE 57.29577951308232


#define MATH_PI          3.141592653589793


#define MATH_2PI         6.283185307179586


#define MATH_PI2         1.570796326794897


/**
 * PI / 360.0f
 */
#define MATH_PI360       0.008726646259972


//----------------------------------------------------------------------------------------------------------------------


/**
 * Random float in range [0.0, 1.0]
 */
static inline float AMath_Random()
{
    return rand() / (double) RAND_MAX;
}


/**
 * Random integer in range [from, to]
 */
static inline int AMath_RandomInt(int from, int to)
{
    return (from) + rand() % ((to) - (from) + 1);
}


/**
 * Random float in range [from, to]
 */
static inline float AMath_RandomFloat(float from, float to)
{
    return from + AMath_Random() * (to - from);
}


/**
 * Random seed by system time
 */
static inline void AMath_RandomSeedByTime()
{
    srand((unsigned) time(NULL));
}


/**
 * Convert degree to radian
 */
static inline float AMath_ToRadian(float degree)
{
    return degree * DEGREE_TO_RADIAN;
}


/**
 * Convert radian to degree
 */
static inline float AMath_ToDegree(float radian)
{
    return radian * RADIAN_TO_DEGREE;
}


/**
 * Cos by degree
 */
static inline float AMath_Cos(float degree)
{
    return cosf(AMath_ToRadian(degree));
}


/**
 * Sin by degree
 */
static inline float AMath_Sin(float degree)
{
    return sinf(AMath_ToRadian(degree));
}


/**
 * Degree by atan2
 */
static inline float AMath_Atan2(float x, float y)
{
    return AMath_ToDegree(atan2f(y, x));
}


/**
 * Degree by acosf
 */
static inline float AMath_Acos(float ratio)
{
    return AMath_ToDegree(acosf(ratio));
}


/**
 * Degree by asinf
 */
static inline float AMath_Asin(float ratio)
{
    return AMath_ToDegree(asinf(ratio));
}


/**
 * Min in x and y, macro can use generic parameter
 */
#define AMath_Min(x, y) \
    (((x) < (y)) ? (x) : (y))


/**
 * Max in a and b, macro can use generic parameter
 */
#define AMath_Max(x, y) \
    (((x) > (y)) ? (x) : (y))


/**
 * Clamp x in min and max, macro can use generic parameter
 */
#define AMath_Clamp(x, min, max) \
    (AMath_Min((max), AMath_Max((x), (min))))


/**
 * Compare float value equals
 */
static inline bool AMath_TestFloatEqual(float x, float y)
{
    return fabsf((x) - (y)) <= FLT_EPSILON;
}


/**
 * Get x sign bit only for little-endian
 * if x >= 0 then  1
 * if x <  0 then -1
 * macro can use generic parameter
 */
#define AMath_SignBit(x) \
    (((signed char*) &(x))[sizeof(x) - 1] >> 7 | 1)


#endif