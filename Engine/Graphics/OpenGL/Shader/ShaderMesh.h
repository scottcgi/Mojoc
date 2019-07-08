/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2016-8-13
 * Update : 2019-1-24
 * Author : scott.cgi
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
    void (*Init)(void);
};


extern struct AShaderMesh AShaderMesh[1];


#endif
