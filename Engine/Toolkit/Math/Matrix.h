/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License.
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub.
 * This Mojoc game engine is a serious project with extreme code pursuit, and will continue to iterate.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2013-1-6
 * Update : 2019-1-17
 * Author : scott.cgi
 */


#ifndef MATRIX_H
#define MATRIX_H


#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Math/Vector.h"


typedef union
{
    float m[16];
    struct
    {
        float
        m0,  m1,  m2,  m3,
        m4,  m5,  m6,  m7,
        m8,  m9,  m10, m11,
        m12, m13, m14, m15;
    };
}
Matrix4;


#define MATRIX4_IDENTITY          \
    {{                            \
          1.0f, 0.0f, 0.0f, 0.0f, \
          0.0f, 1.0f, 0.0f, 0.0f, \
          0.0f, 0.0f, 1.0f, 0.0f, \
          0.0f, 0.0f, 0.0f, 1.0f, \
    }}

#define MATRIX4_IDENTITY_ARRAY \
    (Matrix4[1]) MATRIX4_IDENTITY


/**
 * Control Matrix4.
 */
struct AMatrix
{
    /**
     * Multiply left and right matrix4 into outMatrix4.
     */
    void  (*MultiplyMM)      (Matrix4* left, Matrix4* right, Matrix4* outMatrix4);

    /**
     * Multiply m2 and m3  into outM23.
     * Multiply m1 and m23 into outM231.
     */
    void  (*MultiplyMMM)     (Matrix4* m1, Matrix4* m2, Matrix4* m3, Matrix4* outM23, Matrix4* outM231);

    /**
     * Multiply matrix4 * vector4 into outVector4.
     */
    void  (*MultiplyMV4)     (Matrix4* matrix4, float x, float y, float z, float w, Vector4* outVector4);

    /**
     * Multiply matrix4 * vector3 into outVector3.
     */
    void  (*MultiplyMV3)     (Matrix4* matrix4, float x, float y, float z, Vector3* outVector3);

    /**
     * Multiply matrix4 * vector2 into outVector2.
     */
    void  (*MultiplyMV2)     (Matrix4* matrix4, float x, float y, Vector2* outVector2);

    /**
     * Multiply matrix4 * x, return transformed x.
     */
    float (*MultiplyMX)      (Matrix4* matrix4, float x);

    /**
     * Multiply matrix4 * y, return transformed y.
     */
    float (*MultiplyMY)      (Matrix4* matrix4, float y);

    /**
     * Multiply matrix4 * z, return transformed z.
     */
    float (*MultiplyMZ)      (Matrix4* matrix4, float z);

    /**
     * Translate matrix4 by vector3.
     */
    void  (*Translate)       (Matrix4* matrix4, float x, float y, float z);

    /**
     * Rotate matrix4 by angle and vector3.
     */
    void  (*Rotate)          (Matrix4* matrix4, float angle, float x, float y, float z);

    /**
     * Rotate matrix4 by x axis with angle.
     */
    void  (*RotateX)         (Matrix4* matrix4, float angle);

    /**
     * Rotate matrix4 by y axis with angle.
     */
    void  (*RotateY)         (Matrix4* matrix4, float angle);

    /**
     * Rotate matrix4 by z axis with angle.
     */
    void  (*RotateZ)         (Matrix4* matrix4, float angle);

    /**
     * Scale matrix4 by vector3.
     */
    void  (*Scale)           (Matrix4* matrix4, float x, float y, float z);

    /**
     * Invert matrix4 into outInverse.
     */
    void  (*Inverse)         (Matrix4* matrix4, Matrix4* outInverse);

    /**
     * Transpose matrix4 into outTranspose.
     */
    void  (*Transpose)       (Matrix4* matrix4, Matrix4* outTranspose);

    /**
     * Invert transpose matrix4 into outInverseTranspose.
     */
    void  (*InverseTranspose)(Matrix4* matrix4, Matrix4* outInverseTranspose);

    /**
     * Computes an orthogonal projection matrix4.
     */
    void  (*Ortho)           (
                                float    left,
                                float    right,
                                float    bottom,
                                float    top,
                                float    near,
                                float    far,
                                Matrix4* outProjection
                             );

    /**
     * Define a projection matrix4 in terms of six clip planes.
     */
    void  (*Frustum)         (
                                float    left,
                                float    right,
                                float    bottom,
                                float    top,
                                float    near,
                                float    far,
                                Matrix4* outProjection
                              );

    /**
     * Define a projection matrix in terms of a field of view angle an aspect ratio, and z clip planes.
     */
    void  (*Perspective)     (float fovy, float aspect, float near, float far, Matrix4* outProjection);

    /**
     * Define a viewing transformation in terms of an eye point, a center of view, and an up vector.
     */
    void  (*LookAt)          (
                                float    eyeX,     float eyeY,    float eyeZ,
                                float    centerX,  float centerY, float centerZ,
                                float    upX,      float upY,     float upZ,
                                Matrix4* outView
                             );
};


extern struct AMatrix AMatrix[1];


/**
* Construct Matrix composite literal.
*/
#define AMatrix_Make(...) \
   (Matrix4[1]) {{__VA_ARGS__}}




#endif
