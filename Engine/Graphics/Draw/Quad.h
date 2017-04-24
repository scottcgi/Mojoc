/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2012-12-25
 * Author: scott.cgi
 */

#ifndef quad_h
#define quad_h


#include "Engine/Graphics/OpenGL/MeshDef.h"
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
	 * Get Quad 4 vertices data into vertexData[quad_vertex_num]
	 */
	void (*GetQuadVertex)   (Quad* quad, Texture* texture, float outVertexData[quad_vertex_num]);

	/**
	 * Get Quad 4 vertices position data into bornPositionData[quad_position3_num]
	 */
	void (*GetQuadPosition3)(Quad* quad, float outBornPositionData[quad_position3_num]);

	/**
	 * Get Quad 4 vertices uv data into uvData[quad_uv_num]
	 */
	void (*GetQuadUV)       (Quad* quad, Texture* texture, float outUVvData[quad_uv_num]);

	/**
	 * Get Quad 4 vertices index data into indexData[quad_index_num]
	 */
	void (*GetQuadIndex)    (int vertexNumBefore, short outIndexData[quad_index_num]);
};

extern struct AQuad AQuad[1];

#endif
