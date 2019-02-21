/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2016-8-13
 * Update: 2019-1-24
 * Author: scott.cgi
 */


#ifndef SHADER_MESH_H
#define SHADER_MESH_H


#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"


/**
 * Shader control for Mesh.
 */
struct AShaderMesh
{
    GLuint program;
    GLint  attribPosition;
    GLint  attribTexcoord;
    GLint  attribOpacity;
    GLint  attribRGB;

    GLint  uniformMVPMatrix;
    GLint  uniformSample2D;

    void (*Use) (Matrix4* mvpMatrix);
    void (*Init)();
};


extern struct AShaderMesh AShaderMesh[1];


#endif
