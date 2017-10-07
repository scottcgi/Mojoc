/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2016-8-13
 * Author : scott.cgi
 * Version: 0.0.0
 */

#ifndef SHADER_MESH_H
#define SHADER_MESH_H


#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"


struct AShaderMesh
{
    GLuint program;
    GLuint attribPosition;
    GLuint attribTexcoord;
    GLuint attribOpacity;
    GLuint attribRGB;

    GLint  uniformMVPMatrix;
    GLint  uniformSample2D;

    void (*Use) (Matrix4* mvpMatrix);
    void (*Init)();
};


extern struct AShaderMesh AShaderMesh[1];


#endif
