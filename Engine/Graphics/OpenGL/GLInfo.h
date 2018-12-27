/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-4-17
 * Author: scott.cgi
 */

#ifndef GL_INFO_H
#define GL_INFO_H


#include <stdbool.h>
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/HeaderUtils/Define.h"


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

    void (*Init)();
};


extern struct AGLInfo AGLInfo[1];


#endif
