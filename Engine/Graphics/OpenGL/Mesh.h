/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2016-8-5
 * Update: 2019-1-22
 * Author: scott.cgi
 */


#ifndef MESH_H
#define MESH_H


#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Graphics/Draw/Quad.h"
#include "Engine/Graphics/OpenGL/Texture.h"
#include "Engine/Toolkit/Utils/Array.h"
#include "Engine/Toolkit/Utils/ArrayQueue.h"
#include "Engine/Graphics/OpenGL/MeshDefine.h"


/**
 * The data update to VBO buffer by glBufferSubData.
 */
typedef struct
{
    GLenum     target;
    GLintptr   offset;
    GLsizeiptr length;
    GLvoid*    data;
}
VBOSubData;


/**
 * Render with texture by vertices, and the vertices info come from children SubMesh.
 * implement Drawable's render for render self.
 */
struct Mesh
{
    /**
     * The base class for provide draw functions.
     */
    Drawable             drawable[1];

    /**
     * Render texture.
     */
    Texture*             texture;

    /**
     * The index draw from , default first in children.
     */
    int                  fromIndex;

    /**
     * The index draw to, default last in children.
     */
    int                  toIndex;

    /**
     * Every frame pop pair of [from, to] index in childList to draw.
     * if no range will default draw fromIndex to toIndex.
     */
    ArrayQueue(int)      drawRangeQueue[1];

    /**
     * Mesh children provide vertices info to draw.
     *
     * cannot change children order directly, and use SubMesh index for sorting.
     */
    ArrayList(SubMesh*)  childList[1];

//----------------------------------------------------------------------------------------------------------------------

    /**
     * All vertices data from children SubMesh.
     * data model: [all position data | all uv data | all opacity data | all rgb data]
     */
    Array(float)*         vertexArr;

    /**
     * All vertices data from children SubMesh.
     */
    Array(short)*         indexArr;

    /**
     * If use VBO is the generated VBO ids else 0.
     */
    GLuint                vboIds[Mesh_BufferNum];

    /**
     * If use VAO is the generated vao id else 0.
     */
    GLuint                vaoId;

    /**
     * If use VBO use VBOSubData to update VBO buffer.
     * VBOSubData in ArrayList malloc data.
     */
    ArrayList(VBOSubData) vboSubDataList[1];

//----------------------------------------------------------------------------------------------------------------------

    /**
     * All vertices count.
     */
    int                   vertexCount;

    /**
     * The uv data offset in vertexArr.
     */
    int                   uvDataOffset;

    /**
     * The rgb data offset in vertexArr.
     */
    int                   rgbDataOffset;

    /**
     * The opacity data offset in vertexArr.
     */
    int                   opacityDataOffset;

    /**
     * The position data bytes size.
     */
    int                   positionDataSize;

    /**
     * The UV data bytes size.
     */
    int                   uvDataSize;

    /**
     * The RGB data bytes size.
     */
    int                   rgbDataSize;

    /**
     * The opacity data bytes size.
     */
    int                   opacityDataSize;

    /**
     * The index data bytes size.
     */
    int                   indexDataSize;
};


struct AMesh
{
    Mesh*     (*Create)            (Texture* texture);
    void      (*Init)              (Texture* texture,  Mesh* outMesh);
    void      (*InitWithCapacity)  (Texture* texture,  int capacity, Mesh* outMesh);

    /**
     * Release all member memory and all children SubMesh memory.
     */
    void      (*Release)           (Mesh* mesh);

    /**
     * Clear children, drawRangeQueue, vboSubDataList, and ready to GenerateBuffer.
     */
    void      (*Clear)             (Mesh* mesh);

    /**
     * The positionArr, uvArr and indexArr will copy in SubMesh.
     * SubMesh set parent and index by parent Mesh,
     * and will free by parent Mesh Release.
     */
    SubMesh*  (*AddChildWithData)  (Mesh* mesh, Array(float)* positionArr, Array(float)* uvArr, Array(short)* indexArr);

    /**
     * SubMesh data calculate by Quad.
     * SubMesh set parent and index by parent Mesh,
     * and will free by parent Mesh Release.
     */
    SubMesh*  (*AddChildWithQuad)  (Mesh* mesh, Quad* quad);

    /**
     * Reorder all SubMesh draw order by index.
     */
    void      (*ReorderAllChildren)(Mesh* mesh);

    /**
     * Combine all children SubMesh data into buffer that will upload to GPU.
     * when SubMesh init, remove or add, need to call this for generate buffer.
     */
    void      (*GenerateBuffer)    (Mesh* mesh);

    /**
     * The Mesh Drawable's render function implementation for render self.
     */
    void      (*Render)            (Drawable* drawable);
};


extern struct AMesh AMesh[1];


/**
 * Draw Mesh.
 */
static inline void AMesh_Draw(Mesh* mesh)
{
    ADrawable->Draw(mesh->drawable);
}


/**
 * Draw Mesh by fromIndex and toIndex.
 */
static inline void AMesh_DrawByIndex(Mesh *mesh, int fromIndex, int toIndex)
{
    AArrayQueue_Push(mesh->drawRangeQueue, fromIndex);
    AArrayQueue_Push(mesh->drawRangeQueue, toIndex);
    ADrawable->Draw (mesh->drawable);
}


#endif
