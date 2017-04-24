/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2013-4-17
 * Author: scott.cgi
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

    ALogD
	(
		"vendor = %s \n renderer = %s \n version = %s \n extensions = %s",
    	 vendor, renderer, version, extensions
	);

    if (extensions)
    {
    	AGLInfo->isSupportVAO = strstr(extensions, "vertex_array_object") != NULL;
    }
    ALogD("\n isSupportVAO = %d", AGLInfo->isSupportVAO);


    if(strstr(version, "OpenGL ES 3."))
    {
    	AGLInfo->version = 3.0f;
    }
    else
    {
    	AGLInfo->version = 2.0f;
    }
    ALogD("\n version = %f", AGLInfo->version);

    glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, AGLInfo->pointSizeRange);
    ALogD("\n GL_ALIASED_POINT_SIZE_RANGE[%f, %f]", AGLInfo->pointSizeRange[0], AGLInfo->pointSizeRange[1]);
    
    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, AGLInfo->lineWidthRange);
    ALogD("\n GL_ALIASED_LINE_WIDTH_RANGE[%f, %f]", AGLInfo->lineWidthRange[0], AGLInfo->lineWidthRange[1]);
    
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &AGLInfo->maxVertexAttribs);
    ALogD("\n GL_MAX_VERTEX_ATTRIBS = %d", AGLInfo->maxVertexAttribs);
    
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &AGLInfo->maxVertexUniformVectors);
    ALogD("\n GL_MAX_VERTEX_UNIFORM_VECTORS = %d", AGLInfo->maxVertexUniformVectors);
    
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &AGLInfo->maxFragmentUniformVectors);
    ALogD("\n GL_MAX_FRAGMENT_UNIFORM_VECTORS = %d", AGLInfo->maxFragmentUniformVectors);
    
    glGetIntegerv(GL_MAX_VARYING_VECTORS, &AGLInfo->maxVaryingVectors);
    ALogD("\n GL_MAX_VARYING_VECTORS = %d", AGLInfo->maxVaryingVectors);
    
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &AGLInfo->maxVertexTextureImageUnits);
    ALogD("\n GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = %d", AGLInfo->maxVertexTextureImageUnits);
    
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &AGLInfo->maxCombinedTextureImageUnits);
    ALogD("\n GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = %d", AGLInfo->maxCombinedTextureImageUnits);

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &AGLInfo->maxTextureImageUnits);
    ALogD("\n GL_MAX_TEXTURE_IMAGE_UNITS = %d", AGLInfo->maxTextureImageUnits);
}


struct AGLInfo AGLInfo[1] =
{
	{
		.Init = Init
	}
};
