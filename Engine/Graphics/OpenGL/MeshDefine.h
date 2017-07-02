/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2016-8-7
 * Author : scott.cgi
 * Version: 0.0.0
 */


#ifndef MESH_DEFINE_H
#define MESH_DEFINE_H


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
     * One vertex with x, y, z
     */
     MeshVertex_Position3Size  = 3,

    /**
     * One vertex has u, v
     */
     MeshVertex_UVSize         = 2,

    /**
     * One vertex has opacity
     */
     MeshVertex_OpacitySize    = 1,

    /**
     * One vertex has r,g,b
     */
     MeshVertex_RGBSize        = 3,

    /**
     * One vertex with x, y
     */
     MeshVertex_Position2Size  = 2,


     MeshVertex_Size           = MeshVertex_Position2Size + MeshVertex_UVSize,


    /**
     * 3(x,y,z) * 4(float size)
     */
    MeshVertex_Position3Stride = 12,

    /**
     * 2(u,v) * 4(float size)
     */
    MeshVertex_UVStride        = 8,

    /**
     * 1 * 4(float size)
     */
    MeshVertex_OpacityStride   = 4,

    /**
     * 3(r,g,b) * 4(float size)
     */
    MeshVertex_RGBStride       = 12,

    /**
     * 2(x,y) * 4(float size)
     */
    MeshVertex_Position2Stride = 8,


    MeshVertex_VertexStride    = MeshVertex_Position2Stride + MeshVertex_UVStride,
};


enum
{
    /**
     * Two triangles vertex 6(points)
     */
    Quad_IndexNum                 = 6,
    Quad_IndexNumBytes            = Quad_IndexNum     * sizeof(short),

    /**
     * 3(x,y,z) * 4(points)
     */
    Quad_Position3Num             = 12,
    Quad_Position3NumBytes        = Quad_Position3Num * sizeof(float),

    /**
     * 2(u,v) * 4(points)
     */
    Quad_UVNum                    = 8,
    Quad_UVNumBytes               = Quad_UVNum        * sizeof(float),

    /**
     * 1 * 4(points)
     */
    Quad_OpacityNum               = 4,
    Quad_OpacityNumBytes          = Quad_OpacityNum   * sizeof(float),

    /**
     * 3(r,g,b) * 4(points)
     */
    Quad_RGBNum                   = 12,
    Quad_RGBNumBytes              = Quad_RGBNum       * sizeof(float),

    /**
     * 2(x,y) * 4(points)
     */
    Quad_Position2Num             = 8,


    Quad_VertexNum                = Quad_Position2Num + Quad_UVNum,
    Quad_VertexNumBytes           = Quad_VertexNum    * sizeof(float),
};


#endif
