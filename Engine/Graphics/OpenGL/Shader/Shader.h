/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2015-4-20
 * Update: 2019-1-23
 * Author: scott.cgi
 */


#ifndef SHADER_H
#define  SHADER_H


#include "Engine/Graphics/OpenGL/Platform/gl3.h"


/**
 * Control shader.
 */
struct AShader
{
    /**
     * Record current using shader's program.
     */
    GLuint program;
};


extern struct AShader AShader[1];


#endif
