/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2012-12-22
 * Author : scott.cgi
 * Version: 0.0.0
 */

#ifndef GL_TOOL_H
#define GL_TOOL_H


#include <stdbool.h>
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Graphics/OpenGL/Texture.h"
#include "Engine/Toolkit/Head/Define.h"


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
    void   (*SetSize)              (int width, int height);

    /**
     * Compile a shader
     * shaderType is GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
     */
    GLuint (*LoadShader)           (GLenum shaderType, char* shaderSource);

    /**
     * Load vertex and fragment shader, create a program object, link program
     */
    GLuint (*LoadProgram)          (char* vertexSource, char* fragmentSource);

    /**
     * Create a program object by shader file path
     */
    GLuint (*LoadProgramByFile)    (char* vertexShaderPath, char* fragmentShaderPath);

    /**
     * Load texture from file
     */
    void (*LoadTexture)            (char* filePath, Texture* outTexture);
};


extern struct AGLTool AGLTool[1];


//----------------------------------------------------------------------------------------------------------------------


/**
 * Convert screen width to openGL width
 * same as (screenWidth) * AGLTool->screenRatio / AGLTool->screenHalfWidth
 */
static inline float AGLTool_ToGLWidth(float screenWidth)
{
    return screenWidth * AGLTool->ratioDivideHalfWidth;
}


/**
 * Convert pixel height to openGL height
 */
static inline float AGLTool_ToGLHeight(float screenHeight)
{
    return screenHeight / AGLTool->screenHalfHeight;
}


/**
 * Convert screen x to openGL x
 */
static inline float AGLTool_ToGLX(float screenX)
{
    return AGLTool_ToGLWidth(screenX) - AGLTool->screenRatio;
}


/**
 * Convert screen Y to openGL Y
 */
static inline float AGLTool_ToGLY(float screenY)
{
    return 1.0f - AGLTool_ToGLHeight(screenY);
}


/**
 * Convert openGL Width to screen width
 * same as (glWidth) * AGLTool->screenHalfWidth / AGLTool->screenRatio
 */
static inline float AGLTool_ToScreenWidth(float glWidth)
{
    return glWidth * AGLTool->halfWidthDivideRatio;
}


/**
 * Convert openGL Height to screen height
 */
static inline float AGLTool_ToScreenHeight(float glHeight)
{
    return glHeight * AGLTool->screenHalfHeight;
}


/**
 * Convert openGL x to screen x
 */
static inline float AGLTool_ToScreenX(float glX)
{
    return     AGLTool_ToScreenWidth(glX + AGLTool->screenRatio);
}


/**
 * Convert openGL y to screen y
 */
static inline float AGLTool_ToScreenY(float glY)
{
    return AGLTool_ToScreenHeight(1.0f - glY);
}


/**
 * Convert width to uv width
 * parameters both openGL or screen coordinate
 */
static inline float AGLTool_ToUVWidth(float width, float textureWidth)
{
    return width / textureWidth;
}


/**
 * Convert height to uv height
 * parameters both openGL or screen coordinate
 */
static inline float AGLTool_ToUVHeight(float height, float textureHeight)
{
    return height / textureHeight;
}


#endif
