/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-4-17
 * Author : scott.cgi
 * Version: 0.1
 */


#include <string.h>
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Graphics/OpenGL/GLInfo.h"
#include "Engine/Toolkit/Platform/Log.h"


static void Init()
{
    char* vendor     = (char*) glGetString(GL_VENDOR);
    char* renderer   = (char*) glGetString(GL_RENDERER);
    char* version    = (char*) glGetString(GL_VERSION);
    char* extensions = (char*) glGetString(GL_EXTENSIONS);

    ALog_D
	(
		"vendor = %s \n renderer = %s \n version = %s \n extensions = %s",
    	 vendor, renderer, version, extensions
	);

    if(strstr(version, "OpenGL ES 3.") != NULL)
    {
    	AGLInfo->version = 3.0f;
    }
    else
    {
    	AGLInfo->version = 2.0f;
    }
    
    glGetFloatv  (GL_ALIASED_POINT_SIZE_RANGE,           AGLInfo->pointSizeRange);
    glGetFloatv  (GL_ALIASED_LINE_WIDTH_RANGE,           AGLInfo->lineWidthRange);

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,                &AGLInfo->maxVertexAttribs);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS,        &AGLInfo->maxVertexUniformVectors);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS,      &AGLInfo->maxFragmentUniformVectors);
    glGetIntegerv(GL_MAX_VARYING_VECTORS,               &AGLInfo->maxVaryingVectors);
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,    &AGLInfo->maxVertexTextureImageUnits);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,  &AGLInfo->maxCombinedTextureImageUnits);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,           &AGLInfo->maxTextureImageUnits);
}


struct AGLInfo AGLInfo[1] =
{
	{
		.Init = Init
	}
};
