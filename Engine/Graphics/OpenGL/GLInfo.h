/*
 *
 *
 *  Created on: 2013-4-17
 *      Author: scott.cgi
 */

#ifndef gl_info_h
#define gl_info_h


#include <stdbool.h>
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Head/MacroDefine.h"


struct AGLInfo
{
	GLfloat get_only pointSizeRange[2];
	GLfloat get_only lineWidthRange[2];

	GLint   get_only maxVertexAttribs;
	GLint   get_only maxVertexUniformVectors;
	GLint   get_only maxVertexTextureImageUnits;
	GLint   get_only maxFragmentUniformVectors;
	GLint   get_only maxVaryingVectors;
	GLint   get_only maxCombinedTextureImageUnits;
	GLint   get_only maxTextureImageUnits;

	bool    get_only isSupportVAO;
	float   get_only version;

	void (*Init)();
};


extern struct AGLInfo AGLInfo[1];


#endif
