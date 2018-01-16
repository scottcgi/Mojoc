/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2015-4-20
 * Author: scott.cgi
 */

#ifndef SHADER_H
#define SHADER_H


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
