/*
 *
 *
 *  Created on: 2013-1-6
 *  Author: scott.cgi
 */

#include <math.h>

#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Platform/Log.h"


static void MultiplyMM(Matrix4* left, Matrix4* right, Matrix4* out_param matrix4)
{
    matrix4->m0  = left->m0 * right->m0  + left->m4 * right->m1  + left->m8  * right->m2  + left->m12 * right->m3;
    matrix4->m1  = left->m1 * right->m0  + left->m5 * right->m1  + left->m9  * right->m2  + left->m13 * right->m3;
    matrix4->m2  = left->m2 * right->m0  + left->m6 * right->m1  + left->m10 * right->m2  + left->m14 * right->m3;
    matrix4->m3  = left->m3 * right->m0  + left->m7 * right->m1  + left->m11 * right->m2  + left->m15 * right->m3;

    matrix4->m4  = left->m0 * right->m4  + left->m4 * right->m5  + left->m8  * right->m6  + left->m12 * right->m7;
    matrix4->m5  = left->m1 * right->m4  + left->m5 * right->m5  + left->m9  * right->m6  + left->m13 * right->m7;
    matrix4->m6  = left->m2 * right->m4  + left->m6 * right->m5  + left->m10 * right->m6  + left->m14 * right->m7;
    matrix4->m7  = left->m3 * right->m4  + left->m7 * right->m5  + left->m11 * right->m6  + left->m15 * right->m7;

    matrix4->m8  = left->m0 * right->m8  + left->m4 * right->m9  + left->m8  * right->m10 + left->m12 * right->m11;
    matrix4->m9  = left->m1 * right->m8  + left->m5 * right->m9  + left->m9  * right->m10 + left->m13 * right->m11;
    matrix4->m10 = left->m2 * right->m8  + left->m6 * right->m9  + left->m10 * right->m10 + left->m14 * right->m11;
    matrix4->m11 = left->m3 * right->m8  + left->m7 * right->m9  + left->m11 * right->m10 + left->m15 * right->m11;

    matrix4->m12 = left->m0 * right->m12 + left->m4 * right->m13 + left->m8  * right->m14 + left->m12 * right->m15;
    matrix4->m13 = left->m1 * right->m12 + left->m5 * right->m13 + left->m9  * right->m14 + left->m13 * right->m15;
    matrix4->m14 = left->m2 * right->m12 + left->m6 * right->m13 + left->m10 * right->m14 + left->m14 * right->m15;
    matrix4->m15 = left->m3 * right->m12 + left->m7 * right->m13 + left->m11 * right->m14 + left->m15 * right->m15;

/*
----------------------------------------------------------------------------------------------------
	for (int i = 0; i < 4; i++)
	{
		int i4 = i * 4;

		for (int j = 0; j < 4; j++)
		{
			float s = 0.0f;

			for (int k = 0; k < 4; k++)
			{
				s += left->m[k * 4 + j] * right->m[i4 + k];
			}

			matrix4->m[i4 + j] = s;
		}
	}
----------------------------------------------------------------------------------------------------
*/

}

static void MultiplyMMM(Matrix4* m1, Matrix4* m2, Matrix4* m3, Matrix4* out_param m23, Matrix4* out_param m231)
{
    m23->m0   = m2->m0 * m3->m0  + m2->m4 * m3->m1  + m2->m8  * m3->m2  + m2->m12 * m3->m3;
    m23->m1   = m2->m1 * m3->m0  + m2->m5 * m3->m1  + m2->m9  * m3->m2  + m2->m13 * m3->m3;
    m23->m2   = m2->m2 * m3->m0  + m2->m6 * m3->m1  + m2->m10 * m3->m2  + m2->m14 * m3->m3;
    m23->m3   = m2->m3 * m3->m0  + m2->m7 * m3->m1  + m2->m11 * m3->m2  + m2->m15 * m3->m3;

    m23->m4   = m2->m0 * m3->m4  + m2->m4 * m3->m5  + m2->m8  * m3->m6  + m2->m12 * m3->m7;
    m23->m5   = m2->m1 * m3->m4  + m2->m5 * m3->m5  + m2->m9  * m3->m6  + m2->m13 * m3->m7;
    m23->m6   = m2->m2 * m3->m4  + m2->m6 * m3->m5  + m2->m10 * m3->m6  + m2->m14 * m3->m7;
    m23->m7   = m2->m3 * m3->m4  + m2->m7 * m3->m5  + m2->m11 * m3->m6  + m2->m15 * m3->m7;

    m23->m8   = m2->m0 * m3->m8  + m2->m4 * m3->m9  + m2->m8  * m3->m10 + m2->m12 * m3->m11;
    m23->m9   = m2->m1 * m3->m8  + m2->m5 * m3->m9  + m2->m9  * m3->m10 + m2->m13 * m3->m11;
    m23->m10  = m2->m2 * m3->m8  + m2->m6 * m3->m9  + m2->m10 * m3->m10 + m2->m14 * m3->m11;
    m23->m11  = m2->m3 * m3->m8  + m2->m7 * m3->m9  + m2->m11 * m3->m10 + m2->m15 * m3->m11;

    m23->m12  = m2->m0 * m3->m12 + m2->m4 * m3->m13 + m2->m8  * m3->m14 + m2->m12 * m3->m15;
    m23->m13  = m2->m1 * m3->m12 + m2->m5 * m3->m13 + m2->m9  * m3->m14 + m2->m13 * m3->m15;
    m23->m14  = m2->m2 * m3->m12 + m2->m6 * m3->m13 + m2->m10 * m3->m14 + m2->m14 * m3->m15;
    m23->m15  = m2->m3 * m3->m12 + m2->m7 * m3->m13 + m2->m11 * m3->m14 + m2->m15 * m3->m15;

//--------------------------------------------------------------------------------------------------

    m231->m0  = m1->m0 * m23->m0  + m1->m4 * m23->m1  + m1->m8  * m23->m2  + m1->m12 * m23->m3;
    m231->m1  = m1->m1 * m23->m0  + m1->m5 * m23->m1  + m1->m9  * m23->m2  + m1->m13 * m23->m3;
    m231->m2  = m1->m2 * m23->m0  + m1->m6 * m23->m1  + m1->m10 * m23->m2  + m1->m14 * m23->m3;
    m231->m3  = m1->m3 * m23->m0  + m1->m7 * m23->m1  + m1->m11 * m23->m2  + m1->m15 * m23->m3;

    m231->m4  = m1->m0 * m23->m4  + m1->m4 * m23->m5  + m1->m8  * m23->m6  + m1->m12 * m23->m7;
    m231->m5  = m1->m1 * m23->m4  + m1->m5 * m23->m5  + m1->m9  * m23->m6  + m1->m13 * m23->m7;
    m231->m6  = m1->m2 * m23->m4  + m1->m6 * m23->m5  + m1->m10 * m23->m6  + m1->m14 * m23->m7;
    m231->m7  = m1->m3 * m23->m4  + m1->m7 * m23->m5  + m1->m11 * m23->m6  + m1->m15 * m23->m7;

    m231->m8  = m1->m0 * m23->m8  + m1->m4 * m23->m9  + m1->m8  * m23->m10 + m1->m12 * m23->m11;
    m231->m9  = m1->m1 * m23->m8  + m1->m5 * m23->m9  + m1->m9  * m23->m10 + m1->m13 * m23->m11;
    m231->m10 = m1->m2 * m23->m8  + m1->m6 * m23->m9  + m1->m10 * m23->m10 + m1->m14 * m23->m11;
    m231->m11 = m1->m3 * m23->m8  + m1->m7 * m23->m9  + m1->m11 * m23->m10 + m1->m15 * m23->m11;

    m231->m12 = m1->m0 * m23->m12 + m1->m4 * m23->m13 + m1->m8  * m23->m14 + m1->m12 * m23->m15;
    m231->m13 = m1->m1 * m23->m12 + m1->m5 * m23->m13 + m1->m9  * m23->m14 + m1->m13 * m23->m15;
    m231->m14 = m1->m2 * m23->m12 + m1->m6 * m23->m13 + m1->m10 * m23->m14 + m1->m14 * m23->m15;
    m231->m15 = m1->m3 * m23->m12 + m1->m7 * m23->m13 + m1->m11 * m23->m14 + m1->m15 * m23->m15;
}

static void MultiplyMV4(Matrix4* matrix4, float x, float y, float z, float w, Vector4* out_param vector4)
{
	vector4->x = matrix4->m0 * x + matrix4->m4 * y + matrix4->m8  * z + matrix4->m12 * w;
	vector4->y = matrix4->m1 * x + matrix4->m5 * y + matrix4->m9  * z + matrix4->m13 * w;
	vector4->z = matrix4->m2 * x + matrix4->m6 * y + matrix4->m10 * z + matrix4->m14 * w;
	vector4->w = matrix4->m3 * x + matrix4->m7 * y + matrix4->m11 * z + matrix4->m15 * w;

/*
----------------------------------------------------------------------------------------------------
  for (int i = 0; i < 4; i++)
  {
      vector4->v[i] = matrix4->m[i + 4 * 0] * x +
                      matrix4->m[i + 4 * 1] * y +
                      matrix4->m[i + 4 * 2] * z +
                      matrix4->m[i + 4 * 3] * w;
  }
----------------------------------------------------------------------------------------------------
*/
}


static void multiplyMV3(Matrix4* matrix4, float x, float y, float z, Vector3* out_param vector3)
{
	vector3->x = matrix4->m0 * x + matrix4->m4 * y + matrix4->m8  * z + matrix4->m12;
	vector3->y = matrix4->m1 * x + matrix4->m5 * y + matrix4->m9  * z + matrix4->m13;
	vector3->z = matrix4->m2 * x + matrix4->m6 * y + matrix4->m10 * z + matrix4->m14;

/*
----------------------------------------------------------------------------------------------------
    for (int i = 0; i < 3; i++)
    {
        vector3->v[i] = matrix4->m[i + 4 * 0] * x +
                        matrix4->m[i + 4 * 1] * y +
                        matrix4->m[i + 4 * 2] * z +
                        matrix4->m[i + 4 * 3];
    }
----------------------------------------------------------------------------------------------------
*/
}


static void MultiplyMV2(Matrix4* matrix4, float x, float y, Vector2* out_param vector2)
{
	vector2->x = matrix4->m0 * x + matrix4->m4 * y + matrix4->m8 + matrix4->m12;
	vector2->y = matrix4->m1 * x + matrix4->m5 * y + matrix4->m9 + matrix4->m13;

/*
----------------------------------------------------------------------------------------------------
    for (int i = 0; i < 2; i++)
    {
    	vector2->v[i] = matrix4->m[i + 4 * 0] * x +
                        matrix4->m[i + 4 * 1] * y +
                        matrix4->m[i + 4 * 2] +
                        matrix4->m[i + 4 * 3];
    }
----------------------------------------------------------------------------------------------------
*/
}


static float MultiplyMX(Matrix4* matrix4, float x)
{
	return matrix4->m0 * x + matrix4->m8 + matrix4->m12;
}


static float MultiplyMY(Matrix4* matrix4, float y)
{
	return matrix4->m5 * y + matrix4->m9 + matrix4->m13;
}


/*
----------------------------------------------------------------------------------------------------
// matrix4 must be identity matrix

static void RotateM(Matrix4* matrix4, float angle, float x, float y, float z)
{

//  if matrix4 always identity matrix so no need this for init
//  matrix4->m3  = 0.0f;
//  matrix4->m7  = 0.0f;
//  matrix4->m11 = 0.0f;
//  matrix4->m12 = 0.0f;
//  matrix4->m13 = 0.0f;
//  matrix4->m14 = 0.0f;
//  matrix4->m15 = 1.0f;

    angle     = AMathToRadian(angle);
    float s   = sinf(angle);
    float c   = cosf(angle);
    int   dir = ((x != 0.0f) << 2) | ((y != 0.0f) << 1) | (z != 0.0f);

    switch (dir)
	{
    	case 0:
    		break;

    	case 1:
		{ // x y z  0 0 1
    		matrix4->m0  = c;
    		matrix4->m1  = s;
    		matrix4->m2  = 0.0f;

    		matrix4->m4  = -s;
    		matrix4->m5  = c;
    		matrix4->m6  = 0.0f;

    		matrix4->m8  = 0.0f;
    		matrix4->m9  = 0.0f;
    		matrix4->m10 = 1.0f;

    	}
    	break;

    	case 4:
		{ // x y z  1 0 0
        	matrix4->m0  = 1.0f;
            matrix4->m1  = 0.0f;
            matrix4->m2  = 0.0f;

        	matrix4->m4  = 0.0f;
            matrix4->m5  = c;
            matrix4->m6  = s;

            matrix4->m8  = 0.0f;
            matrix4->m9  = -s;
            matrix4->m10 = c;

    	}
    	break;

    	case 2:
		{ // x y z  0 1 0
            matrix4->m0  = c;
            matrix4->m1  = 0.0f;
            matrix4->m2  = -s;

            matrix4->m4  = 0.0f;
            matrix4->m5  = 1.0f;
            matrix4->m6  = 0.0f;

            matrix4->m8  = s;
            matrix4->m9  = 0.0f;
            matrix4->m10 = c;

    	}
    	break;

    	default:
		{
            float len = AVector2Length3(x, y, z);

            if (len != 1.0f)
			{
                float recipLen = 1.0f / len;

                x *= recipLen;
                y *= recipLen;
                z *= recipLen;
            }

            float nc = 1.0f - c;
            float xy = x * y;
            float yz = y * z;
            float zx = z * x;
            float xs = x * s;
            float ys = y * s;
            float zs = z * s;

    		matrix4->m0  = x  * x  * nc + c;
    		matrix4->m1  = xy * nc + zs;
    		matrix4->m2  = zx * nc - ys;

    		matrix4->m4  = xy * nc - zs;
    		matrix4->m5  = y  * y  * nc + c;
    		matrix4->m6  = yz * nc + xs;

    		matrix4->m8  = zx * nc + ys;
    		matrix4->m9  = yz * nc - xs;
    		matrix4->m10 = z  * z  * nc + c;
    	}
    }
}
----------------------------------------------------------------------------------------------------
*/


/**
 * The OpenGL column matrix multiply vector
 * matrix index 048 is matrix column
 * 048 multiply vector means x do vector sum for final vector's x
 */
static void Translate(Matrix4* matrix4, float x, float y, float z)
{
	matrix4->m12 += matrix4->m0 * x + matrix4->m4 * y + matrix4->m8  * z;
	matrix4->m13 += matrix4->m1 * x + matrix4->m5 * y + matrix4->m9  * z;
	matrix4->m14 += matrix4->m2 * x + matrix4->m6 * y + matrix4->m10 * z;

/*
----------------------------------------------------------------------------------------------------
    // always 1
    matrix4->m15 += matrix4->m3 * x + matrix4->m7 * y + matrix4->m11 * z;

	for (int i = 0; i < 4; i++)
	{
		matrix4->m[i + 12] += matrix4->m[i] * x + matrix4->m[i + 4] * y + matrix4->m[i + 8] * z;
	}
----------------------------------------------------------------------------------------------------
*/
}


/**
 * Rotate identity matrix4 and multiply matrix4
 */
static void Rotate(Matrix4* matrix4, float angle, float x, float y, float z)
{
    angle     = AMathToRadian(angle);
    float s   = sinf(angle);
    float c   = cosf(angle);
    float len = AVector2Length3(x, y, z);

    if (len != 1.0f)
    {
        float recipLen = 1.0f / len;

        x *= recipLen;
        y *= recipLen;
        z *= recipLen;
    }

    float nc = 1.0f - c;
    float xy = x * y;
    float yz = y * z;
    float zx = z * x;
    float xs = x * s;
    float ys = y * s;
    float zs = z * s;

	Matrix4 temp1[1] =
	{
        {
            x  * x  * nc + c,
            xy * nc + zs,
            zx * nc - ys,
            0.0f,

            xy * nc - zs,
            y  * y  * nc + c,
            yz * nc + xs,
            0.0f,

            zx * nc + ys,
            yz * nc - xs,
            z  * z  * nc + c,
            0.0f,

            0.0f, 0.0f, 0.0f, 1.0f,
        }
	};

	Matrix4 temp2[1];
	MultiplyMM(matrix4, temp1, temp2);

//	 memcpy(matrix->m, temp2->m, sizeof(Matrix4));
	*matrix4 = *temp2;
}

static void RotateX(Matrix4* matrix4, float angle)
{
    angle   = AMathToRadian(angle);

    float s = sinf(angle);
    float c = cosf(angle);

/*
----------------------------------------------------------------------------------------------------
	Matrix4 temp1[1] =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f,    c,    s, 0.0f,
		0.0f,   -s,    c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	Matrix4 temp2[1];
	MultiplyMM(matrix4, temp1, temp2);

	// memcpy(matrix4->m, temp2->m, sizeof(Matrix4));
	*matrix4 = *temp2;
----------------------------------------------------------------------------------------------------
*/

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


static void RotateY(Matrix4* matrix4, float angle)
{
    angle    = AMathToRadian(angle);
    float s  = sinf(angle);
    float c  = cosf(angle);

/*
----------------------------------------------------------------------------------------------------
	Matrix4 temp1[1] =
	{
		   c, 0.0f,   -s, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		   s, 0.0f,    c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	Matrix4 temp2[1];
	MultiplyMM(matrix4, temp1, temp2);

	// memcpy(matrix4->m, temp2->m, sizeof(Matrix4));
	*matrix4 = *temp2;
----------------------------------------------------------------------------------------------------
*/

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


static void RotateZ(Matrix4* matrix4, float angle)
{
    angle    = AMathToRadian(angle);
    float s  = sinf(angle);
    float c  = cosf(angle);

/*
----------------------------------------------------------------------------------------------------
	Matrix4 temp1[1] =
	{
		   c,    s, 0.0f, 0.0f,
	      -s,    c, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	Matrix4 temp2[1];
	MultiplyMM(matrix4, temp1, temp2);

	// memcpy(matrix4->m, temp2->m, sizeof(Matrix4));
	*matrix4 = *temp2;
----------------------------------------------------------------------------------------------------
*/

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

/*
----------------------------------------------------------------------------------------------------
	for (int i = 0; i < 3; i++)
	{
		matrix4->m[i]     *= sx;
		matrix4->m[i + 4] *= sy;
		matrix4->m[i + 8] *= sz;
	}
----------------------------------------------------------------------------------------------------
*/
}


static void Inverse(Matrix4* matrix4, Matrix4* out_param inverse)
{
    float a0  = matrix4->m0  * matrix4->m5  - matrix4->m1  * matrix4->m4;
    float a1  = matrix4->m0  * matrix4->m6  - matrix4->m2  * matrix4->m4;
    float a2  = matrix4->m0  * matrix4->m7  - matrix4->m3  * matrix4->m4;
    float a3  = matrix4->m1  * matrix4->m6  - matrix4->m2  * matrix4->m5;
    float a4  = matrix4->m1  * matrix4->m7  - matrix4->m3  * matrix4->m5;
    float a5  = matrix4->m2  * matrix4->m7  - matrix4->m3  * matrix4->m6;

    float b0  = matrix4->m8  * matrix4->m13 - matrix4->m9  * matrix4->m12;
    float b1  = matrix4->m8  * matrix4->m14 - matrix4->m10 * matrix4->m12;
    float b2  = matrix4->m8  * matrix4->m15 - matrix4->m11 * matrix4->m12;
    float b3  = matrix4->m9  * matrix4->m14 - matrix4->m10 * matrix4->m13;
    float b4  = matrix4->m9  * matrix4->m15 - matrix4->m11 * matrix4->m13;
    float b5  = matrix4->m10 * matrix4->m15 - matrix4->m11 * matrix4->m14;

/*
----------------------------------------------------------------------------------------------------
    // calculate the determinant.
    float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

    // Close to zero, can't invert.
    if (fabsf(det) < FLT_EPSILON)
    {
    	 return false;
	}

    float scalar = 1.0f / det;
----------------------------------------------------------------------------------------------------
*/

    float scalar = 1.0f / (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);


/*
----------------------------------------------------------------------------------------------------
    support the case where matrix == result
----------------------------------------------------------------------------------------------------
*/

    inverse->m0  = ( matrix4->m5  * b5 - matrix4->m6  * b4 + matrix4->m7  * b3) * scalar;
    inverse->m1  = (-matrix4->m1  * b5 + matrix4->m2  * b4 - matrix4->m3  * b3) * scalar;
    inverse->m2  = ( matrix4->m13 * a5 - matrix4->m14 * a4 + matrix4->m15 * a3) * scalar;
    inverse->m3  = (-matrix4->m9  * a5 + matrix4->m10 * a4 - matrix4->m11 * a3) * scalar;

    inverse->m4  = (-matrix4->m4  * b5 + matrix4->m6  * b2 - matrix4->m7  * b1) * scalar;
    inverse->m5  = ( matrix4->m0  * b5 - matrix4->m2  * b2 + matrix4->m3  * b1) * scalar;
    inverse->m6  = (-matrix4->m12 * a5 + matrix4->m14 * a2 - matrix4->m15 * a1) * scalar;
    inverse->m7  = ( matrix4->m8  * a5 - matrix4->m10 * a2 + matrix4->m11 * a1) * scalar;

    inverse->m8  = ( matrix4->m4  * b4 - matrix4->m5  * b2 + matrix4->m7  * b0) * scalar;
    inverse->m9  = (-matrix4->m0  * b4 + matrix4->m1  * b2 - matrix4->m3  * b0) * scalar;
    inverse->m10 = ( matrix4->m12 * a4 - matrix4->m13 * a2 + matrix4->m15 * a0) * scalar;
    inverse->m11 = (-matrix4->m8  * a4 + matrix4->m9  * a2 - matrix4->m11 * a0) * scalar;

    inverse->m12 = (-matrix4->m4  * b3 + matrix4->m5  * b1 - matrix4->m6  * b0) * scalar;
    inverse->m13 = ( matrix4->m0  * b3 - matrix4->m1  * b1 + matrix4->m2  * b0) * scalar;
    inverse->m14 = (-matrix4->m12 * a3 + matrix4->m13 * a1 - matrix4->m14 * a0) * scalar;
    inverse->m15 = ( matrix4->m8  * a3 - matrix4->m9  * a1 + matrix4->m10 * a0) * scalar;

//    return true;
}


static void Transpose(Matrix4* matrix, Matrix4* out_param transpose)
{
	*transpose = *(Matrix4[])
	{
        {
            matrix->m0, matrix->m4, matrix->m8,  matrix->m12,
            matrix->m1, matrix->m5, matrix->m9,  matrix->m13,
            matrix->m2, matrix->m6, matrix->m10, matrix->m14,
            matrix->m3, matrix->m7, matrix->m11, matrix->m15,
        }
    };
}



/**
 * Transpose first then inverse
 */
static void InverseTranspose(Matrix4* matrix4, Matrix4* out_param inverseTranspose)
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
----------------------------------------------------------------------------------------------------
    // calculate the determinant.
    float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

    // Close to zero, can't invert.
    if (fabsf(det) < FLT_EPSILON)
    {
       return false;
    }

    float scalar = 1.0f / det;
----------------------------------------------------------------------------------------------------
*/

    float scalar = 1.0f / (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);

/*
----------------------------------------------------------------------------------------------------
    support the case where matrix == result
----------------------------------------------------------------------------------------------------
*/

    inverseTranspose->m0  = ( matrix4->m5 * b5 - matrix4->m9  * b4 + matrix4->m13 * b3) * scalar;
    inverseTranspose->m1  = (-matrix4->m4 * b5 + matrix4->m8  * b4 - matrix4->m12 * b3) * scalar;
    inverseTranspose->m2  = ( matrix4->m7 * a5 - matrix4->m11 * a4 + matrix4->m15 * a3) * scalar;
    inverseTranspose->m3  = (-matrix4->m6 * a5 + matrix4->m10 * a4 - matrix4->m14 * a3) * scalar;

    inverseTranspose->m4  = (-matrix4->m1 * b5 + matrix4->m9  * b2 - matrix4->m13 * b1) * scalar;
    inverseTranspose->m5  = ( matrix4->m0 * b5 - matrix4->m8  * b2 + matrix4->m12 * b1) * scalar;
    inverseTranspose->m6  = (-matrix4->m3 * a5 + matrix4->m11 * a2 - matrix4->m15 * a1) * scalar;
    inverseTranspose->m7  = ( matrix4->m2 * a5 - matrix4->m10 * a2 + matrix4->m14 * a1) * scalar;

    inverseTranspose->m8  = ( matrix4->m1 * b4 - matrix4->m5  * b2 + matrix4->m13 * b0) * scalar;
    inverseTranspose->m9  = (-matrix4->m0 * b4 + matrix4->m4  * b2 - matrix4->m12 * b0) * scalar;
    inverseTranspose->m10 = ( matrix4->m3 * a4 - matrix4->m7  * a2 + matrix4->m15 * a0) * scalar;
    inverseTranspose->m11 = (-matrix4->m2 * a4 + matrix4->m6  * a2 - matrix4->m14 * a0) * scalar;

    inverseTranspose->m12 = (-matrix4->m1 * b3 + matrix4->m5  * b1 - matrix4->m9  * b0) * scalar;
    inverseTranspose->m13 = ( matrix4->m0 * b3 - matrix4->m4  * b1 + matrix4->m8  * b0) * scalar;
    inverseTranspose->m14 = (-matrix4->m3 * a3 + matrix4->m7  * a1 - matrix4->m11 * a0) * scalar;
    inverseTranspose->m15 = ( matrix4->m2 * a3 - matrix4->m6  * a1 + matrix4->m10 * a0) * scalar;

//    return true;
}


/**
 * Define a projection matrix in terms of a field of view angle
 * an aspect ratio, and z clip planes
 */
static void Perspective(float fovy, float aspect, float near, float far, Matrix4* out_param projection)
{
    ALogA
    (
        far != near,
        "Perspective error: far == near"
    );

	float f               = 1.0f / (float) tan(fovy * (math_pi / 360.0f));
	float rangeReciprocal = 1.0f / (near - far);

	projection->m0     = f / aspect;
	projection->m1     = 0.0f;
	projection->m2     = 0.0f;
	projection->m3     = 0.0f;

	projection->m4     = 0.0f;
	projection->m5     = f;
	projection->m6     = 0.0f;
	projection->m7     = 0.0f;

	projection->m8     = 0.0f;
	projection->m9     = 0.0f;
	projection->m10    = (far + near) * rangeReciprocal;
	projection->m11    = -1.0f;

	projection->m12    = 0.0f;
	projection->m13    = 0.0f;
	projection->m14    = 2.0f * far * near * rangeReciprocal;
	projection->m15    = 0.0f;
}


static void Ortho(float left, float right, float bottom, float top, float near, float far, Matrix4* out_param projection)
{
    ALogA
	(
		right != left && top != bottom && far != near,
		"Ortho error: right == left || top == bottom || far == near"
	);

    float width        = 1.0f / (right - left);
    float height       = 1.0f / (top   - bottom);
    float depth        = 1.0f / (far   - near);

    projection->m0  = 2.0f * width;  // x
    projection->m1  = 0.0f;
    projection->m2  = 0.0f;
    projection->m3  = 0.0f;

    projection->m4  = 0.0f;
    projection->m5  = 2.0f * height; // y
    projection->m6  = 0.0f;
    projection->m7  = 0.0f;

    projection->m8  = 0.0f;
    projection->m9  = 0.0f;
    projection->m10 = -2.0f * depth; // z
    projection->m11 = 0.0f;

    projection->m12 = -(right + left)   * width;  // tx
    projection->m13 = -(top   + bottom) * height; // ty
    projection->m14 = -(far   + near)   * depth;  // tz
    projection->m15 = 1.0f;
}


/**
 * Define a projection matrix in terms of six clip planes
 */
static void Frustum(float left, float right, float bottom, float top, float near, float far, Matrix4* out_param projection)
{
    ALogA
	(
		right != left && top != bottom && near != far && near > 0.0f && far > 0.0f,
		"Frustum error: right == left || top == bottom || near == far || near <= 0.0f || far <= 0.0f"
	);

    float width        = 1.0f / (right - left);
    float height       = 1.0f / (top - bottom);
    float depth        = 1.0f / (near - far);

    projection->m0  = 2.0f * (near * width);  // x
    projection->m1  = 0.0f;
    projection->m2  = 0.0f;
    projection->m3  = 0.0f;

    projection->m4  = 0.0f;
    projection->m5  = 2.0f * (near * height); // y
    projection->m6  = 0.0f;
    projection->m7  = 0.0f;

    projection->m8  = 2.0f * ((right + left) * width); // A
    projection->m9  = (top + bottom) * height;         // B
    projection->m10 = (far + near)   * depth;          // C
    projection->m11 = -1.0f;

    projection->m12 = 0.0f;
    projection->m13 = 0.0f;
    projection->m14 = 2.0f * (far * near * depth);     // D
    projection->m15 = 0.0f;
}



static void LookAt
(
    float    eyeX,     float eyeY,    float eyeZ,
    float    centerX,  float centerY, float centerZ,
    float    upX,      float upY,     float upZ,
    Matrix4* out_param view
)
{

	// see the OpenGL GLUT documentation for gluLookAt for a description
	// of the algorithm. We implement it in a straightforward way

	float fx   = centerX - eyeX;
	float fy   = centerY - eyeY;
	float fz   = centerZ - eyeZ;

	// normalize f
	float rlf  = AVector2Normalize3(fx, fy, fz);
	fx        *= rlf;
	fy        *= rlf;
	fz        *= rlf;

	// compute s = f x up (x means "cross product")
	float sx  = fy * upZ - fz * upY;
	float sy  = fz * upX - fx * upZ;
	float sz  = fx * upY - fy * upX;

	// and normalize s
	float rls = AVector2Normalize3(sx, sy, sz);
	sx       *= rls;
	sy       *= rls;
	sz       *= rls;

	// compute u = s x f
	float ux  = sy * fz - sz * fy;
	float uy  = sz * fx - sx * fz;
	float uz  = sx * fy - sy * fx;

	view->m0  = sx;
	view->m1  = ux;
	view->m2  = -fx;
	view->m3  = 0.0f;

	view->m4  = sy;
	view->m5  = uy;
	view->m6  = -fy;
	view->m7  = 0.0f;

	view->m8  = sz;
	view->m9  = uz;
	view->m10 = -fz;
	view->m11 = 0.0f;

	view->m12 = 0.0f;
	view->m13 = 0.0f;
	view->m14 = 0.0f;
	view->m15 = 1.0f;

	Translate(view, -eyeX, -eyeY, -eyeZ);
}


struct AMatrix AMatrix[1] =
{
	MultiplyMM,
	MultiplyMMM,
	MultiplyMV4,
	multiplyMV3,
	MultiplyMV2,
	MultiplyMX,
	MultiplyMY,

	Translate,
	Rotate,
	RotateX,
	RotateY,
	RotateZ,
	Scale,
    Inverse,
    Transpose,
    InverseTranspose,
	Ortho,
	Frustum,
    Perspective,
	LookAt,
};
