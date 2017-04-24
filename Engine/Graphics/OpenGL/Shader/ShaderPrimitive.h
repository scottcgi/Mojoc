/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2014-2-26
 * Author: scott.cgi
 */
#ifndef shader_primitive_h
#define shader_primitive_h


#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Graphics/Draw/Color.h"


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
