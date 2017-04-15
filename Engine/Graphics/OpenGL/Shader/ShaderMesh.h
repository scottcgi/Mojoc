/*
 *
 *
 *  Created on: 2016-8-13
 *  Author: scott.cgi
 */

#ifndef shader_mesh_h
#define shader_mesh_h

#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"

struct AShaderMesh
{
	GLuint get_only  program;
	GLuint get_only  attribPosition;
	GLuint get_only  attribTexcoord;
	GLuint get_only  attribOpacity;
	GLuint get_only  attribRGB;

	GLint  get_only  uniformMVPMatrix;
	GLint  get_only  uniformSample2D;

	void (*Use) (Matrix4* mvpMatrix);
	void (*Init)();
};

extern struct AShaderMesh AShaderMesh[1];

#endif
