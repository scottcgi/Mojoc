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


#ifndef MESH_DEFINE_H
#define  MESH_DEFINE_H


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
    Mesh_VertexPositionSize    = 3,

    /**
     * One vertex has u, v.
     */
    Mesh_VertexUVSize          = 2,

    /**
     * One vertex has one opacity.
     */
    Mesh_VertexOpacitySize     = 1,

    /**
     * One vertex has r, g, b.
     */
    Mesh_VertexRGBSize         = 3,

    /**
     * 3 (x ,y z) * 4 (sizeof float)
     */
    Mesh_VertexPositionStride  = 12,

    /**
     * 2 (u, v) * 4 (sizeof float)
     */
    Mesh_VertexUVStride        = 8,

    /**
     * 1 * 4 (sizeof float)
     */
    Mesh_VertexOpacityStride   = 4,

    /**
     * 3 (r, g, b) * 4 (sizeof float)
     */
    Mesh_VertexRGBStride       = 12,
};


#endif
