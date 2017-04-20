/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2016-8-7
 * Author : scott.cgi
 */

#ifndef mesh_def_h
#define mesh_def_h


#include "Engine/Graphics/OpenGL/Platform/gl3.h"


typedef struct Mesh    Mesh;
typedef struct SubMesh SubMesh;


enum
{
	mesh_buffer_num      = 2,
	mesh_buffer_num_byte = mesh_buffer_num * sizeof(GLuint),

	mesh_buffer_vertex   = 0,
	mesh_buffer_index    = 1,
};


enum
{
	/**
	 * One vertex with x, y, z
	 */
	mesh_vertex_position3_size   = 3,

	/**
	 * One vertex has u, v
	 */
	mesh_vertex_uv_size          = 2,

	/**
	 * One vertex has opacity
	 */
	mesh_vertex_opacity_size     = 1,

	/**
	 * One vertex has r,g,b
	 */
	mesh_vertex_rgb_size         = 3,

	/**
	 * One vertex with x, y
	 */
	mesh_vertex_position2_size   = 2,


	mesh_vertex_size             = mesh_vertex_position2_size + mesh_vertex_uv_size,


	/**
	 * 3(x,y,z) * 4(float size)
	 */
	mesh_vertex_position3_stride = 12,

	/**
	 * 2(u,v) * 4(float size)
	 */
	mesh_vertex_uv_stride        = 8,

	/**
	 * 1 * 4(float size)
	 */
	mesh_vertex_opacity_stride   = 4,

	/**
	 * 3(r,g,b) * 4(float size)
	 */
	mesh_vertex_rgb_stride       = 12,

	/**
	 * 2(x,y) * 4(float size)
	 */
	mesh_vertex_position2_stride = 8,


	mesh_vertex_stride           = mesh_vertex_position2_stride + mesh_vertex_uv_stride,


	/**
	 * Two triangles vertex 6(points)
	 */
	quad_index_num                 = 6,
	quad_index_num_byte            = quad_index_num     * sizeof(short),

	/**
	 * 3(x,y,z) * 4(points)
	 */
	quad_position3_num             = 12,
	quad_position3_num_byte        = quad_position3_num * sizeof(float),

	/**
	 * 2(u,v) * 4(points)
	 */
	quad_uv_num                    = 8,
	quad_uv_num_byte               = quad_uv_num        * sizeof(float),

	/**
	 * 1 * 4(points)
	 */
	quad_opacity_num               = 4,
	quad_opacity_num_byte          = quad_opacity_num   * sizeof(float),

	/**
	 * 3(r,g,b) * 4(points)
	 */
	quad_rgb_num                   = 12,
	quad_rgb_num_byte              = quad_rgb_num       * sizeof(float),

	/**
	 * 2(x,y) * 4(points)
	 */
	quad_position2_num             = 8,


	quad_vertex_num                = quad_position2_num + quad_uv_num,
	quad_vertex_num_byte           = quad_vertex_num    * sizeof(float),
};


#endif
