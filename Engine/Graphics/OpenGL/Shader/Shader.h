/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2015-4-20
 * Author : scott.cgi
 */

#ifndef shader_h
#define shader_h


#include "Engine/Graphics/OpenGL/Platform/gl3.h"


/**
 * Record current using shader info
 */
struct AShader
{
	GLuint program;
};


extern struct AShader AShader[1];


#endif
