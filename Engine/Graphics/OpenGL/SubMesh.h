/*
 * SubMesh.h
 *
 *  Created on: 2016-8-7
 *      Author: scott.cgi
 */

#ifndef sub_mesh_h
#define sub_mesh_h


#include "Engine/Graphics/OpenGL/MeshDef.h"
#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Graphics/Draw/Quad.h"


struct SubMesh
{
    /**
     * if regenerate, the drawable parent must visible
     * or parent property will lost
     */
	Drawable              drawable[1];

	/**
	 * SubMesh coordinate under parent Mesh drawable matrix
	 * if SubMesh drawable has own parent, equivalent to the parent under Mesh drawable matrix
	 * so let parent Mesh coordinate equal world space will make SubMesh own parent coordinate understandable
	 */
    Mesh*         get_only parent;

	/**
	 * Index in Mesh for Mesh ReorderChildren
	 */
	int           get_only index;

//--------------------------------------------------------------------------------------------------

	Array(float*) get_only positionArr[1];
	Array(float*) get_only uvArr      [1];

	/**
	 * Careful 4 byte aligned
	 */
	Array(short*) get_only indexArr   [1];

//--------------------------------------------------------------------------------------------------

	int           get_only vertexCount;
	int           get_only indexOffset;

	int           get_only positionDataOffset;
	int           get_only uvDataOffset;
	int           get_only rgbDataOffset;
	int           get_only opacityDataOffset;
	int           get_only indexDataOffset;
};


struct ASubMesh
{
	/**
	 * The positionArr(has x y z) uvArr and indexArr will copy in
	 */
	SubMesh* (*CreateWithData)(Array(float)* positionArr, Array(float)* uvArr, Array(short)* indexArr);

	/**
	 * The positionArr uvArr and indexArr will calculate by Quad in Texture
	 */
	SubMesh* (*CreateWithQuad)(Texture* texture, Quad* quad);

	/**
	 * Update SubMesh data by Quad
	 */
	void     (*SetWithQuad)   (SubMesh* subMesh, Texture* texture, Quad* quad);
};


extern struct ASubMesh ASubMesh[1];


#endif
