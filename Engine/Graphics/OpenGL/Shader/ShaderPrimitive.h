/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The project Mojoc is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2014-2-26
 * Update: 2019-1-24
 * Author: scott.cgi
 */


#ifndef SHADER_PRIMITIVE_H
#define SHADER_PRIMITIVE_H


#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Graphics/Draw/Color.h"


/**
 * Shader control for GLPrimitive.
 */
struct AShaderPrimitive
{
    GLuint program;
    GLint  attribPosition;
    GLint  uniformMVPMatrix;
    GLint  uniformColor;
    GLint  uniformPointSize;

    void (*Use) (Matrix4* mvpMatrix, Color* color, GLfloat pointSize);
    void (*Init)(void);
};


extern struct AShaderPrimitive AShaderPrimitive[1];


#endif
