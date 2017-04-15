/*
 * Mesh.h
 *
 *  Created on: 2016-8-5
 *      Author: scott.cgi
 */

#ifndef mesh_h
#define mesh_h


#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Graphics/Draw/Quad.h"
#include "Engine/Graphics/OpenGL/Texture.h"
#include "Engine/Toolkit/Utils/Array.h"
#include "Engine/Toolkit/Utils/ArrayQueue.h"
#include "Engine/Graphics/OpenGL/MeshDef.h"


/**
 * If use VBO update to buffer
 */
typedef struct
{
	GLenum        target;
	GLintptr      offset;
	GLsizeiptr    length;
	const GLvoid* data;
}
VBOSubData;


struct Mesh
{
	/**
	 * Draw from index, default first in children
	 */
	int                              fromIndex;

	/**
	 * Draw to index, default last in children
	 */
	int                              toIndex;

	/**
	 * Pair of from and to index SubMesh to draw
	 * if no range will default draw fromIndex to toIndex
	 */
	ArrayQueue(int)        get_only  drawRangeQueue[1];

	/**
	 * Sometimes use fixed index for get child in children
	 * so can not change children order instead of use SubMesh index for sorting
	 */
	ArrayList(SubMesh*)    get_only  children      [1];

	Drawable                         drawable      [1];
	Texture*                         texture;

//--------------------------------------------------------------------------------------------------

    /**
     * All vertex data, every vertex has position, uv, color
     */
	Array(float)*          get_only   vertexArr;

	/**
	 * All vertex index data
	 */
	Array(short)*          get_only   indexArr;

	/**
	 * If use VBO is array buffer id
	 */
	GLuint                 get_only   vboIds[mesh_buffer_num];

    /**
     * If use VAO is generated id else 0
     */
    GLuint                 get_only   vaoId;

	/**
	 * If use VBO update to buffer
	 */
	ArrayList(VBOSubData)  get_only   vboSubDataList[1];

//--------------------------------------------------------------------------------------------------

    int                    get_only   uvDataOffset;
	int                    get_only   rgbDataOffset;
	int                    get_only   opacityDataOffset;
    int                    get_only   vertexCountOffset;

    int                    get_only   positionDataLength;
    int                    get_only   uvDataLength;
    int                    get_only   rgbDataLength;
    int                    get_only   opacityDataLength;
    int                    get_only   indexDataLength;
};


struct AMesh
{
	Mesh*     (*Create)            (Texture* texture);
	void      (*Init)              (Texture* texture,  Mesh* out_param mesh);
	void      (*InitWithCapacity)  (Texture* texture,  int capacity, Mesh* out_param mesh);

	/**
	 * Call Mesh member's Release and free all SubMesh memory
	 */
	void      (*Release)           (Mesh* mesh);

	/**
	 * Clear children, drawRangeQueue, bufferSubDataList,
	 * ready to GenerateBuffer
	 */
	void      (*Clear)             (Mesh* mesh);

	/**
	 * The positionArr(has x y z) uvArr and indexArr will copy in SubMesh
	 * SubMesh set parent and index with Mesh and free by parent Release
	 */
	SubMesh*  (*AddChildWithData)  (Mesh* mesh, Array(float)* positionArr, Array(float)* uvArr, Array(short)* indexArr);

	/**
	 * SubMesh data calculate by Quad in Texture
	 * SubMesh set parent and index with spriteBatch and free by parent Release
	 */
	SubMesh*  (*AddChildWithQuad)  (Mesh* mesh, Quad* quad);

	/**
	 * Reorder all SubMesh draw order by index, not changed pos in children
	 */
	void      (*ReorderChildren)   (Mesh* mesh);

	/**
	 * Combine SubMesh data into buffer when draw function upload to openGL
	 * when SubMesh initialization, remove or add, need call this for generate buffer
	 */
	void      (*GenerateBuffer)    (Mesh* mesh);

	/**
	 * Mesh implement Drawable's render
	 */
	void      (*Render)            (Drawable* drawable);
};


extern struct AMesh AMesh[1];


static inline void AMeshDraw(Mesh* mesh)
{
	ADrawable->Draw(mesh->drawable);
}


static inline void AMeshSetIncrease(Mesh* mesh, int increase)
{
	mesh->children->increase = increase;
}


/**
 * Push startIndex and endIndex into drawRangeQueue
 */
static inline void AMeshPushDrawRange(Mesh* mesh, int startIndex, int endIndex)
{
    AArrayQueuePush(mesh->drawRangeQueue, startIndex);
    AArrayQueuePush(mesh->drawRangeQueue, endIndex);
}


#endif
