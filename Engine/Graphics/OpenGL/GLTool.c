/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2012-12-22
 * Author : scott.cgi
 */

#include <string.h>
#include <stdlib.h>

#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Toolkit/Utils/FileTool.h"
#include "Engine/Graphics/Utils/Image.h"


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
	const char* vSource = AFileTool->ReadStringPlatform(vertexShaderPath);
	const char* fSource = AFileTool->ReadStringPlatform(fragmentShaderPath);
	GLuint      program = AGLTool  ->LoadProgram       (vSource, fSource);

	free((void*) vSource);
	free((void*) fSource);

	return program;
}


static void LoadTexture(const char* filePath, Texture* outTexture)
{
	 GLuint textureId;

	 // use tightly packed data
	 // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	 // generate a outTexture object
	 glGenTextures(1, &textureId);

	 // bind to the outTexture in OpenGL
	 glBindTexture(GL_TEXTURE_2D, textureId);

	 outTexture->id = textureId;

	 // set filtering
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);


	 float width;
	 float height;

	 void* pixels = AImage->CreatePixelDataFromPng(filePath, &width, &height);
	 ALogA(pixels != NULL, "LoadTexture failed, no pixls data");

	 // load the data into the bound outTexture
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	 outTexture->width  = AGLToolToGLWidth (width);
	 outTexture->height = AGLToolToGLHeight(height);

	 free(pixels);
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
};

