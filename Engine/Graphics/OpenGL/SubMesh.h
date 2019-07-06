/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License.
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub.
 * This Mojoc game engine is a serious project with extreme code pursuit, and will continue to iterate.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2016-8-7
 * Update : 2019-1-19
 * Author : scott.cgi
 */


#ifndef SUB_MESH_H
#define SUB_MESH_H


#include "Engine/Graphics/OpenGL/MeshDefine.h"
#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Graphics/Draw/Quad.h"


/**
 * The child of Mesh that render part of Texture.
 * 
 * the SubMesh has an array of vertices,
 * and each vertex has position (xyz), uv coordinates, and vertex indices.
 */
struct SubMesh
{
    /**
     * The base class for provide draw functions.
     * if regenerate and the drawable parent invisible，then the parent influence will lost.
     */
    Drawable      drawable[1];

    /**
     * SubMesh coordinate under parent Mesh drawable matrix.
     * if SubMesh drawable has own parent, equivalent to the own parent under Mesh drawable matrix.
     * so if let parent Mesh coordinate equal world space, will make SubMesh own coordinate more understandable.
     */
    Mesh*         parent;

    /**
     * Index in Mesh, so change it will reorder SubMesh by Mesh ReorderAllChildren.
     * default -1.
     */
    int           index;

    /**
     * The uv width in parent texture.
     */
    float         uvWidth;

    /**
     * The uv height in parent texture.
     */
    float         uvHeight;

//----------------------------------------------------------------------------------------------------------------------

    Array(float*) positionArr[1];
    Array(float*) uvArr      [1];
    Array(short*) indexArr   [1];

//----------------------------------------------------------------------------------------------------------------------

    /**
     * All vertices count.
     */
    int           vertexCount;

    /**
     * The vertex bytes data size.
     */
    int           vertexDataSize;

    /**
     * The index bytes data size.
     */
    int           indexDataSize;

    /**
     * The position bytes data size.
     */
    int           positionDataSize;

    /**
     * The uv bytes data size.
     */
    int           uvDataSize;

    /**
     * The index offset in parent Mesh indexArr.
     */
    int           indexOffset;

    /**
     * The position bytes data offset in parent Mesh vertexArr.
     */
    int           positionDataOffset;

    /**
     * The uv bytes data offset in parent Mesh vertexArr.
     */
    int           uvDataOffset;

    /**
     * The rgb bytes data offset in parent Mesh vertexArr.
     */
    int           rgbDataOffset;

    /**
     * The opacity bytes data offset in parent Mesh vertexArr.
     */
    int           opacityDataOffset;

    /**
     * The index bytes data offset in parent Mesh indexArr.
     */
    int           indexDataOffset;
};


/**
 * Control SubMesh.
 */
struct ASubMesh
{
    /**
     * The positionArr (array of xyz), uvArr (array of uv), indexArr (one vertex one index) will copy into SubMesh,
     * and all data create by one malloc.
     */
    SubMesh* (*CreateWithData)(Mesh* paren, Array(float)* positionArr, Array(float)* uvArr, Array(short)* indexArr);

    /**
     * The positionArr, uvArr, indexArr will calculated with quad that in texture.
     */
    SubMesh* (*CreateWithQuad)(Mesh* parent, Quad* quad);

    /**
     * Set uv data with quad that in parent texture.
     */
    void     (*SetUVWithQuad) (SubMesh* subMesh, Quad* quad);

    /**
     * Deform SubMesh vertex position and uv.
     * the positionDeformArr will add each position (x y).
     * the uvDeformArr will add each uv.
     * the vertices order is from left-top counterclockwise to right-top.
     *
     * positionDeformArr: if NULL will not deform.
     *                    the length must equals vertex positions number (positionArr length).
     *
     * uvDeformArr      : if NULL will not deform.
     *                    the length must equals vertex uvs number (uvArr length).
     */
    void    (*Deform)         (SubMesh* subMesh, Array(float)* positionDeformArr, Array(float)* uvDeformArr);

    /**
     * Same as Deform function but deform position and uv by index.
     *
     * indexArr: the index of position or uv array, and each vertex position or uv will deform.
     *
     * if deform position, then the indexArr length must equals positionDeformArr length.
     * if deform uv, then the indexArr length must equals uvArr length.
     * 
     * if deform position and uv at same time, then the indexArr must give both of position and uv index,
     * and the form like [position...position...uv...uv...],
     * and the positionDeformArr length and uvDeformArr length that each equals half length of indexArr.
     */
    void    (*DeformByIndex)  (
                                  SubMesh*      subMesh,
                                  Array(float)* positionDeformArr,
                                  Array(float)* uvDeformArr,
                                  Array(int)*   indexArr
                              );
};


extern struct ASubMesh ASubMesh[1];


/**
 * Draw SubMesh.
 */
static inline void ASubMesh_Draw(SubMesh* subMesh)
{
    ADrawable->Draw(subMesh->drawable);
}


#endif
