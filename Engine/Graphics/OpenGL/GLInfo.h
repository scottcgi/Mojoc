/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2013-4-17
 * Update   : 2019-1-14
 * Author   : scott.cgi
 */


#ifndef GL_INFO_H
#define GL_INFO_H


#include <stdbool.h>
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/HeaderUtils/Define.h"


/**
 * OpenGL some information.
 */
struct AGLInfo
{
    GLfloat pointSizeRange[2];
    GLfloat lineWidthRange[2];

    GLint   maxVertexAttribs;
    GLint   maxVertexUniformVectors;
    GLint   maxVertexTextureImageUnits;
    GLint   maxFragmentUniformVectors;
    GLint   maxVaryingVectors;
    GLint   maxCombinedTextureImageUnits;
    GLint   maxTextureImageUnits;

    float   version;

    void (*Init)(void);
};


extern struct AGLInfo AGLInfo[1];


#endif
