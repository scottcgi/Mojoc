/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2013-4-17
 * Update   : 2019-1-24
 * Author   : scott.cgi
 */


#include <string.h>
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Graphics/OpenGL/GLInfo.h"
#include "Engine/Toolkit/Platform/Log.h"


static void Init()
{
    const char* version = (char*) glGetString(GL_VERSION);

    ALog_D
    (
        "vendor = %s \n renderer = %s \n version = %s \n extensions = %s",
         glGetString(GL_VENDOR),
         glGetString(GL_RENDERER),
         version,
         glGetString(GL_EXTENSIONS)
    );

    if(strstr(version, "OpenGL ES 3.") != NULL)
    {
        AGLInfo->version = 3.0f;
    }
    else
    {
        AGLInfo->version = 2.0f;
    }


    glGetFloatv  (GL_ALIASED_POINT_SIZE_RANGE,          AGLInfo->pointSizeRange);
    glGetFloatv  (GL_ALIASED_LINE_WIDTH_RANGE,          AGLInfo->lineWidthRange);

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,               &AGLInfo->maxVertexAttribs);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS,       &AGLInfo->maxVertexUniformVectors);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS,     &AGLInfo->maxFragmentUniformVectors);
    glGetIntegerv(GL_MAX_VARYING_VECTORS,              &AGLInfo->maxVaryingVectors);
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,   &AGLInfo->maxVertexTextureImageUnits);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &AGLInfo->maxCombinedTextureImageUnits);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,          &AGLInfo->maxTextureImageUnits);
}


struct AGLInfo AGLInfo[1] =
{{
    .Init = Init
}};
