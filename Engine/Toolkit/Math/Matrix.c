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
 * Since    : 2013-1-6
 * Update   : 2019-1-17
 * Author   : scott.cgi
 */


#include <math.h>
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Platform/Log.h"


/**
 * x: right   axis
 * y: up      axis
 * z: forward axis
 * t: translation
 * 
 * (m0  m1  m2 ) x m3   |  (m0  m1  m2 ) x m3
 * (m4  m5  m6 ) y m7   |  (m4  m5  m6 ) y m7
 * (m8  m9  m10) z m11  |  (m8  m9  m10) z m11
 * (m12 m13 m14) t m15  |  (m12 m13 m14) t m15
 *
 * M-cr      = L-column         * R-row
 * M-00 (m0) = L-column[0]-048  * R-row[0]-012
 * M-10 (m1) = L-column[1]-159  * R-row[0]-012
 * M-20 (m2) = L-column[2]-2610 * R-row[0]-012
 */
static void MultiplyMM(Matrix4* left, Matrix4* right, Matrix4* outMatrix4)
{
    outMatrix4->m0  = left->m0 * right->m0  + left->m4 * right->m1  + left->m8  * right->m2  + left->m12 * right->m3;
    outMatrix4->m1  = left->m1 * right->m0  + left->m5 * right->m1  + left->m9  * right->m2  + left->m13 * right->m3;
    outMatrix4->m2  = left->m2 * right->m0  + left->m6 * right->m1  + left->m10 * right->m2  + left->m14 * right->m3;
    outMatrix4->m3  = left->m3 * right->m0  + left->m7 * right->m1  + left->m11 * right->m2  + left->m15 * right->m3;

    outMatrix4->m4  = left->m0 * right->m4  + left->m4 * right->m5  + left->m8  * right->m6  + left->m12 * right->m7;
    outMatrix4->m5  = left->m1 * right->m4  + left->m5 * right->m5  + left->m9  * right->m6  + left->m13 * right->m7;
    outMatrix4->m6  = left->m2 * right->m4  + left->m6 * right->m5  + left->m10 * right->m6  + left->m14 * right->m7;
    outMatrix4->m7  = left->m3 * right->m4  + left->m7 * right->m5  + left->m11 * right->m6  + left->m15 * right->m7;

    outMatrix4->m8  = left->m0 * right->m8  + left->m4 * right->m9  + left->m8  * right->m10 + left->m12 * right->m11;
    outMatrix4->m9  = left->m1 * right->m8  + left->m5 * right->m9  + left->m9  * right->m10 + left->m13 * right->m11;
    outMatrix4->m10 = left->m2 * right->m8  + left->m6 * right->m9  + left->m10 * right->m10 + left->m14 * right->m11;
    outMatrix4->m11 = left->m3 * right->m8  + left->m7 * right->m9  + left->m11 * right->m10 + left->m15 * right->m11;

    outMatrix4->m12 = left->m0 * right->m12 + left->m4 * right->m13 + left->m8  * right->m14 + left->m12 * right->m15;
    outMatrix4->m13 = left->m1 * right->m12 + left->m5 * right->m13 + left->m9  * right->m14 + left->m13 * right->m15;
    outMatrix4->m14 = left->m2 * right->m12 + left->m6 * right->m13 + left->m10 * right->m14 + left->m14 * right->m15;
    outMatrix4->m15 = left->m3 * right->m12 + left->m7 * right->m13 + left->m11 * right->m14 + left->m15 * right->m15;
}


static void MultiplyMMM(Matrix4* m1, Matrix4* m2, Matrix4* m3, Matrix4* outM23, Matrix4* outM231)
{
    outM23->m0   = m2->m0 * m3->m0  + m2->m4 * m3->m1  + m2->m8  * m3->m2  + m2->m12 * m3->m3;
    outM23->m1   = m2->m1 * m3->m0  + m2->m5 * m3->m1  + m2->m9  * m3->m2  + m2->m13 * m3->m3;
    outM23->m2   = m2->m2 * m3->m0  + m2->m6 * m3->m1  + m2->m10 * m3->m2  + m2->m14 * m3->m3;
    outM23->m3   = m2->m3 * m3->m0  + m2->m7 * m3->m1  + m2->m11 * m3->m2  + m2->m15 * m3->m3;

    outM23->m4   = m2->m0 * m3->m4  + m2->m4 * m3->m5  + m2->m8  * m3->m6  + m2->m12 * m3->m7;
    outM23->m5   = m2->m1 * m3->m4  + m2->m5 * m3->m5  + m2->m9  * m3->m6  + m2->m13 * m3->m7;
    outM23->m6   = m2->m2 * m3->m4  + m2->m6 * m3->m5  + m2->m10 * m3->m6  + m2->m14 * m3->m7;
    outM23->m7   = m2->m3 * m3->m4  + m2->m7 * m3->m5  + m2->m11 * m3->m6  + m2->m15 * m3->m7;

    outM23->m8   = m2->m0 * m3->m8  + m2->m4 * m3->m9  + m2->m8  * m3->m10 + m2->m12 * m3->m11;
    outM23->m9   = m2->m1 * m3->m8  + m2->m5 * m3->m9  + m2->m9  * m3->m10 + m2->m13 * m3->m11;
    outM23->m10  = m2->m2 * m3->m8  + m2->m6 * m3->m9  + m2->m10 * m3->m10 + m2->m14 * m3->m11;
    outM23->m11  = m2->m3 * m3->m8  + m2->m7 * m3->m9  + m2->m11 * m3->m10 + m2->m15 * m3->m11;

    outM23->m12  = m2->m0 * m3->m12 + m2->m4 * m3->m13 + m2->m8  * m3->m14 + m2->m12 * m3->m15;
    outM23->m13  = m2->m1 * m3->m12 + m2->m5 * m3->m13 + m2->m9  * m3->m14 + m2->m13 * m3->m15;
    outM23->m14  = m2->m2 * m3->m12 + m2->m6 * m3->m13 + m2->m10 * m3->m14 + m2->m14 * m3->m15;
    outM23->m15  = m2->m3 * m3->m12 + m2->m7 * m3->m13 + m2->m11 * m3->m14 + m2->m15 * m3->m15;

//----------------------------------------------------------------------------------------------------------------------

    outM231->m0  = m1->m0 * outM23->m0  + m1->m4 * outM23->m1  + m1->m8  * outM23->m2  + m1->m12 * outM23->m3;
    outM231->m1  = m1->m1 * outM23->m0  + m1->m5 * outM23->m1  + m1->m9  * outM23->m2  + m1->m13 * outM23->m3;
    outM231->m2  = m1->m2 * outM23->m0  + m1->m6 * outM23->m1  + m1->m10 * outM23->m2  + m1->m14 * outM23->m3;
    outM231->m3  = m1->m3 * outM23->m0  + m1->m7 * outM23->m1  + m1->m11 * outM23->m2  + m1->m15 * outM23->m3;

    outM231->m4  = m1->m0 * outM23->m4  + m1->m4 * outM23->m5  + m1->m8  * outM23->m6  + m1->m12 * outM23->m7;
    outM231->m5  = m1->m1 * outM23->m4  + m1->m5 * outM23->m5  + m1->m9  * outM23->m6  + m1->m13 * outM23->m7;
    outM231->m6  = m1->m2 * outM23->m4  + m1->m6 * outM23->m5  + m1->m10 * outM23->m6  + m1->m14 * outM23->m7;
    outM231->m7  = m1->m3 * outM23->m4  + m1->m7 * outM23->m5  + m1->m11 * outM23->m6  + m1->m15 * outM23->m7;

    outM231->m8  = m1->m0 * outM23->m8  + m1->m4 * outM23->m9  + m1->m8  * outM23->m10 + m1->m12 * outM23->m11;
    outM231->m9  = m1->m1 * outM23->m8  + m1->m5 * outM23->m9  + m1->m9  * outM23->m10 + m1->m13 * outM23->m11;
    outM231->m10 = m1->m2 * outM23->m8  + m1->m6 * outM23->m9  + m1->m10 * outM23->m10 + m1->m14 * outM23->m11;
    outM231->m11 = m1->m3 * outM23->m8  + m1->m7 * outM23->m9  + m1->m11 * outM23->m10 + m1->m15 * outM23->m11;

    outM231->m12 = m1->m0 * outM23->m12 + m1->m4 * outM23->m13 + m1->m8  * outM23->m14 + m1->m12 * outM23->m15;
    outM231->m13 = m1->m1 * outM23->m12 + m1->m5 * outM23->m13 + m1->m9  * outM23->m14 + m1->m13 * outM23->m15;
    outM231->m14 = m1->m2 * outM23->m12 + m1->m6 * outM23->m13 + m1->m10 * outM23->m14 + m1->m14 * outM23->m15;
    outM231->m15 = m1->m3 * outM23->m12 + m1->m7 * outM23->m13 + m1->m11 * outM23->m14 + m1->m15 * outM23->m15;
}

/**
 * x: right   axis
 * y: up      axis
 * z: forward axis
 * t: translation
 *
 * (m0  m1  m2 ) x m3  w  |  (x y z) w
 * (m4  m5  m6 ) y m7  w  |
 * (m8  m9  m10) z m11 w  |
 * (m12 m13 m14) t m15 w  |
 *
 * V-c[xyz]r[0] = L-column * V-row
 */
static void MultiplyMV4(Matrix4* matrix4, float x, float y, float z, float w, Vector4* outVector4)
{
    outVector4->x = matrix4->m0 * x + matrix4->m4 * y + matrix4->m8  * z + matrix4->m12 * w;
    outVector4->y = matrix4->m1 * x + matrix4->m5 * y + matrix4->m9  * z + matrix4->m13 * w;
    outVector4->z = matrix4->m2 * x + matrix4->m6 * y + matrix4->m10 * z + matrix4->m14 * w;
    outVector4->w = matrix4->m3 * x + matrix4->m7 * y + matrix4->m11 * z + matrix4->m15 * w;
}


static void multiplyMV3(Matrix4* matrix4, float x, float y, float z, Vector3* outVector3)
{
    outVector3->x = matrix4->m0 * x + matrix4->m4 * y + matrix4->m8  * z + matrix4->m12;
    outVector3->y = matrix4->m1 * x + matrix4->m5 * y + matrix4->m9  * z + matrix4->m13;
    outVector3->z = matrix4->m2 * x + matrix4->m6 * y + matrix4->m10 * z + matrix4->m14;
}


static void MultiplyMV2(Matrix4* matrix4, float x, float y, Vector2* outVector2)
{
    outVector2->x = matrix4->m0 * x + matrix4->m4 * y + matrix4->m12;
    outVector2->y = matrix4->m1 * x + matrix4->m5 * y + matrix4->m13;
}


static float MultiplyMX(Matrix4* matrix4, float x)
{
    return matrix4->m0 * x + matrix4->m12;
}


static float MultiplyMY(Matrix4* matrix4, float y)
{
    return matrix4->m5 * y + matrix4->m13;
}


static float MultiplyMZ(Matrix4* matrix4, float z)
{
   return matrix4->m10 * z + matrix4->m14;
}


/**
 * x: right   axis
 * y: up      axis
 * z: forward axis
 * t: translation
 *
 * (m0  m1  m2 ) x m3  w  |  (x y z)
 * (m4  m5  m6 ) y m7  w  |
 * (m8  m9  m10) z m11 w  |
 * (m12 m13 m14) t m15 w  |
 */
static void Translate(Matrix4* matrix4, float x, float y, float z)
{
    matrix4->m12 += matrix4->m0 * x + matrix4->m4 * y + matrix4->m8  * z;
    matrix4->m13 += matrix4->m1 * x + matrix4->m5 * y + matrix4->m9  * z;
    matrix4->m14 += matrix4->m2 * x + matrix4->m6 * y + matrix4->m10 * z;
}


/**
 * Rotate identity matrix4 and multiply matrix4.
 *
 * s = sin angle
 * c = cos angle
 * n = 1 - c
 *
 * (m0  m1  m2 ) x m3  w  |  xxn + c   xyn + zs  zxn − ys 0
 * (m4  m5  m6 ) y m7  w  |  xyn - zs  yyn + c   yzn + xs 0
 * (m8  m9  m10) z m11 w  |  zxn + ys  yzn - xs  zzn + c  0
 * (m12 m13 m14) t m15 w  |  0         0         0        1
 * 
 */
static void Rotate(Matrix4* matrix4, float angle, float x, float y, float z)
{
    angle     = AMath_ToRadian(angle);
    float s   = sinf(angle);
    float c   = cosf(angle);
    float len = AVector3_Length3(x, y, z);

    if (len  != 1.0f)
    {
        // reciprocal length
        float recipLen = 1.0f / len;
        x             *= recipLen;
        y             *= recipLen;
        z             *= recipLen;
    }

    float nc = 1.0f - c;
    float xy = x * y;
    float yz = y * z;
    float zx = z * x;
    float xs = x * s;
    float ys = y * s;
    float zs = z * s;

/*
    Matrix4 temp1[1] =
    {{
        // right x axis
        x  * x  * nc + c,
        xy * nc + zs,
        zx * nc - ys,
        0.0f,

        // up y axis
        xy * nc - zs,
        y  * y  * nc + c,
        yz * nc + xs,
        0.0f,

        // forward z axis
        zx * nc + ys,
        yz * nc - xs,
        z  * z  * nc + c,
        0.0f,

        // translation
        0.0f, 0.0f, 0.0f, 1.0f,
    }};

    Matrix4 temp2[1];
    MultiplyMM(matrix4, temp1, temp2);

    *matrix4 = *temp2; // memcpy(matrix->m, temp2->m, sizeof(Matrix4));
*/

    /* expand matrix multiplication */
    
    float rm0    = x  * x  * nc + c;
    float rm1    = xy * nc + zs;
    float rm2    = zx * nc - ys;

    float rm4    = xy * nc - zs;
    float rm5    = y  * y  * nc + c;
    float rm6    = yz * nc + xs;

    float rm8    = zx * nc + ys;
    float rm9    = yz * nc - xs;
    float rm10   = z  * z  * nc + c;

    float lm0    = matrix4->m0;
    float lm1    = matrix4->m1;
    float lm2    = matrix4->m2;
    float lm3    = matrix4->m3;

    float lm4    = matrix4->m4;
    float lm5    = matrix4->m5;
    float lm6    = matrix4->m6;
    float lm7    = matrix4->m7;

    float lm8    = matrix4->m8;
    float lm9    = matrix4->m9;
    float lm10   = matrix4->m10;
    float lm11   = matrix4->m11;

    matrix4->m0  = lm0 * rm0 + lm4 * rm1 + lm8  * rm2;
    matrix4->m1  = lm1 * rm0 + lm5 * rm1 + lm9  * rm2;
    matrix4->m2  = lm2 * rm0 + lm6 * rm1 + lm10 * rm2;
    matrix4->m3  = lm3 * rm0 + lm7 * rm1 + lm11 * rm2;

    matrix4->m4  = lm0 * rm4 + lm4 * rm5 + lm8  * rm6;
    matrix4->m5  = lm1 * rm4 + lm5 * rm5 + lm9  * rm6;
    matrix4->m6  = lm2 * rm4 + lm6 * rm5 + lm10 * rm6;
    matrix4->m7  = lm3 * rm4 + lm7 * rm5 + lm11 * rm6;

    matrix4->m8  = lm0 * rm8 + lm4 * rm9 + lm8  * rm10;
    matrix4->m9  = lm1 * rm8 + lm5 * rm9 + lm9  * rm10;
    matrix4->m10 = lm2 * rm8 + lm6 * rm9 + lm10 * rm10;
    matrix4->m11 = lm3 * rm8 + lm7 * rm9 + lm11 * rm10;
}


/**
 * x: right   axis
 * y: up      axis
 * z: forward axis
 * t: translation
 *
 * (m0  m1  m2 ) x m3   |  (1  0  0) x 0
 * (m4  m5  m6 ) y m7   |  (0  c  s) y 0
 * (m8  m9  m10) z m11  |  (0 -s  c) z 0
 * (m12 m13 m14) t m15  |  (0  0  0) t 1
 * 
 */
static void RotateX(Matrix4* matrix4, float angle)
{
    angle   = AMath_ToRadian(angle);
    float s = sinf(angle);
    float c = cosf(angle);

/*
    Matrix4 temp1[1] =
    {{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f,    c,    s, 0.0f,
        0.0f,   -s,    c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    }};

    Matrix4 temp2[1];
    MultiplyMM(matrix4, temp1, temp2);

    *matrix4 = *temp2; // memcpy(matrix4->m, temp2->m, sizeof(Matrix4));
*/

    /* expand matrix multiplication */
    
    float m4     = matrix4->m4;
    float m5     = matrix4->m5;
    float m6     = matrix4->m6;
    float m7     = matrix4->m7;

    matrix4->m4  =  m4 *  c + matrix4->m8  * s;
    matrix4->m5  =  m5 *  c + matrix4->m9  * s;
    matrix4->m6  =  m6 *  c + matrix4->m10 * s;
    matrix4->m7  =  m7 *  c + matrix4->m11 * s;

    matrix4->m8  =  m4 * -s + matrix4->m8  * c;
    matrix4->m9  =  m5 * -s + matrix4->m9  * c;
    matrix4->m10 =  m6 * -s + matrix4->m10 * c;
    matrix4->m11 =  m7 * -s + matrix4->m11 * c;
}



/**
 * x: right   axis
 * y: up      axis
 * z: forward axis
 * t: translation
 *
 * (m0  m1  m2 ) x m3   |  (c  0 -s) x 0
 * (m4  m5  m6 ) y m7   |  (0  1  0) y 0
 * (m8  m9  m10) z m11  |  (s  0  c) z 0
 * (m12 m13 m14) t m15  |  (0  0  0) t 1
 */
static void RotateY(Matrix4* matrix4, float angle)
{
    angle   = AMath_ToRadian(angle);
    float s = sinf(angle);
    float c = cosf(angle);

/*
    Matrix4 temp1[1] =
    {{
           c, 0.0f,   -s, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
           s, 0.0f,    c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    }};

    Matrix4 temp2[1];
    MultiplyMM(matrix4, temp1, temp2);

    *matrix4 = *temp2; // memcpy(matrix4->m, temp2->m, sizeof(Matrix4));
*/

    /* expand matrix multiplication */

    float m0     = matrix4->m0;
    float m1     = matrix4->m1;
    float m2     = matrix4->m2;
    float m3     = matrix4->m3;

    matrix4->m0  = m0 * c + matrix4->m8  * -s;
    matrix4->m1  = m1 * c + matrix4->m9  * -s;
    matrix4->m2  = m2 * c + matrix4->m10 * -s;
    matrix4->m3  = m3 * c + matrix4->m11 * -s;

    matrix4->m8  = m0 * s + matrix4->m8  *  c;
    matrix4->m9  = m1 * s + matrix4->m9  *  c;
    matrix4->m10 = m2 * s + matrix4->m10 *  c;
    matrix4->m11 = m3 * s + matrix4->m11 *  c;
}


/**
 * x: right   axis
 * y: up      axis
 * z: forward axis
 * t: translation
 *
 * (m0  m1  m2 ) x m3   |  ( c  s  0) x 0
 * (m4  m5  m6 ) y m7   |  (-s  c  0) y 0
 * (m8  m9  m10) z m11  |  ( 0  0  1) z 0
 * (m12 m13 m14) t m15  |  ( 0  0  0) t 1
*/
static void RotateZ(Matrix4* matrix4, float angle)
{
    angle   = AMath_ToRadian(angle);
    float s = sinf(angle);
    float c = cosf(angle);

/*
    Matrix4 temp1[1] =
    {{
           c,    s, 0.0f, 0.0f,
          -s,    c, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    }};

    Matrix4 temp2[1];
    MultiplyMM(matrix4, temp1, temp2);

    *matrix4 = *temp2; // memcpy(matrix4->m, temp2->m, sizeof(Matrix4));
*/

    /* expand matrix multiplication */

    float m0    = matrix4->m0;
    float m1    = matrix4->m1;
    float m2    = matrix4->m2;
    float m3    = matrix4->m3;

    matrix4->m0 = m0 *  c + matrix4->m4 * s;
    matrix4->m1 = m1 *  c + matrix4->m5 * s;
    matrix4->m2 = m2 *  c + matrix4->m6 * s;
    matrix4->m3 = m3 *  c + matrix4->m7 * s;

    matrix4->m4 = m0 * -s + matrix4->m4 * c;
    matrix4->m5 = m1 * -s + matrix4->m5 * c;
    matrix4->m6 = m2 * -s + matrix4->m6 * c;
    matrix4->m7 = m3 * -s + matrix4->m7 * c;
}


/**
 * x: right   axis
 * y: up      axis
 * z: forward axis
 * t: translation
 *
 * (m0  m1  m2 ) x m3  w
 * (m4  m5  m6 ) y m7  w
 * (m8  m9  m10) z m11 w
 * (m12 m13 m14) t m15 w
 */
static void Scale(Matrix4* matrix4, float sx, float sy, float sz)
{
    matrix4->m0  *= sx;
    matrix4->m1  *= sx;
    matrix4->m2  *= sx;

    matrix4->m4  *= sy;
    matrix4->m5  *= sy;
    matrix4->m6  *= sy;

    matrix4->m8  *= sz;
    matrix4->m9  *= sz;
    matrix4->m10 *= sz;
}


static void Inverse(Matrix4* matrix4, Matrix4* outInverse)
{
    float a0 = matrix4->m0  * matrix4->m5  - matrix4->m1  * matrix4->m4;
    float a1 = matrix4->m0  * matrix4->m6  - matrix4->m2  * matrix4->m4;
    float a2 = matrix4->m0  * matrix4->m7  - matrix4->m3  * matrix4->m4;
    float a3 = matrix4->m1  * matrix4->m6  - matrix4->m2  * matrix4->m5;
    float a4 = matrix4->m1  * matrix4->m7  - matrix4->m3  * matrix4->m5;
    float a5 = matrix4->m2  * matrix4->m7  - matrix4->m3  * matrix4->m6;

    float b0 = matrix4->m8  * matrix4->m13 - matrix4->m9  * matrix4->m12;
    float b1 = matrix4->m8  * matrix4->m14 - matrix4->m10 * matrix4->m12;
    float b2 = matrix4->m8  * matrix4->m15 - matrix4->m11 * matrix4->m12;
    float b3 = matrix4->m9  * matrix4->m14 - matrix4->m10 * matrix4->m13;
    float b4 = matrix4->m9  * matrix4->m15 - matrix4->m11 * matrix4->m13;
    float b5 = matrix4->m10 * matrix4->m15 - matrix4->m11 * matrix4->m14;

/*
    // calculate the determinant
    float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

    // close to zero, can't invert
    if (fabsf(det) < FLT_EPSILON)
    {
         return false;
    }

    float scalar = 1.0f / det;
*/

    float scalar = 1.0f / (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);

    // support the case where matrix == result

    outInverse->m0  = ( matrix4->m5  * b5 - matrix4->m6  * b4 + matrix4->m7  * b3) * scalar;
    outInverse->m1  = (-matrix4->m1  * b5 + matrix4->m2  * b4 - matrix4->m3  * b3) * scalar;
    outInverse->m2  = ( matrix4->m13 * a5 - matrix4->m14 * a4 + matrix4->m15 * a3) * scalar;
    outInverse->m3  = (-matrix4->m9  * a5 + matrix4->m10 * a4 - matrix4->m11 * a3) * scalar;

    outInverse->m4  = (-matrix4->m4  * b5 + matrix4->m6  * b2 - matrix4->m7  * b1) * scalar;
    outInverse->m5  = ( matrix4->m0  * b5 - matrix4->m2  * b2 + matrix4->m3  * b1) * scalar;
    outInverse->m6  = (-matrix4->m12 * a5 + matrix4->m14 * a2 - matrix4->m15 * a1) * scalar;
    outInverse->m7  = ( matrix4->m8  * a5 - matrix4->m10 * a2 + matrix4->m11 * a1) * scalar;

    outInverse->m8  = ( matrix4->m4  * b4 - matrix4->m5  * b2 + matrix4->m7  * b0) * scalar;
    outInverse->m9  = (-matrix4->m0  * b4 + matrix4->m1  * b2 - matrix4->m3  * b0) * scalar;
    outInverse->m10 = ( matrix4->m12 * a4 - matrix4->m13 * a2 + matrix4->m15 * a0) * scalar;
    outInverse->m11 = (-matrix4->m8  * a4 + matrix4->m9  * a2 - matrix4->m11 * a0) * scalar;

    outInverse->m12 = (-matrix4->m4  * b3 + matrix4->m5  * b1 - matrix4->m6  * b0) * scalar;
    outInverse->m13 = ( matrix4->m0  * b3 - matrix4->m1  * b1 + matrix4->m2  * b0) * scalar;
    outInverse->m14 = (-matrix4->m12 * a3 + matrix4->m13 * a1 - matrix4->m14 * a0) * scalar;
    outInverse->m15 = ( matrix4->m8  * a3 - matrix4->m9  * a1 + matrix4->m10 * a0) * scalar;

//  return true;
}


static void Transpose(Matrix4* matrix, Matrix4* outTranspose)
{
    *outTranspose = *AMatrix_Make
                     (
                         matrix->m0, matrix->m4, matrix->m8,  matrix->m12,
                         matrix->m1, matrix->m5, matrix->m9,  matrix->m13,
                         matrix->m2, matrix->m6, matrix->m10, matrix->m14,
                         matrix->m3, matrix->m7, matrix->m11, matrix->m15
                     );
}


/**
 * Transpose and inverse.
 */
static void InverseTranspose(Matrix4* matrix4, Matrix4* outInverseTranspose)
{
    float a0 = matrix4->m0  * matrix4->m5  - matrix4->m4  * matrix4->m1;
    float a1 = matrix4->m0  * matrix4->m9  - matrix4->m8  * matrix4->m1;
    float a2 = matrix4->m0  * matrix4->m13 - matrix4->m12 * matrix4->m1;
    float a3 = matrix4->m4  * matrix4->m9  - matrix4->m8  * matrix4->m5;
    float a4 = matrix4->m4  * matrix4->m13 - matrix4->m12 * matrix4->m5;
    float a5 = matrix4->m8  * matrix4->m13 - matrix4->m12 * matrix4->m9 ;

    float b0 = matrix4->m2  * matrix4->m7  - matrix4->m6  * matrix4->m3;
    float b1 = matrix4->m2  * matrix4->m11 - matrix4->m10 * matrix4->m3;
    float b2 = matrix4->m2  * matrix4->m15 - matrix4->m14 * matrix4->m3;
    float b3 = matrix4->m6  * matrix4->m11 - matrix4->m10 * matrix4->m7;
    float b4 = matrix4->m6  * matrix4->m15 - matrix4->m14 * matrix4->m7;
    float b5 = matrix4->m10 * matrix4->m15 - matrix4->m14 * matrix4->m11;

/*
    // calculate the determinant
    float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

    // close to zero, can't invert
    if (fabsf(det) < FLT_EPSILON)
    {
       return false;
    }

    float scalar = 1.0f / det;
*/

    float scalar = 1.0f / (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);

    // support the case where matrix == result

    outInverseTranspose->m0  = ( matrix4->m5 * b5 - matrix4->m9  * b4 + matrix4->m13 * b3) * scalar;
    outInverseTranspose->m1  = (-matrix4->m4 * b5 + matrix4->m8  * b4 - matrix4->m12 * b3) * scalar;
    outInverseTranspose->m2  = ( matrix4->m7 * a5 - matrix4->m11 * a4 + matrix4->m15 * a3) * scalar;
    outInverseTranspose->m3  = (-matrix4->m6 * a5 + matrix4->m10 * a4 - matrix4->m14 * a3) * scalar;

    outInverseTranspose->m4  = (-matrix4->m1 * b5 + matrix4->m9  * b2 - matrix4->m13 * b1) * scalar;
    outInverseTranspose->m5  = ( matrix4->m0 * b5 - matrix4->m8  * b2 + matrix4->m12 * b1) * scalar;
    outInverseTranspose->m6  = (-matrix4->m3 * a5 + matrix4->m11 * a2 - matrix4->m15 * a1) * scalar;
    outInverseTranspose->m7  = ( matrix4->m2 * a5 - matrix4->m10 * a2 + matrix4->m14 * a1) * scalar;

    outInverseTranspose->m8  = ( matrix4->m1 * b4 - matrix4->m5  * b2 + matrix4->m13 * b0) * scalar;
    outInverseTranspose->m9  = (-matrix4->m0 * b4 + matrix4->m4  * b2 - matrix4->m12 * b0) * scalar;
    outInverseTranspose->m10 = ( matrix4->m3 * a4 - matrix4->m7  * a2 + matrix4->m15 * a0) * scalar;
    outInverseTranspose->m11 = (-matrix4->m2 * a4 + matrix4->m6  * a2 - matrix4->m14 * a0) * scalar;

    outInverseTranspose->m12 = (-matrix4->m1 * b3 + matrix4->m5  * b1 - matrix4->m9  * b0) * scalar;
    outInverseTranspose->m13 = ( matrix4->m0 * b3 - matrix4->m4  * b1 + matrix4->m8  * b0) * scalar;
    outInverseTranspose->m14 = (-matrix4->m3 * a3 + matrix4->m7  * a1 - matrix4->m11 * a0) * scalar;
    outInverseTranspose->m15 = ( matrix4->m2 * a3 - matrix4->m6  * a1 + matrix4->m10 * a0) * scalar;

//  return true;
}


static void Perspective(float fovy, float aspect, float near, float far, Matrix4* outProjection)
{
    ALog_A
    (
        far != near,
        "AMatrix Perspective failed, because far == near"
    );

    fovy               = 1.0f / tanf(fovy * DEGREE_TO_RADIAN2);
    float range        = 1.0f / (near - far);

    outProjection->m0  = fovy / aspect;
    outProjection->m1  = 0.0f;
    outProjection->m2  = 0.0f;
    outProjection->m3  = 0.0f;

    outProjection->m4  = 0.0f;
    outProjection->m5  = fovy;
    outProjection->m6  = 0.0f;
    outProjection->m7  = 0.0f;

    outProjection->m8  = 0.0f;
    outProjection->m9  = 0.0f;
    outProjection->m10 = (far + near) * range;
    outProjection->m11 = -1.0f;

    outProjection->m12 = 0.0f;
    outProjection->m13 = 0.0f;
    outProjection->m14 = 2.0f * far * near * range;
    outProjection->m15 = 0.0f;
}


static void Orthographic(float left, float right, float bottom, float top, float near, float far, Matrix4* outProjection)
{
    ALog_A
    (
        right != left && top != bottom && far != near,
        "AMatrix Orthographic failed, because right == left || top == bottom || far == near"
    );

    float width        = 1.0f / (right - left);
    float height       = 1.0f / (top   - bottom);
    float depth        = 1.0f / (far   - near);

    outProjection->m0  = 2.0f * width;  // x
    outProjection->m1  = 0.0f;
    outProjection->m2  = 0.0f;
    outProjection->m3  = 0.0f;

    outProjection->m4  = 0.0f;
    outProjection->m5  = 2.0f * height; // y
    outProjection->m6  = 0.0f;
    outProjection->m7  = 0.0f;

    outProjection->m8  = 0.0f;
    outProjection->m9  = 0.0f;
    outProjection->m10 = -2.0f * depth; // z
    outProjection->m11 = 0.0f;

    outProjection->m12 = -(right + left)   * width;  // tx
    outProjection->m13 = -(top   + bottom) * height; // ty
    outProjection->m14 = -(far   + near)   * depth;  // tz
    outProjection->m15 = 1.0f;
}


static void Frustum(float left, float right, float bottom, float top, float near, float far, Matrix4* outProjection)
{
    ALog_A
    (
        right != left && top != bottom && near != far && near > 0.0f && far > 0.0f,
        "AMatrix Frustum failed,"
        "because right == left || top == bottom || near == far || near <= 0.0f || far <= 0.0f"
    );

    float width        = 1.0f / (right - left);
    float height       = 1.0f / (top - bottom);
    float depth        = 1.0f / (near - far);

    outProjection->m0  = 2.0f * (near * width);  // x
    outProjection->m1  = 0.0f;
    outProjection->m2  = 0.0f;
    outProjection->m3  = 0.0f;

    outProjection->m4  = 0.0f;
    outProjection->m5  = 2.0f * (near * height); // y
    outProjection->m6  = 0.0f;
    outProjection->m7  = 0.0f;

    outProjection->m8  = 2.0f * ((right + left) * width); // A
    outProjection->m9  = (top + bottom) * height;         // B
    outProjection->m10 = (far + near)   * depth;          // C
    outProjection->m11 = -1.0f;

    outProjection->m12 = 0.0f;
    outProjection->m13 = 0.0f;
    outProjection->m14 = 2.0f * (far * near * depth);     // D
    outProjection->m15 = 0.0f;
}


static void LookAt
(
    float    eyeX,     float eyeY,    float eyeZ,
    float    centerX,  float centerY, float centerZ,
    float    upX,      float upY,     float upZ,
    Matrix4* outView
)
{

    // see the OpenGL GLUT documentation for gluLookAt for a description of the algorithm
    // we implement it in a straightforward way

    float fx   = centerX - eyeX;
    float fy   = centerY - eyeY;
    float fz   = centerZ - eyeZ;

    // normalize f
    float rlf  = AVector3_Normalize3(fx, fy, fz);
    fx        *= rlf;
    fy        *= rlf;
    fz        *= rlf;

    // compute s = f cross product up
    float sx  = fy * upZ - fz * upY;
    float sy  = fz * upX - fx * upZ;
    float sz  = fx * upY - fy * upX;

    // and normalize s
    float rls = AVector3_Normalize3(sx, sy, sz);
    sx       *= rls;
    sy       *= rls;
    sz       *= rls;

    // compute u = s cross product f
    float ux  = sy * fz - sz * fy;
    float uy  = sz * fx - sx * fz;
    float uz  = sx * fy - sy * fx;

    outView->m0  = sx;
    outView->m1  = ux;
    outView->m2  = -fx;
    outView->m3  = 0.0f;

    outView->m4  = sy;
    outView->m5  = uy;
    outView->m6  = -fy;
    outView->m7  = 0.0f;

    outView->m8  = sz;
    outView->m9  = uz;
    outView->m10 = -fz;
    outView->m11 = 0.0f;

    outView->m12 = 0.0f;
    outView->m13 = 0.0f;
    outView->m14 = 0.0f;
    outView->m15 = 1.0f;

    Translate(outView, -eyeX, -eyeY, -eyeZ);
}


struct AMatrix AMatrix[1] =
{{
    MultiplyMM,
    MultiplyMMM,
    MultiplyMV4,
    multiplyMV3,
    MultiplyMV2,
    MultiplyMX,
    MultiplyMY,
    MultiplyMZ,

    Translate,
    Rotate,
    RotateX,
    RotateY,
    RotateZ,
    Scale,
    Inverse,
    Transpose,
    InverseTranspose,
    Orthographic,
    Frustum,
    Perspective,
    LookAt,
}};
