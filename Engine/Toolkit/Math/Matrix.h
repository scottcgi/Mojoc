/*
 *
 *
 *  Created on: 2013-1-6
 *  Author: scott.cgi
 */

#ifndef matrix_h
#define matrix_h


#include "Mojoc/Toolkit/Math/Math.h"
#include "Mojoc/Toolkit/Math/Vector.h"

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


#define matrix4_identity           \
	{                              \
       {                           \
           1.0f, 0.0f, 0.0f, 0.0f, \
           0.0f, 1.0f, 0.0f, 0.0f, \
           0.0f, 0.0f, 1.0f, 0.0f, \
           0.0f, 0.0f, 0.0f, 1.0f, \
       }                           \
	}


struct AMatrix
{
	/**
     * Multiply left and right matrix4 into outMatrix4
     */
    void  (*MultiplyMM)      (Matrix4* left, Matrix4* right, Matrix4* outMatrix4);

	/**
     * Multiply m2 and m3      into outM23
     * Multiply m1 and outM23 into outM231
     */
    void  (*MultiplyMMM)     (Matrix4* m1, Matrix4* m2, Matrix4* m3, Matrix4* outM23, Matrix4* outM231);

    /**
     * Multiply matrix4 * vector4 into outVector4
     */
    void  (*MultiplyMV4)     (Matrix4* matrix4, float x, float y, float z, float w, Vector4* outVector4);

    /**
     * Multiply matrix4 * vector3, w default 1.0f into outVector3
     */
    void  (*MultiplyMV3)     (Matrix4* matrix4, float x, float y, float z, Vector3* outVector3);

    /**
     * Multiply matrix4 * vector2, z and w default 1.0f into outVector2
     */
    void  (*MultiplyMV2)     (Matrix4* matrix4, float x, float y, Vector2* outVector2);

    /**
     * Multiply matrix4 * x, return transformed x
     */
    float (*MultiplyMX)      (Matrix4* matrix4, float x);

    /**
     * Multiply matrix4 * y, return transformed y
     */
    float (*MultiplyMY)      (Matrix4* matrix4, float y);

    /**
     * Translate matrix4 with vector3
     */
    void  (*Translate)       (Matrix4* matrix4, float x, float y, float z);

    /**
     * Rotate matrix4 with angle and vector3
     */
    void  (*Rotate)          (Matrix4* matrix4, float angle, float x, float y, float z);

    /**
     * Rotate matrix4 with angle by x axis
     */
    void  (*RotateX)         (Matrix4* matrix4, float angle);

    /**
     * Rotate matrix4 with angle by y axis
     */
    void  (*RotateY)         (Matrix4* matrix4, float angle);

    /**
     * Rotate matrix4 with angle by z axis
     */
    void  (*RotateZ)         (Matrix4* matrix4, float angle);

    /**
     * Scale matrix4 with vector3
     */
    void  (*Scale)           (Matrix4* matrix4, float x, float y, float z);

    /**
     *  Invert matrix4 into outInverse
     */
    void  (*Inverse)         (Matrix4* matrix4, Matrix4* outInverse);

    /**
     * Transpose matrix4 into outTransposed
     */
    void  (*Transpose)       (Matrix4* matrix4, Matrix4* outTranspose);

    /**
     * Invert transpose matrix4 into outInverseTransposed
     */
    void  (*InverseTranspose)(Matrix4* matrix4, Matrix4* outInverseTranspose);

    /**
     * Computes an orthogonal projection matrix4
     */
    void  (*Ortho)           (float left, float right, float bottom, float top, float near, float far, Matrix4* outProjection);

    /**
     * Define a projection matrix4 in terms of six clip planes
     */
    void  (*Frustum)         (float left, float right, float bottom, float top, float near, float far, Matrix4* outProjection);

	/**
	 * Define a projection matrix in terms of a field of view angle
     * an aspect ratio, and z clip planes
	 */
	void  (*Perspective)     (float fovy, float aspect, float near, float far, Matrix4* outProjection);

    /**
     * Define a viewing transformation in terms of an eye point, a center of view, and an up vector
     */
    void  (*LookAt)          (float eyeX,    float eyeY,    float eyeZ,
						      float centerX, float centerY, float centerZ,
						      float upX,     float upY,     float upZ, Matrix4* outView);
};

extern struct AMatrix AMatrix[1];

#endif
