/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2014-6-19
 * Update: 2019-1-17
 * Author: scott.cgi
 */


#ifndef VECTOR_H
#define  VECTOR_H


#include <math.h>


typedef union
{
    float v[2];
    struct
    {
        float x;
        float y;
    };
}
Vector2;


typedef union
{
    float v[3];
    struct
    {
        float x;
        float y;
        float z;
    };
}
Vector3;


typedef union
{
    float v[4];
    struct
    {
        float x;
        float y;
        float z;
        float w;
    };
}
Vector4;


/**
 * Calculate Vector2 length by vec2.
 */
static inline float AVector2_Length(Vector2* vec2)
{
    return sqrtf(vec2->x * vec2->x + vec2->y * vec2->y);
}

/**
* Calculate Vector2 length by x and y.
*/
static inline float AVector2_Length2(float x, float y)
{
    return sqrtf(x * x + y * y);
}


/**
 * Calculate Vector3 length by vec3.
 */
static inline float AVector3_Length(Vector3* vec3)
{
    return sqrtf(vec3->x * vec3->x + vec3->y * vec3->y + vec3->z * vec3->z);
}


/**
 * Calculate Vector3 length by x y z.
 */
static inline float AVector3_Length3(float x, float y, float z)
{
    return sqrtf(x * x + y * y + z * z);
}


/**
 * Normalize Vector3 by vec3.
 */
static inline float AVector3_Normalize(Vector3* vec3)
{
    return 1.0f / sqrtf(vec3->x * vec3->x + vec3->y * vec3->y + vec3->z * vec3->z);
}


/**
 * Normalize Vector3 by x y z.
 */
static inline float AVector3_Normalize3(float x, float y, float z)
{
    return 1.0f / sqrtf(x * x + y * y + z * z);
}


#endif 
