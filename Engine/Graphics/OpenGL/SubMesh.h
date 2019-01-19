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
#define  SUB_MESH_H


#include "Engine/Graphics/OpenGL/MeshDefine.h"
#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Graphics/Draw/Quad.h"


/**
 * The child of Mesh that render part of Texture.
 */
struct SubMesh
{
    /**
     * if regenerate and the drawable parent invisible，then the parent influence will lost.
     */
    Drawable     drawable[1];

    /**
     * SubMesh coordinate under parent Mesh drawable matrix.
     * if SubMesh drawable has own parent, equivalent to the own parent under Mesh drawable matrix.
     * so if let parent Mesh coordinate equal world space, will make SubMesh own coordinate more understandable.
     */
    Mesh*         parent;

    /**
     * Index in Mesh for Mesh ReorderAllChildren.
     */
    int           index;

//----------------------------------------------------------------------------------------------------------------------

    Array(float*)  positionArr[1];
    Array(float*)  uvArr      [1];

    /**
     * Careful 4 byte aligned.
     */
    Array(short*) indexArr  [1];

//----------------------------------------------------------------------------------------------------------------------

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
