/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-3-12
 * Author : scott.cgi
 * Version: 0.1
 */


#include "Engine/Toolkit/Platform/Platform.h"


//--------------------------------------------------------------------------------------------------
#ifdef IS_PLATFORM_ANDROID
//--------------------------------------------------------------------------------------------------


#include "Engine/Graphics/OpenGL/Platform/EGLTool.h"
#include "Engine/Toolkit/Platform/Log.h"


void CreateEGL(EGLNativeWindowType window, EGLDisplay* outDisplay, EGLContext* outContext, EGLSurface* outSurface, EGLConfig*  outConfig)
{

    *outDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (*outDisplay == EGL_NO_DISPLAY)
    {
        ALog_A(false, "CreateEGL failed EGL unable to eglGetDisplay");
    }

    if (!eglInitialize(*outDisplay, NULL/*major*/, NULL/*minor*/))
    {
        ALog_A(false, "CreateEGL failed EGL unable to eglInitialize");
    }

    EGLint numConfigs;

    // here specify the attributes of the desired configuration.
    // below, we select an EGLConfig with at least 8 bits per color
    // component compatible with on-screen windows
    EGLint configAttribs[] =
    {
        EGL_DEPTH_SIZE,      16,
        EGL_RED_SIZE,        8,   //   5,
        EGL_GREEN_SIZE,      8,   //   6,
        EGL_BLUE_SIZE,       8,   //   5,
        EGL_ALPHA_SIZE,      8,   //   0,
        EGL_STENCIL_SIZE,    8,
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    // here, the application chooses the configuration it desires. In this
    // sample, we have a very simplified selection process, where we pick
    // the first EGLConfig that matches our criteria
    eglChooseConfig(*outDisplay, configAttribs, outConfig, 1, &numConfigs);

    if (numConfigs < 1)
    {
        ALog_A(false, "CreateEGL failed no config match eglChooseConfig");
    }

    EGLint surfaceAttrs[] =
    {
        EGL_RENDER_BUFFER,
        EGL_BACK_BUFFER,
        EGL_NONE
    };

    *outSurface = eglCreateWindowSurface(*outDisplay, *outConfig, window, surfaceAttrs);
    if (*outSurface == EGL_NO_SURFACE)
    {
        ALog_A(false, "CreateEGL failed EGL unable to eglCreateWindowSurface");
    }

    EGLint contextAttribs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    *outContext = eglCreateContext(*outDisplay, *outConfig, EGL_NO_CONTEXT, contextAttribs);
    if (*outContext == EGL_NO_CONTEXT)
    {
        ALog_A(false, "CreateEGL failed EGL unable to eglCreateContext");
    }

    if (!eglMakeCurrent(*outDisplay, *outSurface, *outSurface, *outContext))
    {
        ALog_A(false, "CreateEGL failed EGL unable to eglMakeCurrent");
    }

//	eglSwapInterval(*outDisplay, 0);
}

void DestroyEGL(EGLDisplay* display, EGLContext* context, EGLSurface* surface)
{
    if (*display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(*display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (*context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(*display, *context);
        }

        if (*surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(*display, *surface);
        }

        eglTerminate(*display);
    }

    *display = EGL_NO_DISPLAY;
    *context = EGL_NO_CONTEXT;
    *surface = EGL_NO_SURFACE;
}


static void ResetSurface(EGLNativeWindowType window, EGLDisplay display, EGLContext context, EGLConfig config, EGLSurface* surface)
{
    if (*surface != EGL_NO_SURFACE)
    {
        eglDestroySurface(display, *surface);
    }

    EGLint surfaceAttrs[] =
    {
        EGL_RENDER_BUFFER,
        EGL_BACK_BUFFER,
        EGL_NONE
    };

    *surface = eglCreateWindowSurface(display, config, window, surfaceAttrs);

    if (eglMakeCurrent(display, *surface, *surface, context) == false)
    {
        ALog_A(false, "ResetSurface failed EGL unable to eglMakeCurrent");
    }
}


struct AEGLTool AEGLTool[1] =
{
    CreateEGL,
    DestroyEGL,
    ResetSurface,
};


//--------------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------
