/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2012-12-25
 * Author: scott.cgi
 */

#ifndef QUAD_H
#define QUAD_H


#include "Engine/Toolkit/Utils/Array.h"
#include "Engine/Graphics/OpenGL/Texture.h"


enum
{
    /**
     * Two triangles (012, 230) has 6 vertices (2 vertices repeats).
     */
    Quad_IndexNum      = 6,
    Quad_IndexSize     = Quad_IndexNum     * sizeof(short),

    /**
     *  3 (x, y, z) * 4 (vertices)
     */
    Quad_Position3Num  = 12,
    Quad_Position3Size = Quad_Position3Num * sizeof(float),

    /**
     * 2 (u, v) * 4 (vertices)
     */
    Quad_UVNum         = 8,
    Quad_UVSize        = Quad_UVNum        * sizeof(float),

    /**
     * 1 * 4 (vertices)
     */
    Quad_OpacityNum    = 4,
    Quad_OpacitySize   = Quad_OpacityNum   * sizeof(float),

    /**
     * 3 (r, g ,b) * 4 (vertices)
     */
    Quad_RGBNum        = 12,
    Quad_RGBSize       = Quad_RGBNum       * sizeof(float),

    /**
     * 2 (x, y) * 4 (vertices)
     */
    Quad_Position2Num  = 8,

    Quad_VertexNum     = Quad_Position2Num + Quad_UVNum,
    Quad_VertexSize    = Quad_VertexNum    * sizeof(float),
};


/**
 * Rect in texture
 */
typedef struct
{
    /**
     * The width in openGL coordinate
     */
    float width;

    /**
     * The height in openGL coordinate
     */
    float height;

    /**
     * The top left is (0, 0)
     * top left x in texture
     * Default 0
     *
     * because texture width and height is openGL coordinate
     * offsetTextureX must be openGL coordinate
     */
    float offsetTextureX;

    /**
     * The top left is (0, 0)
     * top left y in texture
     * Default 0
     *
     * because texture width and height is openGL coordinate
     * offsetTextureY must be openGL coordinate
     */
    float offsetTextureY;

    /**
     * The top left x offset the center
     * default Quad center borned in center of openGL coordinate
     * so default value is -width / 2
     */
    float offsetCenterX;
    
    /**
     * The top left y offset the center
     * default Quad center borned in center of openGL coordinate
     * so default value is height / 2
     */
    float offsetCenterY;
}
Quad;


struct AQuad
{
    Quad* (*Create)         (float width, float height);
    void  (*Init)           (float width, float height, Quad* outQuad);

    /**
     * Max size of all quads
     */
    void (*MaxSize)         (Array(Quad)* quadArr, float* outWidth, float* outHeight);

    /**
     * Get Quad 4 vertices data into vertexData[Quad_VertexNum]
     */
    void (*GetQuadVertex)   (Quad* quad, Texture* texture, float outVertexData[Quad_VertexNum]);

    /**
     * Get Quad 4 vertices position data into bornPositionData[Quad_Position3Num]
     */
    void (*GetQuadPosition3)(Quad* quad, float outBornPositionData[Quad_Position3Num]);

    /**
     * Get Quad 4 vertices uv data into uvData[Quad_UVNum]
     */
    void (*GetQuadUV)       (Quad* quad, Texture* texture, float outUVvData[Quad_UVNum]);

    /**
     * Get Quad 4 vertices index data into indexData[Quad_IndexNum]
     */
    void (*GetQuadIndex)    (int vertexNumBefore, short outIndexData[Quad_IndexNum]);
};


extern struct AQuad AQuad[1];


#endif
