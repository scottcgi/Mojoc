/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2016-8-7
 * Author: scott.cgi
 */


#ifndef MESH_DEFINE_H
#define  MESH_DEFINE_H


#include "Engine/Graphics/OpenGL/Platform/gl3.h"


typedef struct Mesh    Mesh;
typedef struct SubMesh SubMesh;


enum
{
    MeshBuffer_Num      = 2,
    MeshBuffer_NumBytes = MeshBuffer_Num * sizeof(GLuint),

    MeshBuffer_Vertex   = 0,
    MeshBuffer_Index    = 1,
};


enum
{
    /**
     * x, y, z
     */
     MeshVertex_Position3Size  = 3,

    /**
     * u, v
     */
     MeshVertex_UVSize         = 2,

    /**
     * opacity
     */
     MeshVertex_OpacitySize    = 1,

    /**
     * r, g, b
     */
     MeshVertex_RGBSize        = 3,

    /**
     * x, y.
     */
     MeshVertex_Position2Size  = 2,

     /**
      * One vertex size.
      */
     MeshVertex_Size           = MeshVertex_Position2Size + MeshVertex_UVSize,


    /**
     * 3 (x ,y z) * 4 (float size)
     */
    MeshVertex_Position3Stride = 12,

    /**
     * 2 (u, v) * 4 (float size)
     */
    MeshVertex_UVStride        = 8,

    /**
     * 1 * 4 (float size)
     */
    MeshVertex_OpacityStride   = 4,

    /**
     * 3 (r, g, b) * 4 (float size)
     */
    MeshVertex_RGBStride       = 12,

    /**
     * 2 (x, y) * 4 (float size)
     */
    MeshVertex_Position2Stride = 8,

    /**
     * One vertex stride.
     */
    MeshVertex_VertexStride    = MeshVertex_Position2Stride + MeshVertex_UVStride,
};


enum
{
    /**
     * Two triangles (012, 230) has 6 vertices (2 vertices repeats).
     */
    Quad_IndexNum                 = 6,
    Quad_IndexNumBytes            = Quad_IndexNum     * sizeof(short),

    /**
     *  3 (x, y, z) * 4 (vertices)
     */
    Quad_Position3Num             = 12,
    Quad_Position3NumBytes        = Quad_Position3Num * sizeof(float),

    /**
     * 2 (u, v) * 4 (vertices)
     */
    Quad_UVNum                    = 8,
    Quad_UVNumBytes               = Quad_UVNum        * sizeof(float),

    /**
     * 1 * 4 (vertices)
     */
    Quad_OpacityNum               = 4,
    Quad_OpacityNumBytes          = Quad_OpacityNum   * sizeof(float),

    /**
     * 3 (r, g ,b) * 4 (vertices)
     */
    Quad_RGBNum                   = 12,
    Quad_RGBNumBytes              = Quad_RGBNum       * sizeof(float),

    /**
     * 2 (x, y) * 4 (vertices)
     */
    Quad_Position2Num             = 8,

    Quad_VertexNum                = Quad_Position2Num + Quad_UVNum,
    Quad_VertexNumBytes           = Quad_VertexNum    * sizeof(float),
};


#endif
