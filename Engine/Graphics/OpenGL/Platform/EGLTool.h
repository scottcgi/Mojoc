/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-3-12
 * Update: 2019-2-23
 * Author: scott.cgi
 */


#ifndef EGL_TOOL_H
#define EGL_TOOL_H


#include "Engine/Toolkit/Platform/Platform.h"


#ifdef IS_PLATFORM_ANDROID


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


#endif


#endif
