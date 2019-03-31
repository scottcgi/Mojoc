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
 * Since : 2013-4-23
 * Update: 2019-1-24
 * Author: scott.cgi
 */


#ifndef SHADER_SPRITE_H
#define SHADER_SPRITE_H


#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Graphics/Draw/Color.h"


/**
 * Shader control for Sprite.
 */
struct AShaderSprite
{
    GLuint program;
    GLint  attribPositionTexcoord;
    GLint  uniformSampler2D;
    GLint  uniformMVPMatrix;
    GLint  uniformColor;

    void (*Use) (Matrix4* mvpMatrix, Color* color);
    void (*Init)(void);
};


extern struct AShaderSprite AShaderSprite[1];


#endif
