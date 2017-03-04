/*
 *
 *
 *  Created on: 2012-12-22
 *  Author: scott.cgi
 */

#include <string.h>
#include <stdbool.h>

#include "Mojoc/Toolkit/Platform/Log.h"
#include "Mojoc/Graphics/OpenGL/GLTool.h"
#include "Mojoc/Toolkit/Platform/File.h"
#include "Mojoc/Graphics/Utils/Image.h"

static void SetSize(int width, int height)
{
	AGLTool->screenWidth           = (float) width;
	AGLTool->screenHeight          = (float) height;
	AGLTool->screenRatio           = (float) width  / (float) height;
	AGLTool->screenHalfWidth       = (float) width  / 2;
	AGLTool->screenHalfHeight      = (float) height / 2;

	AGLTool->ratioDivideHalfWidth  = AGLTool->screenRatio     / AGLTool->screenHalfWidth;
	AGLTool->halfWidthDivideRatio  = AGLTool->screenHalfWidth / AGLTool->screenRatio;
}

static void CheckGLError(const char* printMsg)
{
    for (GLint error = glGetError(); error; error = glGetError())
    {
        ALogE("after %s() glError (0x%x)", printMsg, error);
    }
}

static void PrintGLString(const char *name, GLenum s)
{
    ALogD("GL %s = %s", name, (const char *) glGetString(s));
}


static GLuint LoadShader(GLenum shaderType, const char* shaderSource)
{
	// create the shader object
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		CheckGLError("glCreateShader");
		return shader;
	}

	// load the shader source
	glShaderSource(shader, 1, &shaderSource, NULL);

	// compile the shader
	glCompileShader(shader);

	GLint compiled;

	// check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_FALSE)
	{
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 0)
		{
			char buf[infoLen];
			glGetShaderInfoLog(shader, infoLen, NULL, buf);
			ALogE("could not compile shader %d:%s", shaderType, buf);

			glDeleteShader(shader);

			return 0;
		}
	}

    return shader;
}

static GLuint LoadProgram(const char* vertexSource, const char* fragmentSource)
{
	GLuint program;

	// load the vertex shaders
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vertexSource);

	if (vertexShader == 0)
	{
		return 0;
	}

	// load the fragment shaders
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentSource);

	if (fragmentShader == 0)
	{
		return 0;
	}

	// create the program object
	program = glCreateProgram();

	if (program == 0)
	{
		CheckGLError("glCreateProgram");
		return 0;
	}

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// link the program
	glLinkProgram(program);

	GLint linkStatus;

	// check the link status
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE)
	{
		GLint bufLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);

		if (bufLength > 0)
		{
			char buf[bufLength];
			glGetProgramInfoLog(program, bufLength, NULL, buf);
			ALogE("could not link program: %s", buf);
		}

		glDeleteProgram(program);

		return 0;
	}

	// free up no longer needed shader resources
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

GLuint LoadProgramByFile(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	const char* vSource = AFile->ReadString(vertexShaderPath);
	const char* fSource = AFile->ReadString(fragmentShaderPath);
	GLuint      program = AGLTool->LoadProgram(vSource, fSource);

	free((void*) vSource);
	free((void*) fSource);

	return program;
}

static void LoadTexture(const char* filePath, Texture* texture)
{
	 GLuint textureId;

	 // use tightly packed data
	 // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	 // Generate a texture object
	 glGenTextures(1, &textureId);

	 // Bind to the texture in OpenGL
	 glBindTexture(GL_TEXTURE_2D, textureId);

	 texture->id = textureId;

	 // set filtering
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);


	 float width;
	 float height;

	 void* pixels = AImage->CreatePixelDataFromPng(filePath, &width, &height);
	 ALogA(pixels != NULL, "LoadTexture failed, no pixls data");

	 // load the data into the bound texture
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	 texture->width  = AGLToolToGLWidth (width);
	 texture->height = AGLToolToGLHeight(height);

	 free(pixels);
}

void CreateEGL(EGLNativeWindowType window, EGLDisplay* outDisplay, EGLContext* outContext, EGLSurface* outSurface, EGLConfig*  outConfig)
{

	*outDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	if (*outDisplay == EGL_NO_DISPLAY)
	{
		ALogA(false, "CreateEGL failed EGL unable to eglGetDisplay");
	}

	if (!eglInitialize(*outDisplay, NULL/*major*/, NULL/*minor*/))
	{
		ALogA(false, "CreateEGL failed EGL unable to eglInitialize");
	}

	EGLint numConfigs;

	// here specify the attributes of the desired configuration.
	// below, we select an EGLConfig with at least 8 bits per color
	// component compatible with on-screen windows
	const EGLint configAttribs[] =
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
		ALogA(false, "CreateEGL failed no config match eglChooseConfig");
	}

	const EGLint surfaceAttrs[] =
	{
		EGL_RENDER_BUFFER,
		EGL_BACK_BUFFER,
		EGL_NONE
	};

	*outSurface = eglCreateWindowSurface(*outDisplay, *outConfig, window, surfaceAttrs);
	if (*outSurface == EGL_NO_SURFACE)
	{
		ALogA(false, "CreateEGL failed EGL unable to eglCreateWindowSurface");
	}

	const EGLint contextAttribs[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	*outContext = eglCreateContext(*outDisplay, *outConfig, EGL_NO_CONTEXT, contextAttribs);
	if (*outContext == EGL_NO_CONTEXT)
	{
		ALogA(false, "CreateEGL failed EGL unable to eglCreateContext");
	}

	if (!eglMakeCurrent(*outDisplay, *outSurface, *outSurface, *outContext))
	{
		ALogA(false, "CreateEGL failed EGL unable to eglMakeCurrent");
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

	const EGLint surfaceAttrs[] =
	{
		EGL_RENDER_BUFFER,
		EGL_BACK_BUFFER,
		EGL_NONE
	};

	*surface = eglCreateWindowSurface(display, config, window, surfaceAttrs);

	if (eglMakeCurrent(display, *surface, *surface, context) == false)
	{
		ALogA(false, "ResetSurface failed EGL unable to eglMakeCurrent");
	}
}

struct AGLTool AGLTool[1] =
{
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	SetSize,

	CheckGLError,
	PrintGLString,

	LoadShader,
	LoadProgram,

	LoadProgramByFile,
	LoadTexture,

	CreateEGL,
    DestroyEGL,
	ResetSurface,
};

