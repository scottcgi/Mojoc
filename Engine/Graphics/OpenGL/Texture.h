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
 * Since    : 2012-12-29
 * Update   : 2019-1-19
 * Author   : scott.cgi
 */


#ifndef TEXTURE_H
#define TEXTURE_H


#include "Engine/Graphics/OpenGL/Platform/gl2.h"
#include "Engine/Toolkit/HeaderUtils/Define.h"


typedef struct
{
    /**
     *  The openGL generated texture id.
     */
    GLuint id;

    /**
     * The texture width.
     */
    float   width;

    /**
     * The texture height.
     */
    float   height;

    /**
     * All texture cache in ArrayStrMap by filePath.
     */
    const char* filePath;
}
Texture;


/**
 * Manage and control Texture.
 */
struct ATexture
{
    /**
     * Get Texture by resourceFilePath, not found will create one.
     *
     * resourceFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    Texture*  (*Get)    (const char* resourceFilePath);

    /**
     * Destroy texture memory both in GPU and CPU, and removed from cache.
     *
     * important: after Destroy the texture will be invalidated.
     */
    void      (*Destroy)(Texture* texture);
};


extern struct ATexture ATexture[1];


#endif
