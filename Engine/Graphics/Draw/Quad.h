/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2012-12-25
 * Author : scott.cgi
 * Version: 0.1
 */


#ifndef QUAD_H
#define QUAD_H


#include "Engine/Graphics/OpenGL/MeshDefine.h"
#include "Engine/Toolkit/Utils/Array.h"
#include "Engine/Graphics/OpenGL/Texture.h"


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
