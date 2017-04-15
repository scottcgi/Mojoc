/*
 *
 *
 *  Created on: 2013-4-23
 *  Author: scott.cgi
 */

#ifndef shader_sprite_h
#define shader_sprite_h

#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Graphics/Draw/Color.h"

struct AShaderSprite
{
	GLuint get_only program;
	GLuint get_only attribPositionTexcoord;
	GLint  get_only uniformSampler2D;
	GLint  get_only uniformMVPMatrix;
	GLint  get_only uniformColor;

	void (*Use) (Matrix4* mvpMatrix, Color* color);
	void (*Init)();
};

extern struct AShaderSprite AShaderSprite[1];

#endif
