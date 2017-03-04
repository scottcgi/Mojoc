/*
 *
 *
 *  Created on: 2014-2-26
 *  Author: scott.cgi
 */

#ifndef shader_primitive_h
#define shader_primitive_h

#include "Mojoc/Graphics/OpenGL/Head/gl3.h"
#include "Mojoc/Toolkit/Math/Matrix.h"
#include "Mojoc/Toolkit/Math/Math.h"

#include "Mojoc/Graphics/Draw/Color.h"


struct AShaderPrimitive
{
	GLuint program;
	GLint  attribPosition;
	GLint  uniformMVPMatrix;
	GLint  uniformColor;
	GLint  uniformPointSize;

	void (*Use) (Matrix4* mvpMatrix, Color* color, GLint pointSize);
	void (*Init)();
};

extern struct AShaderPrimitive AShaderPrimitive[1];

#endif
