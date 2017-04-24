/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2016-8-13
 * Author: scott.cgi
 */

#ifndef shader_mesh_h
#define shader_mesh_h


#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"


struct AShaderMesh
{
	GLuint program;
	GLuint attribPosition;
	GLuint attribTexcoord;
	GLuint attribOpacity;
	GLuint attribRGB;

	GLint  uniformMVPMatrix;
	GLint  uniformSample2D;

	void (*Use) (Matrix4* mvpMatrix);
	void (*Init)();
};


extern struct AShaderMesh AShaderMesh[1];


#endif
