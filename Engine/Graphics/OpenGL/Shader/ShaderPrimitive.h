/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2014-2-26
 * Author : scott.cgi
 * Version: 0.0.0
 */


#ifndef SHADER_PRIMITIVE_H
#define SHADER_PRIMITIVE_H


#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Graphics/Draw/Color.h"


struct AShaderPrimitive
{
    GLuint program;
    GLint  attribPosition;
    GLint  uniformMVPMatrix;
    GLint  uniformColor;
    GLint  uniformPointSize;

    void (*Use) (Matrix4* mvpMatrix, Color* color, GLint pointSize);
    void (*Init)();
};


extern struct AShaderPrimitive AShaderPrimitive[1];


#endif
