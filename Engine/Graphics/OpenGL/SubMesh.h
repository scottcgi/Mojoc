/*
 * SubMesh.h
 *
 *  Created on: 2016-8-7
 *      Author: scott.cgi
 */

#ifndef sub_mesh_h
#define sub_mesh_h

#include "Mojoc/Graphics/OpenGL/MeshDef.h"
#include "Mojoc/Graphics/Draw/Drawable.h"
#include "Mojoc/Graphics/Draw/Quad.h"

struct SubMesh
{
    /**
     * if regenerate, the drawable parent must visible
     * or parent property will lost
     */
	Drawable      drawable   [1];

	/**
	 * SubMesh coordinate under parent Mesh drawable matrix
	 * if SubMesh drawable has own parent, equivalent to the parent under Mesh drawable matrix
	 * so let parent Mesh coordinate equal world space will make SubMesh own parent coordinate understandable
	 */
    Mesh*         parent;

	/**
	 * Index in Mesh for Mesh ReorderChildren
	 */
	int           index;

//--------------------------------------------------------------------------------------------------

	Array(float*) positionArr[1];
	Array(float*) uvArr      [1];

	/** Careful 4 byte aligned */
	Array(short*) indexArr   [1];

//--------------------------------------------------------------------------------------------------

	int           vertexCount;
	int           indexOffset;

	int           positionDataOffset;
	int           uvDataOffset;
	int           rgbDataOffset;
	int           opacityDataOffset;
	int           indexDataOffset;
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
