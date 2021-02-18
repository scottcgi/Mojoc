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
 * Since    : 2014-6-19
 * Update   : 2019-1-17
 * Author   : scott.cgi
 */


#ifndef VECTOR_H
#define VECTOR_H


#include <math.h>
#include "Engine/Toolkit/Math/Math.h"


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
 * Construct Vector2 composite literal.
 */
#define AVector2_Make(x, y) \
   (Vector2[1]) {{x, y}}


/**
 * Construct Vector3 composite literal.
 */
#define AVector3_Make(x, y, z) \
   (Vector3[1]) {{x, y, z}}


/**
* Construct Vector4 composite literal.
*/
#define AVector4_Make(x, y, z, w) \
   (Vector4[1]) {{x, y, z, w}}


/**
 * Calculate Vector2 length by vec2.
 */
static inline float AVector2_Length(Vector2* vec2)
{
    return AMath_Sqrtf(vec2->x * vec2->x + vec2->y * vec2->y);
}

/**
* Calculate Vector2 length by x and y.
*/
static inline float AVector2_Length2(float x, float y)
{
    return AMath_Sqrtf(x * x + y * y);
}


/**
 * Calculate Vector3 length by vec3.
 */
static inline float AVector3_Length(Vector3* vec3)
{
    return AMath_Sqrtf(vec3->x * vec3->x + vec3->y * vec3->y + vec3->z * vec3->z);
}


/**
 * Calculate Vector3 length by x y z.
 */
static inline float AVector3_Length3(float x, float y, float z)
{
    return AMath_Sqrtf(x * x + y * y + z * z);
}


/**
 * Normalize Vector3 by vec3.
 */
static inline float AVector3_Normalize(Vector3* vec3)
{
//  return 1.0f  /  sqrtf(vec3->x * vec3->x + vec3->y * vec3->y + vec3->z * vec3->z);
    return AMath_InvSqrtf(vec3->x * vec3->x + vec3->y * vec3->y + vec3->z * vec3->z);
}


/**
 * Normalize Vector3 by x y z.
 */
static inline float AVector3_Normalize3(float x, float y, float z)
{
//  return 1.0f  /  sqrtf(x * x + y * y + z * z);
    return AMath_InvSqrtf(x * x + y * y + z * z);
}


#endif 
