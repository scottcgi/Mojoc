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
 * The Mojoc game engine is a serious project with extreme code pursuit,
 * and will continue to iterate.
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
