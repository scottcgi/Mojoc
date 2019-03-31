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
 * Update: 2019-1-24
 * Author: scott.cgi
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
