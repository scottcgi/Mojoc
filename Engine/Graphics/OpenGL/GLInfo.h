/*
 *
 *
 *  Created on: 2013-4-17
 *      Author: scott.cgi
 */

#ifndef gl_info_h
#define gl_info_h

#include <stdbool.h>

#include "Engine/Graphics/OpenGL/Head/gl3.h"

struct AGLInfo
{
	GLfloat pointSizeRange[2];
	GLfloat lineWidthRange[2];

	GLint   maxVertexAttribs;
	GLint   maxVertexUniformVectors;
	GLint   maxVertexTextureImageUnits;
	GLint   maxFragmentUniformVectors;
	GLint   maxVaryingVectors;
	GLint   maxCombinedTextureImageUnits;
	GLint   maxTextureImageUnits;

	bool    isSupportVAO;
	float   version;

	void (*Init)();
};

extern struct AGLInfo AGLInfo[1];

#endif
