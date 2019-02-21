/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
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
    void (*Init)();
};


extern struct AShaderSprite AShaderSprite[1];


#endif
