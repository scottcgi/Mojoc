/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2013-3-12
 * Author : scott.cgi
 */

#ifndef egl_tool_h
#define egl_tool_h


#include "Engine/Toolkit/Platform/Platform.h"

//--------------------------------------------------------------------------------------------------
#ifdef is_platform_android
//--------------------------------------------------------------------------------------------------

#include <stdbool.h>
#include "Engine/Graphics/OpenGL/Platform/egl.h"


struct AEGLTool
{
    /**
     * Creates an EGL rendering context and all associated elements
     */
    void (*CreateEGL)   (EGLNativeWindowType window, EGLDisplay* outDisplay, EGLContext* outContext, EGLSurface* outSurface, EGLConfig*  outConfig);

    /**
     * Destroy EGL context and all associated elements
     */
    void (*DestroyEGL)  (EGLDisplay* display, EGLContext* context, EGLSurface* surface);


    /**
     * When native window resized, we need reset surface
     * the surface will destroyed and create new one and make current
     */
    void (*ResetSurface)(EGLNativeWindowType window, EGLDisplay display, EGLContext context, EGLConfig config, EGLSurface* surface);
};

extern struct AEGLTool AEGLTool[1];

//--------------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------


#endif
