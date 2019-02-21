/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-4-20
 * Update: 2019-1-21
 * Author: scott.cgi
 */


#ifndef SPRITE_H
#define SPRITE_H


#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Graphics/Draw/Quad.h"
#include "Engine/Graphics/OpenGL/Texture.h"
#include "Engine/Toolkit/Utils/Array.h"


enum
{
    /**
     * Number of buffers.
     */
     Sprite_BufferNum           = 2,

    /**
     * Index of buffer vertex.
     */
     Sprite_BufferVertex        = 0,
     
    /**
     * Index of buffer index.
     */
     Sprite_BufferIndex         = 1,

    /**
     * One vertex position has x, y.
     */
    Sprite_VertexPositionSize   = 2,

    /**
     * One vertex has u, v.
     */
    Sprite_VertexUVSize         = 2,

    /**
     * One vertex size.
     */
    Sprite_VertexSize           = Sprite_VertexPositionSize + Sprite_VertexUVSize,

    /**
     * 2 (x, y) * 4 (sizeof float)
     */
    Sprite_VertexPositionStride = 8,

    /**
     * 2 (u, v) * 4 (sizeof float)
     */
    Sprite_VertexUVStride       = 8,

    /**
     * One vertex stride.
     */
    Sprite_VertexStride         = Sprite_VertexPositionStride + Sprite_VertexUVStride,
};


/**
 * Render with texture by vertices.
 * implement Drawable's render for render self.
 */
typedef struct
{
    /**
     * The base class for provide draw functions.
     */
    Drawable      drawable[1];

    /**
     * Render texture.
     */
    Texture*      texture;

    /**
     * All vertices index count.
     */
    int           indexCount;

    /**
     * If use VBO is NULL else buffer all vertex data.
     * data model: [all position data | all uv data]
     */
    Array(float)*  vertexArr;

    /**
     * If use VBO is NULL else buffer all index data.
     */
    Array(short)* indexArr;

    /**
     * If use VBO is the generated VBO ids else 0.
     */
    GLuint        vboIds[Sprite_BufferNum];

    /**
     * If use VAO is the generated vao id else 0.
     */
    GLuint        vaoId;
}
Sprite;


/**
 * Control Sprite.
 */
struct ASprite
{
    Sprite* (*Create)             (Texture*    texture);
    void    (*Init)               (Texture*    texture,  Sprite* outSprite);

    /**
     * Create Sprite by resourceFilePath.
     *
     * resourceFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    Sprite* (*CreateWithFile)     (const char* resourceFilePath);

    /**
     * Init Sprite by resourceFilePath.
     *
     * resourceFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    void    (*InitWithFile)       (const char* resourceFilePath,  Sprite* outSprite);

    Sprite* (*CreateWithQuad)     (Texture*    texture,  Quad*   quad);
    void    (*InitWithQuad)       (Texture*    texture,  Quad*   quad, Sprite* outSprite);


    Sprite* (*CreateWithQuadArray)(Texture*    texture, Array(Quad)* quadArr);
    void    (*InitWithQuadArray)  (Texture*    texture, Array(Quad)* quadArr, Sprite* outSprite);

    void    (*Release)            (Sprite*     sprite);

    /**
     * The Sprite Drawable's render function implementation for render self.
     */
    void    (*Render)             (Drawable*   drawable);
};


extern struct ASprite ASprite[1];


/**
 * Sprite draw self.
 */
static inline void ASprite_Draw(Sprite* sprite)
{
    // indirect call ASprite->Render
    ADrawable->Draw(sprite->drawable);
}


#endif
