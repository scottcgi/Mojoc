/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2013-4-20
 * Update : 2019-1-21
 * Author : scott.cgi
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
    Sprite_VertexPositionNum    = 2,

    /**
     * One vertex has u, v.
     */
    Sprite_VertexUVNum          = 2,

    /**
     * One vertex size.
     */
    Sprite_VertexNum            = Sprite_VertexPositionNum    + Sprite_VertexUVNum,

    /**
     * 2 (x, y) * 4 (sizeof float)
     */
    Sprite_VertexPositionStride = Sprite_VertexPositionNum    * sizeof(float),

    /**
     * 2 (u, v) * 4 (sizeof float)
     */
    Sprite_VertexUVStride       = Sprite_VertexUVNum          * sizeof(float),

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
     * The uv width in texture.
     */
    float         uvWidth;

    /**
     * The uv height in texture.
     */
    float         uvHeight;

    /**
     * If use VBO is NULL else buffer all vertex data.
     * data model: [x, y, u, v...x, y, u, v...]
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

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Whether vertexArr has been deformed by ASprite Deform function.
     * auto reset by Render function.
     */
    bool          isDeformed;


    /**
     * All vertices index count.
     */
    int           indexCount;

    /**
     * The vertex bytes data size.
     */
    int           vertexDataSize;
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
    void    (*InitWithFile)       (const char* resourceFilePath, Sprite* outSprite);

    Sprite* (*CreateWithQuad)     (Texture* texture, Quad* quad);
    void    (*InitWithQuad)       (Texture* texture, Quad* quad, Sprite* outSprite);


    Sprite* (*CreateWithQuadArray)(Texture* texture, Array(Quad)* quadArr);
    void    (*InitWithQuadArray)  (Texture* texture, Array(Quad)* quadArr, Sprite* outSprite);

    void    (*Release)            (Sprite*  sprite);

    /**
     * Deform Sprite vertex position and uv.
     * the positionDeformArr will add each position (x y).
     * the uvDeformArr will add each uv.
     *
     * the Sprite consists of quads,
     * and each quad vertices order is from left-top counterclockwise to right-top.
     *
     * positionDeformArr: if NULL will not deform.
     *                    the length must equals vertex positions number (the half of Sprite vertexArr length).
     *
     * uvDeformArr     : if NULL will not deform.
     *                   the length must equals vertex uvs number (the half of Sprite vertexArr length).
     */
    void    (*Deform)             (Sprite* sprite, Array(float)* positionDeformArr, Array(float)* uvDeformArr);


    /**
     * Same as Deform function but deform position and uv by index.
     *
     * indexArr: the index of position and uv array, and each vertex position or uv will deform.
     *           the positionDeformArr or uvDeformArr length must equals indexArr length.
     *
     */
    void    (*DeformByIndex)      (
                                      Sprite*       sprite,
                                      Array(float)* positionDeformArr,
                                      Array(float)* uvDeformArr,
                                      Array(int)*   indexArr
                                  );

    /**
     * The implementation of Drawable's render function for render Sprite.
     */
    void    (*Render)             (Drawable* drawable);
};


extern struct ASprite ASprite[1];


/**
 * Draw Sprite.
 */
static inline void ASprite_Draw(Sprite* sprite)
{
    // indirect call ASprite->Render
    ADrawable->Draw(sprite->drawable);
}


#endif
