/*
 *
 *
 *  Created on: 2013-4-23
 *  Author: scott.cgi
 */

#ifndef shader_sprite_h
#define shader_sprite_h

#include "Mojoc/Graphics/OpenGL/Head/gl3.h"
#include "Mojoc/Toolkit/Math/Matrix.h"
#include "Mojoc/Toolkit/Math/Math.h"

#include "Mojoc/Graphics/Draw/Color.h"

struct AShaderSprite
{
	GLuint program;
	GLuint attribPositionTexcoord;
	GLint  uniformSampler2D;
	GLint  uniformMVPMatrix;
	GLint  uniformColor;

	void (*Use) (Matrix4* mvpMatrix, Color* color);
	void (*Init)();
};

extern struct AShaderSprite AShaderSprite[1];


#endif
