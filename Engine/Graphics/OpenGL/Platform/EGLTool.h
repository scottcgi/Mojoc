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
 * Since    : 2013-3-12
 * Update   : 2019-2-23
 * Author   : scott.cgi
 */


#ifndef EGL_TOOL_H
#define EGL_TOOL_H


#include "Engine/Toolkit/Platform/Platform.h"


//------------------------
#ifdef IS_PLATFORM_ANDROID
//------------------------


#include <stdbool.h>
#include "Engine/Graphics/OpenGL/Platform/egl.h"


struct AEGLTool
{
    /**
     * Creates an EGL rendering context and all associated elements.
     */
    void (*CreateEGL)   (
                            EGLNativeWindowType window,
                            EGLDisplay*         outDisplay,
                            EGLContext*         outContext,
                            EGLSurface*         outSurface,
                            EGLConfig*          outConfig
                        );

    /**
     * Destroy EGL context and all associated elements.
     */
    void (*DestroyEGL)  (EGLDisplay* display, EGLContext* context, EGLSurface* surface);


    /**
     * When native window resized, we need reset surface,
     * the surface will destroyed and create new one and make current.
     */
    void (*ResetSurface)(
                            EGLNativeWindowType window,
                            EGLDisplay          display,
                            EGLContext          context,
                            EGLConfig           config,
                            EGLSurface*         surface
                        );
};


extern struct AEGLTool AEGLTool[1];


//---------------------------
#endif // IS_PLATFORM_ANDROID
//---------------------------


#endif
