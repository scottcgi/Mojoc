/*
 *
 *
 *  Created on: 2013-1-10
 *  Author: scott.cgi
 */

#ifndef camera_h
#define camera_h


#include "Mojoc/Toolkit/Math/Matrix.h"
#include "Mojoc/Toolkit/Math/Math.h"


struct ACamera
{
	float left;
	float right;
	float bottom;

	float top;
	float near;
	float far;

	/** Camera position */
	float eyeX;
	float eyeY;
	float eyeZ;

	/** We are looking toward the distance */
	float lookX;
	float lookY;
	float lookZ;

	/** up vector control camera direction, perpendicular to the camera plane */
	float upX;
	float upY;
	float upZ;
	
	/** Store view projection matrix */
	Matrix4 vp        [1];
	/** Store the projection matrix */
	Matrix4 projection[1];
	/** Store the view matrix */
	Matrix4 view      [1];

	void  (*SetOrtho)     ();
	void  (*SetFrustum)   ();
	void  (*SetLookAt)    ();

	/** the ZOrder most near camera */
	float (*GetNearZOrder)();
	/** the ZOrder most far camer */
	float (*GetFarZOrder) ();
};

extern struct ACamera ACamera[1];

#endif
