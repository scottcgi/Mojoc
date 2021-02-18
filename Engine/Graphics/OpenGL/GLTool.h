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
 * Since    : 2012-12-22
 * Update   : 2019-1-23
 * Author   : scott.cgi
 */


#ifndef GL_TOOL_H
#define GL_TOOL_H


#include <stdbool.h>
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Graphics/OpenGL/Texture.h"
#include "Engine/Toolkit/HeaderUtils/Define.h"


struct AGLTool
{
    /**
     * Screen pixel width.
     */
    float screenWidth;

    /**
     * Screen pixel heigh.t
     */
    float screenHeight;

    /**
     * Screen half pixel width.
     */
    float screenHalfWidth;

    /**
     * Screen half pixel height.
     */
    float screenHalfHeight;

    /**
     * Screen (width / height).
     */
    float screenRatio;

    /**
     * Screen (ratio / halfWidth).
     */
    float ratioDivideHalfWidth;

    /**
     * Screen (halfWidth / ratio).
     */
    float halfWidthDivideRatio;

    /**
     * When screen rect changed called.
     */
    void   (*SetSize)            (int width, int height);

    /**
     * Load and compile shader from shaderSourceStr.
     * shaderType: GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
     *
     * if success return shader id else 0.
     */
    GLuint (*LoadShader)         (GLenum shaderType, const char* shaderSourceStr);

    /**
     * Load vertex and fragment shader, and create program object, link program object.
     */
    GLuint (*LoadProgram)        (const char* vertexSourceStr, const char* fragmentSourceStr);

    /**
     * Create a program object from shader file path.
     *
     * vertexShaderFilePath and fragmentShaderFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    GLuint (*LoadProgramFromFile)(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);

    /**
     * Load texture from textureFilePath.
     * 
     * textureFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    void (*LoadTexture)          (const char* textureFilePath, Texture* outTexture);
};


extern struct AGLTool AGLTool[1];


//----------------------------------------------------------------------------------------------------------------------


/**
 * Convert screen pixel width to openGL width.
 * same as (screenWidth) * AGLTool->screenRatio / AGLTool->screenHalfWidth
 */
static inline float AGLTool_ToGLWidth(float screenWidth)
{
    return screenWidth * AGLTool->ratioDivideHalfWidth;
}


/**
 * Convert screen pixel height to openGL height.
 */
static inline float AGLTool_ToGLHeight(float screenHeight)
{
    return screenHeight / AGLTool->screenHalfHeight;
}


/**
 * Convert screen pixel x to openGL x.
 */
static inline float AGLTool_ToGLX(float screenX)
{
    return AGLTool_ToGLWidth(screenX) - AGLTool->screenRatio;
}


/**
 * Convert screen pixel y to openGL y.
 */
static inline float AGLTool_ToGLY(float screenY)
{
    return 1.0f - AGLTool_ToGLHeight(screenY);
}


/**
 * Convert openGL Width to screen pixel width.
 * same as (glWidth) * AGLTool->screenHalfWidth / AGLTool->screenRatio
 */
static inline float AGLTool_ToScreenWidth(float glWidth)
{
    return glWidth * AGLTool->halfWidthDivideRatio;
}


/**
 * Convert openGL Height to screen screen pixel.
 */
static inline float AGLTool_ToScreenHeight(float glHeight)
{
    return glHeight * AGLTool->screenHalfHeight;
}


/**
 * Convert openGL x to screen pixel x.
 */
static inline float AGLTool_ToScreenX(float glX)
{
    return AGLTool_ToScreenWidth(glX + AGLTool->screenRatio);
}


/**
 * Convert openGL y to screen pixel y.
 */
static inline float AGLTool_ToScreenY(float glY)
{
    return AGLTool_ToScreenHeight(1.0f - glY);
}


/**
 * Convert width to uv width.
 * parameters both openGL or pixel coordinate.
 */
static inline float AGLTool_ToUVWidth(float width, float textureWidth)
{
    return width / textureWidth;
}


/**
 * Convert height to uv height.
 * parameters both openGL or pixel coordinate.
 */
static inline float AGLTool_ToUVHeight(float height, float textureHeight)
{
    return height / textureHeight;
}


#endif
