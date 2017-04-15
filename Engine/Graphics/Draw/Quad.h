/*
 *
 *
 *  Created on: 2012-12-25
 *  Author: scott.cgi
 */

#ifndef quad_h
#define quad_h

#include "Engine/Graphics/OpenGL/MeshDef.h"
#include "Engine/Toolkit/Utils/Array.h"
#include "Engine/Graphics/OpenGL/Texture.h"


/**
 * Rect in texture
 * use openGL world coordinate system
 */
typedef struct
{
	float get_only width;
	float get_only height;

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
	void  (*Init)           (float width, float height, Quad* out_param quad);

	/**
	 * Max size of all quads
	 */
	void (*MaxSize)         (Array(Quad)* quadArr, float* out_param width, float* out_param height);

	/**
	 * Get Quad 4 vertices data into vertexData[quad_vertex_num]
	 */
	void (*GetQuadVertex)   (Quad* quad, Texture* texture, float out_param vertexData[quad_vertex_num]);

	/**
	 * Get Quad 4 vertices position data into bornPositionData[quad_position3_num]
	 */
	void (*GetQuadPosition3)(Quad* quad, float out_param bornPositionData[quad_position3_num]);

	/**
	 * Get Quad 4 vertices uv data into uvData[quad_uv_num]
	 */
	void (*GetQuadUV)       (Quad* quad, Texture* texture, float out_param uvData[quad_uv_num]);

	/**
	 * Get Quad 4 vertices index data into indexData[quad_index_num]
	 */
	void (*GetQuadIndex)    (int vertexNumBefore, short out_param indexData[quad_index_num]);
};

extern struct AQuad AQuad[1];

#endif
