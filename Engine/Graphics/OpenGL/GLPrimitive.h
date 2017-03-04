/*
 *
 *
 *  Created on: 2014-2-26
 *      Author: scott.cgi
 */


#ifndef gl_primitive_h
#define gl_primitive_h

#include "Mojoc/Graphics/OpenGL/Head/gl3.h"

#include "Mojoc/Toolkit/Math/Matrix.h"
#include "Mojoc/Toolkit/Math/Math.h"

#include "Mojoc/Toolkit/Define/Rect.h"
#include "Mojoc/Graphics/Draw/Color.h"
#include "Mojoc/Toolkit/Utils/Array.h"


struct AGLPrimitive
{
	/** Default 1.0f */
	GLfloat  pointSize;

	/** Default 1.0f */
	GLfloat  lineWidth;

	/** Default {1.0f, 1.0f, 1.0f, 1.0f} */
	Color    color[1];

	/** Default NULL */
	Matrix4* modelMatrix;

	/**
	 * Each point pair of x, y if model NULL will use identity matrix
	 */
    void (*DrawPoints) (Array(float)* pointArr);

	/**
	 * Each vertex pair of x, y if model NULL will use identity matrix
	 */
    void (*DrawPolygon)(Array(float)* vertexArr);

    /**
     * Each vertex pair of x, y if model NULL will use identity matrix
     */
    void (*DrawLines)  (Array(float)* vertexArr);

    /**
     * If model NULL will use identity matrix
     */
    void (*DrawRect)   (Rect* rect);
};

extern struct AGLPrimitive AGLPrimitive[1];

#endif
