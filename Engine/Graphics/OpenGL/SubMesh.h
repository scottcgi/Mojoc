/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2016-8-7
 * Update: 2019-1-19
 * Author: scott.cgi
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
    SubMesh* (*CreateWithData)(Array(float)* positionArr, Array(float)* uvArr, Array(short)* indexArr);

    /**
     * The positionArr, uvArr, indexArr will calculated by quad that in texture.
     */
    SubMesh* (*CreateWithQuad)(Texture* texture, Quad* quad);

    /**
     * Update SubMesh all data by quad that in texture.
     */
    void     (*SetWithQuad)   (SubMesh* subMesh, Texture* texture, Quad* quad);
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
