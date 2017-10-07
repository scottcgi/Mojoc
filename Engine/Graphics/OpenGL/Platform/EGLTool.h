/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-3-12
 * Author : scott.cgi
 * Version: 0.0.0
 */

#ifndef EGL_TOOL_H
#define EGL_TOOL_H


#include "Engine/Toolkit/Platform/Platform.h"


//----------------------------------------------------------------------------------------------------------------------
#ifdef IS_PLATFORM_ANDROID
//----------------------------------------------------------------------------------------------------------------------


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


//----------------------------------------------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------------------------------------------


#endif
