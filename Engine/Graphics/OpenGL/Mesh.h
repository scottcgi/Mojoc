/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2016-8-5
 * Update   : 2019-1-22
 * Author   : scott.cgi
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
 * The data update to VBO buffer by glBufferSubData or glMapBufferRange.
 */
typedef struct
{
    /**
     * GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER.
     */
    GLenum     target;

    /**
     * The byte offset in VBO data.
     */
    GLintptr   offset;

    /**
     * The bytes size of update data.
     */
    GLsizeiptr size;

    /**
     * The update data ptr.
     */
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
     * The mode of glDrawElements, default GL_TRIANGLES.
     */
    GLenum               drawMode;

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
     * if from equals to then means only draw child at to index.
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
    GLuint                vboIDs[Mesh_BufferNum];

    /**
     * If use VAO is the generated vao id else 0.
     */
    GLuint                vaoID;

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
     * The vertex bytes data size.
     */
    int                   vertexDataSize;

    /**
     * The index bytes data size.
     */
    int                   indexDataSize;

    /**
     * The uv bytes data offset in vertexArr.
     */
    int                   uvDataOffset;

    /**
     * The rgb bytes data offset in vertexArr.
     */
    int                   rgbDataOffset;

    /**
     * The opacity bytes data offset in vertexArr.
     */
    int                   opacityDataOffset;

    /**
     * The position data length in vertexArr..
     */
    int                   positionDataLength;

    /**
     * The UV data length in vertexArr.
     */
    int                   uvDataLength;

    /**
     * The RGB data length in vertexArr.
     */
    int                   rgbDataLength;

    /**
     * The opacity data length in vertexArr.
     */
    int                   opacityDataLength;

    /**
     * The index data length in indexArr.
     */
    int                   indexDataLength;
};


struct AMesh
{
    Mesh*     (*Create)            (Texture* texture);
    void      (*Init)              (Texture* texture, Mesh* outMesh);
    void      (*InitWithCapacity)  (Texture* texture, int capacity, Mesh* outMesh);

    /*
     Create or Init Mesh by resourceFilePath.

     resourceFilePath:
         Android: assets
         IOS    : NSBundle
     */

    Mesh*     (*CreateWithFile)         (const char* resourceFilePath);
    void      (*InitWithFile)           (const char* resourceFilePath, Mesh* outMesh);
    void      (*InitWithFileAndCapacity)(const char* resourceFilePath, int capacity, Mesh* outMesh);

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
     * SubMesh set parent and index by parent Mesh, and will free by parent Mesh Release.
     *
     * important: before GenerateBuffer will not work correctly,
     *            and we can calling GenerateBuffer when all children have been added.
     */
    SubMesh*  (*AddChildWithData)  (Mesh* mesh, Array(float)* positionArr, Array(float)* uvArr, Array(short)* indexArr);

    /**
     * SubMesh data calculate by Quad.
     * SubMesh set parent and index by parent Mesh, and will free by parent Mesh Release.
     *
     * important: apply effect after calling GenerateBuffer,
     *            and we can calling GenerateBuffer when all children have been added.
     */
    SubMesh*  (*AddChildWithQuad)  (Mesh* mesh, Quad* quad);

    /**
     * Reorder all SubMesh draw order by index.
     */
    void      (*ReorderAllChildren)(Mesh* mesh);

    /**
     * Combine all children SubMesh data into buffer that will upload to GPU.
     * when Mesh init or SubMesh add, need to call this for generate buffer.
     * 
     * usually generate buffer before Mesh's SubMesh stable.
     */
    void      (*GenerateBuffer)    (Mesh* mesh);

    /**
     * The implementation of Drawable's render function for render Mesh.
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
 * if fromIndex equals toIndex then means only draw child at toIndex.
 */
static inline void AMesh_DrawByIndex(Mesh* mesh, int fromIndex, int toIndex)
{
    AArrayQueue_Push(mesh->drawRangeQueue, fromIndex);
    AArrayQueue_Push(mesh->drawRangeQueue, toIndex);
    ADrawable->Draw (mesh->drawable);
}


#endif
