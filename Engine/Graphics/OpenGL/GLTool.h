/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2012-12-22
 * Author : scott.cgi
 */

#ifndef gl_tool_h
#define gl_tool_h


#include <stdbool.h>
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Graphics/OpenGL/Texture.h"
#include "Engine/Toolkit/Head/MacroDefine.h"


struct AGLTool
{
	/**
	 * Screen pixel width
	 */
	float screenWidth;

	/**
	 * Screen pixel height
	 */
	float screenHeight;

	/**
	 * Screen half pixel width
	 */
	float screenHalfWidth;

	/**
	 * Screen half pixel height
	 */
	float screenHalfHeight;

	/**
	 * Screen width / height
	 */
	float screenRatio;

	/**
	 * Screen ratio / halfWidth
	 */
	float ratioDivideHalfWidth;

	/**
	 * Screen halfWidth / ratio
	 */
	float halfWidthDivideRatio;

	/**
	 * When screen changed called
	 */
	void (*SetSize)                (int width, int height);

	/**
	 * Check and print GL error
	 */
	void (*CheckGLError)           (const char* printMsg);

	/**
	 * Print GL string value
	 */
	void (*PrintGLString)          (const char *name, GLenum s);

	/**
	 * Compile a shader
	 * shaderType is GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	 */
	GLuint (*LoadShader)           (GLenum shaderType, const char* shaderSource);

	/**
	 * Load vertex and fragment shader, create a program object, link program
	 */
	GLuint (*LoadProgram)          (const char* vertexSource, const char* fragmentSource);

	/**
	 * Create a program object by shader file path
	 */
	GLuint (*LoadProgramByFile)    (const char* vertexShaderPath, const char* fragmentShaderPath);

	/**
	 * Load texture from file
	 */
	void (*LoadTexture)            (const char* filePath, Texture* outTexture);
};


extern struct AGLTool AGLTool[1];


//--------------------------------------------------------------------------------------------------


/**
 * Convert screen width to openGL width
 * same as (screenWidth) * AGLTool->screenRatio / AGLTool->screenHalfWidth
 */
static inline float AGLToolToGLWidth(float screenWidth)
{
	return screenWidth * AGLTool->ratioDivideHalfWidth;
}


/**
 * Convert pixel height to openGL height
 */
static inline float AGLToolToGLHeight(float screenHeight)
{
	return screenHeight / AGLTool->screenHalfHeight;
}


/**
 * Convert screen x to openGL x
 */
static inline float AGLToolToGLX(float screenX)
{
	return AGLToolToGLWidth(screenX) - AGLTool->screenRatio;
}


/**
 * Convert screen Y to openGL Y
 */
static inline float AGLToolToGLY(float screenY)
{
	return 1.0f - AGLToolToGLHeight(screenY);
}


/**
 * Convert openGL Width to screen width
 * same as (glWidth) * AGLTool->screenHalfWidth / AGLTool->screenRatio
 */
static inline float AGLToolToScreenWidth(float glWidth)
{
	return glWidth * AGLTool->halfWidthDivideRatio;
}


/**
 * Convert openGL Height to screen height
 */
static inline float AGLToolToScreenHeight(float glHeight)
{
	return glHeight * AGLTool->screenHalfHeight;
}


/**
 * Convert openGL x to screen x
 */
static inline float AGLToolToScreenX(float glX)
{
	return 	AGLToolToScreenWidth(glX + AGLTool->screenRatio);
}


/**
 * Convert openGL y to screen y
 */
static inline float AGLToolToScreenY(float glY)
{
	return AGLToolToScreenHeight(1.0f - glY);
}


/**
 * Convert width to uv width
 * parameters both openGL or screen coordinate
 */
static inline float AGLToolToUVWidth(float width, float textureWidth)
{
	return width / textureWidth;
}


/**
 * Convert height to uv height
 * parameters both openGL or screen coordinate
 */
static inline float AGLToolToUVHeight(float height, float textureHeight)
{
	return height / textureHeight;
}


#endif
