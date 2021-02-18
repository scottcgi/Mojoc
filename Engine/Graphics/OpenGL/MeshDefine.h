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
 * Since    : 2016-8-7
 * Update   : 2019-1-19
 * Author   : scott.cgi
 */


#ifndef MESH_DEFINE_H
#define MESH_DEFINE_H


#include "Engine/Graphics/OpenGL/Platform/gl3.h"


typedef struct Mesh    Mesh;
typedef struct SubMesh SubMesh;


enum
{
    /**
     * Number of buffers.
     */
    Mesh_BufferNum             = 2,

    /**
     * Index of buffer vertex.
     */
    Mesh_BufferVertex          = 0,

    /**
     * Index of buffer index.
     */
    Mesh_BufferIndex           = 1,

    /**
     * One vertex position has x, y, z.
     */
    Mesh_VertexPositionNum     = 3,

    /**
     * One vertex has u, v.
     */
    Mesh_VertexUVNum           = 2,

    /**
     * One vertex has one opacity.
     */
    Mesh_VertexOpacityNum      = 1,

    /**
     * One vertex has r, g, b.
     */
    Mesh_VertexRGBNum          = 3,

    /**
     * 3 (x ,y z) * 4 (sizeof float)
     */
    Mesh_VertexPositionStride  = Mesh_VertexPositionNum * sizeof(float),

    /**
     * 2 (u, v) * 4 (sizeof float)
     */
    Mesh_VertexUVStride        = Mesh_VertexUVNum       * sizeof(float),

    /**
     * 1 * 4 (sizeof float)
     */
    Mesh_VertexOpacityStride   = Mesh_VertexOpacityNum  * sizeof(float),

    /**
     * 3 (r, g, b) * 4 (sizeof float)
     */
    Mesh_VertexRGBStride       = Mesh_VertexRGBNum      * sizeof(float),
};


#endif
