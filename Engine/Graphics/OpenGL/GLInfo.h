/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-4-17
 * Update: 2019-1-14
 * Author: scott.cgi
 */


#ifndef GL_INFO_H
#define GL_INFO_H


#include <stdbool.h>
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/HeaderUtils/Define.h"


/**
 * OpenGL infos.
 */
struct AGLInfo
{
    GLfloat pointSizeRange[2];
    GLfloat lineWidthRange[2];

    GLint  maxVertexAttribs;
    GLint  maxVertexUniformVectors;
    GLint  maxVertexTextureImageUnits;
    GLint  maxFragmentUniformVectors;
    GLint  maxVaryingVectors;
    GLint  maxCombinedTextureImageUnits;
    GLint  maxTextureImageUnits;

    float   version;

    void (*Init)();
};


extern struct AGLInfo AGLInfo[1];


#endif
