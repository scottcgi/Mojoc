/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License.
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub.
 * This Mojoc game engine is a serious project with extreme code pursuit, and will continue to iterate.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2015-4-20
 * Update : 2019-1-23
 * Author : scott.cgi
 */


#ifndef SHADER_H
#define SHADER_H


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
