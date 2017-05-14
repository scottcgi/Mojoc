/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2016-8-5
 * Author: scott.cgi
 */

#include <string.h>
#include "Engine/Graphics/OpenGL/Mesh.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderMesh.h"
#include "Engine/Toolkit/Head/Struct.h"
#include "Engine/Graphics/Graphics.h"


static void ReorderChildren(Mesh* mesh)
{
	ArrayList* children        = mesh->childList;
	// SubMesh keep original indexDataOffset
	int        indexDataOffset = 0;

	for (int i = 0; i < children->size; i++)
	{
		SubMesh* subMesh = AArrayListGet(children, i, SubMesh*);

		while (subMesh->index != i)
		{
			subMesh = AArrayListGet(children, subMesh->index, SubMesh*);
		}

		int indexDataByteLength = subMesh->indexArr->length * sizeof(short);

		memcpy
		(
			(char*) mesh->indexArr->data + indexDataOffset,
			subMesh->indexArr->data,
			indexDataByteLength
		);

		indexDataOffset += indexDataByteLength;
	}

	if (AGraphics->isUseVBO)
	{
		// update all index data
		VBOSubData* subData = AArrayListGetPtrAdd(mesh->vboSubDataList, VBOSubData);
		subData->target     = GL_ELEMENT_ARRAY_BUFFER;
		subData->offset     = 0;
		subData->length     = mesh->indexArr->length * sizeof(short);
		subData->data       = mesh->indexArr->data;
	}
}


static void Draw(Drawable* meshDrawable)
{
	Mesh* mesh             = AStruct_GetParent3  (meshDrawable, Mesh, drawable);
	bool  isChangedOpacity = ADrawableCheckState(meshDrawable, drawable_state_opacity_ed);
	bool  isChangedRGB     = ADrawableCheckState(meshDrawable, drawable_state_rgb_ed);

	for (int i = 0; i < mesh->childList->size; i++)
	{
		SubMesh* subMesh = AArrayListGet(mesh->childList, i, SubMesh*);

//--------------------------------------------------------------------------------------------------

		bool isDrawnBefore = ADrawableCheckState(subMesh->drawable, drawable_state_draw_ed);
		ADrawable->Draw(subMesh->drawable);
		bool isDrawnAfter  = ADrawableCheckState(subMesh->drawable, drawable_state_draw_ed);

//--------------------------------------------------------------------------------------------------

		if (isDrawnAfter)
		{
			if (ADrawableCheckState(subMesh->drawable, drawable_state_transform_ed))
			{
				float* bornData     = AArrayGetData(subMesh->positionArr, float);
				float* positionData = (float*) ((char*) mesh->vertexArr->data + subMesh->positionDataOffset);

				// the born position data transformed(translate, scale, rotate) by SubMesh modelMatrix
				for (int j = 0; j < subMesh->positionArr->length; j += mesh_vertex_position3_size)
				{
					AMatrix->MultiplyMV3
					(
						subMesh->drawable->modelMatrix,
						bornData[j],
						bornData[j + 1],
						bornData[j + 2],
						(Vector3*) (positionData + j)
					);
				}

				if (AGraphics->isUseVBO)
				{
					VBOSubData* subData = AArrayListGetPtrAdd(mesh->vboSubDataList, VBOSubData);
					subData->target     = GL_ARRAY_BUFFER;
					subData->offset     = subMesh->positionDataOffset;
					subData->length     = subMesh->positionArr->length * sizeof(float);
					subData->data       = positionData;
				}
			}

//--------------------------------------------------------------------------------------------------

			if (ADrawableCheckState(subMesh->drawable, drawable_state_opacity_ed) || isChangedOpacity)
			{
				float  opacity     = subMesh->drawable->blendColor->a * meshDrawable->blendColor->a;
				float* opacityData = (float*) (
												(char*) mesh->vertexArr->data +
                                                mesh->opacityDataOffset       +
                                                subMesh->opacityDataOffset
											  );

				for (int j = 0; j < subMesh->vertexCount; j++)
				{
					opacityData[j] = opacity;
				}

				if (AGraphics->isUseVBO)
				{
					VBOSubData* subData = AArrayListGetPtrAdd(mesh->vboSubDataList, VBOSubData);
					subData->target     = GL_ARRAY_BUFFER;
					subData->offset     = mesh->opacityDataOffset + subMesh->opacityDataOffset;
					subData->length     = subMesh->vertexCount * sizeof(float);
					subData->data       = opacityData;
				}
			}

//--------------------------------------------------------------------------------------------------

			if (ADrawableCheckState(subMesh->drawable, drawable_state_rgb_ed) || isChangedRGB)
			{
				float  r       = subMesh->drawable->blendColor->r * meshDrawable->blendColor->r;
				float  g       = subMesh->drawable->blendColor->g * meshDrawable->blendColor->g;
				float  b       = subMesh->drawable->blendColor->b * meshDrawable->blendColor->b;

				float* rgbData = (float*) (
											(char*) mesh->vertexArr->data +
                                            mesh->rgbDataOffset           +
                                            subMesh->rgbDataOffset
										  );

				for (int j = 0; j < subMesh->vertexCount; j++)
				{
					int index          = j * 3;
					rgbData[index]     = r;
					rgbData[index + 1] = g;
					rgbData[index + 2] = b;
				}

				if (AGraphics->isUseVBO)
				{
					VBOSubData* subData = AArrayListGetPtrAdd(mesh->vboSubDataList, VBOSubData);
					subData->target     = GL_ARRAY_BUFFER;
					subData->offset     = mesh->rgbDataOffset + subMesh->rgbDataOffset;
					subData->length     = subMesh->vertexCount * 3 * sizeof(float);
					subData->data       = rgbData;
				}
			}
		}

//--------------------------------------------------------------------------------------------------

		// test visible changed
		if (isDrawnBefore != isDrawnAfter)
		{
			float* opacityData = (float*) (
										  	  (char*) mesh->vertexArr->data +
                                              mesh->opacityDataOffset       +
                                              subMesh->opacityDataOffset
										  );

			if (ADrawableCheckState(subMesh->drawable, drawable_state_draw_ed))
			{
				float opacity = subMesh->drawable->blendColor->a * meshDrawable->blendColor->a;
				for (int j = 0; j < subMesh->vertexCount; j++)
				{
					opacityData[j] = opacity;
				}
			}
			else
			{
                memset(opacityData, 0, subMesh->vertexCount * sizeof(float));
			}

			if (AGraphics->isUseVBO)
			{
				VBOSubData* subData = AArrayListGetPtrAdd(mesh->vboSubDataList, VBOSubData);
				subData->target     = GL_ARRAY_BUFFER;
				subData->offset     = mesh->opacityDataOffset + subMesh->opacityDataOffset;
				subData->length     = subMesh->vertexCount * sizeof(float);
				subData->data       = opacityData;
			}
		}
	}
}


static inline void BindVBO(Mesh* mesh)
{
    // load the position
    glVertexAttribPointer
    (
        AShaderMesh->attribPosition,
        mesh_vertex_position3_size,
        GL_FLOAT,
        false,
        mesh_vertex_position3_stride,
        0
    );

    // load the texture coordinate
    glVertexAttribPointer
    (
        AShaderMesh->attribTexcoord,
        mesh_vertex_uv_size,
        GL_FLOAT,
        false,
        mesh_vertex_uv_stride,
        (GLvoid*) (intptr_t) mesh->uvDataOffset
    );

    // load the opacity
    glVertexAttribPointer
    (
        AShaderMesh->attribOpacity,
        mesh_vertex_opacity_size,
        GL_FLOAT,
        false,
        mesh_vertex_opacity_stride,
        (GLvoid*) (intptr_t) mesh->opacityDataOffset
    );

    // load the rgb
    glVertexAttribPointer
    (
        AShaderMesh->attribRGB,
        mesh_vertex_rgb_size,
        GL_FLOAT,
        false,
        mesh_vertex_rgb_stride,
        (GLvoid*) (intptr_t) mesh->rgbDataOffset
    );
}


static void Render(Drawable* drawable)
{
	Mesh* mesh = AStruct_GetParent2(drawable, Mesh);

	if (mesh->childList->size == 0)
	{
		return;
	}

	SubMesh* fromChild;
	SubMesh* toChild;

	if (mesh->drawRangeQueue->elementList->size == 0)
	{
		fromChild = AArrayListGet(mesh->childList, mesh->fromIndex, SubMesh*);
		toChild   = AArrayListGet(mesh->childList, mesh->toIndex,   SubMesh*);
	}
	else
	{
		fromChild = AArrayListGet
					(
						mesh->childList,
						AArrayQueuePopWithDefault(mesh->drawRangeQueue, int, mesh->fromIndex),
						SubMesh*
					);

		toChild   = AArrayListGet
				    (
						mesh->childList,
						AArrayQueuePopWithDefault(mesh->drawRangeQueue, int, mesh->toIndex),
						SubMesh*
					);
	}

	// all SubMesh under Mesh matrix
	AShaderMesh->Use(drawable->mvpMatrix);

    glBindTexture(GL_TEXTURE_2D, mesh->texture->id);

//--------------------------------------------------------------------------------------------------

    if (mesh->vboSubDataList->size > 0)
    {
        // load the vertex data
        glBindBuffer(GL_ARRAY_BUFFER,         mesh->vboIds[mesh_buffer_vertex]);

        // load the vertex index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIds[mesh_buffer_index]);

        // in no vao state update sub data
        if (AGraphics->isUseMapBuffer)
        {
            for (int i = 0; i < mesh->vboSubDataList->size; i++)
            {
                VBOSubData* subData   = AArrayListGetPtr(mesh->vboSubDataList, i, VBOSubData);
                void*       mappedPtr = glMapBufferRange
                                        (
                                            subData->target,
                                            subData->offset,
                                            subData->length,
                                            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT
                                        );

                memcpy
                (
                    mappedPtr,
                    subData->data,
                    subData->length
                );

                glUnmapBuffer(subData->target);
            }
        }
        else
        {
            for (int i = 0; i < mesh->vboSubDataList->size; i++)
            {
                VBOSubData* subData = AArrayListGetPtr(mesh->vboSubDataList, i, VBOSubData);
                glBufferSubData(subData->target, subData->offset, subData->length, subData->data);
            }
        }

        AArrayList->Clear(mesh->vboSubDataList);

        if (AGraphics->isUseVAO)
        {
            // clean VBO bind
            glBindBuffer(GL_ARRAY_BUFFER,         0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            goto label_mesh_use_vao;
        }
        else if (AGraphics->isUseVBO)
        {
            goto label_mesh_use_vbo;
        }
    }

//--------------------------------------------------------------------------------------------------

    if (AGraphics->isUseVAO)
    {
        label_mesh_use_vao:
        
//--------------------------------------------------------------------------------------------------

        glBindVertexArray(mesh->vaoId);

        glDrawElements
        (
            GL_TRIANGLES,
            toChild->indexOffset - fromChild->indexOffset + toChild->indexArr->length,
            GL_UNSIGNED_SHORT,
            (GLvoid*) (intptr_t) fromChild->indexDataOffset
        );

        // clear VAO bind
        glBindVertexArray(0);
    }
    else if (AGraphics->isUseVBO)
	{
        // load the vertex data
        glBindBuffer(GL_ARRAY_BUFFER,         mesh->vboIds[mesh_buffer_vertex]);
        // load the vertex index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIds[mesh_buffer_index]);

        label_mesh_use_vbo:

//--------------------------------------------------------------------------------------------------

        BindVBO(mesh);

        glDrawElements
        (
            GL_TRIANGLES,
            toChild->indexOffset - fromChild->indexOffset + toChild->indexArr->length,
            GL_UNSIGNED_SHORT,
            (GLvoid*) (intptr_t) fromChild->indexDataOffset
        );

        // clean VBO bind
        glBindBuffer(GL_ARRAY_BUFFER,         0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
	{
    	// load the position
        glVertexAttribPointer
		(
			AShaderMesh->attribPosition,
			mesh_vertex_position3_size,
			GL_FLOAT,
			false,
			mesh_vertex_position3_stride,
			mesh->vertexArr->data
	    );

        // load the texture coordinate
        glVertexAttribPointer
		(
			AShaderMesh->attribTexcoord,
			mesh_vertex_uv_size,
			GL_FLOAT,
			false,
			mesh_vertex_uv_stride,
			(char*) mesh->vertexArr->data + mesh->uvDataOffset
		);

        // load the opacity
        glVertexAttribPointer
		(
			AShaderMesh->attribOpacity,
			mesh_vertex_opacity_size,
			GL_FLOAT,
			false,
			mesh_vertex_opacity_stride,
			(char*) mesh->vertexArr->data + mesh->opacityDataOffset
	    );

        // load the rgb
        glVertexAttribPointer
		(
			AShaderMesh->attribRGB,
			mesh_vertex_rgb_size,
			GL_FLOAT,
			false,
			mesh_vertex_rgb_stride,
			(char*) mesh->vertexArr->data + mesh->rgbDataOffset
        );

        glDrawElements
		(
			GL_TRIANGLES,
			toChild->indexOffset - fromChild->indexOffset + toChild->indexArr->length,
			GL_UNSIGNED_SHORT,
			(char*) mesh->indexArr->data + fromChild->indexDataOffset
	    );
	}
}


static void Init(Texture* texture, Mesh* outMesh)
{
	Quad quad[1];
	AQuad->Init(texture->width, texture->height, quad);

	Drawable* drawable               = outMesh->drawable;
	ADrawable->Init(drawable);

	// override
	drawable->Draw                   = Draw;
	drawable->Render                 = Render;

	ADrawableSetState(drawable, drawable_state_is_update_mvp);

	outMesh->texture                    = texture;
	outMesh->vboIds[mesh_buffer_index]  = 0;
    outMesh->vboIds[mesh_buffer_vertex] = 0;

    outMesh->vaoId                      = 0;
	outMesh->vertexArr                  = NULL;
	outMesh->indexArr                   = NULL;

	outMesh->vertexCountOffset          = 0;
    outMesh->positionDataLength         = 0;
    outMesh->uvDataLength               = 0;
    outMesh->rgbDataLength              = 0;
    outMesh->opacityDataLength          = 0;
    outMesh->indexDataLength            = 0;

	AArrayQueue->Init(sizeof(int),             outMesh->drawRangeQueue);
	AArrayList ->Init(sizeof(SubMesh*),        outMesh->childList);
	AArrayList ->Init(sizeof(VBOSubData),      outMesh->vboSubDataList);
    outMesh->vboSubDataList->increase = outMesh->childList->increase * 4;
}

static inline void InitBuffer(Mesh* mesh)
{
	mesh->vertexArr         = AArray->Create(sizeof(float), mesh->positionDataLength  + mesh->uvDataLength + mesh->opacityDataLength + mesh->rgbDataLength);
	mesh->indexArr          = AArray->Create(sizeof(short), mesh->indexDataLength);

	mesh->uvDataOffset      = mesh->positionDataLength                                * sizeof(float);
	mesh->opacityDataOffset = mesh->uvDataOffset            + mesh->uvDataLength      * sizeof(float);
	mesh->rgbDataOffset     = mesh->opacityDataOffset       + mesh->opacityDataLength * sizeof(float);

	char* uvData            = (char*) mesh->vertexArr->data + mesh->uvDataOffset;

	for (int i = 0; i < mesh->childList->size; i++)
	{
		SubMesh* subMesh = AArrayListGet(mesh->childList, i, SubMesh*);

		memcpy((char*) mesh->indexArr->data  + subMesh->indexDataOffset,    subMesh->indexArr->data,    subMesh->indexArr->length    * sizeof(short));
		memcpy((char*) mesh->vertexArr->data + subMesh->positionDataOffset, subMesh->positionArr->data, subMesh->positionArr->length * sizeof(float));
		memcpy(uvData                        + subMesh->uvDataOffset,       subMesh->uvArr->data,       subMesh->uvArr->length       * sizeof(float));

        // make drawable property update to buffer
        ADrawableSetState(subMesh->drawable, drawable_state_change);
	}

	mesh->fromIndex = 0;
	mesh->toIndex   = mesh->childList->size - 1;
}


static void InitWithCapacity(Texture* texture, int capacity, Mesh* outMesh)
{
	Init(texture, outMesh);
	AArrayList->SetCapacity(outMesh->childList, capacity);
}


static Mesh* Create(Texture* texture)
{
	Mesh* mesh = (Mesh*) malloc(sizeof(Mesh));
	Init(texture, mesh);

	return mesh;
}


static inline SubMesh* AddChild(Mesh* mesh, SubMesh* subMesh)
{
    for (int i = 0; i < subMesh->indexArr->length; i++)
    {
        // each child index add before children vertex count
        AArrayGet(subMesh->indexArr, i, short) += mesh->vertexCountOffset;
    }

    subMesh->index              = mesh->childList->size;
    subMesh->parent             = mesh;

    subMesh->positionDataOffset = mesh->positionDataLength * sizeof(float);
    subMesh->uvDataOffset       = mesh->uvDataLength       * sizeof(float);
    subMesh->opacityDataOffset  = mesh->opacityDataLength  * sizeof(float);
    subMesh->rgbDataOffset      = mesh->rgbDataLength      * sizeof(float);
    subMesh->indexDataOffset    = mesh->indexDataLength    * sizeof(short);

    subMesh->indexOffset        = mesh->indexDataLength;
    subMesh->vertexCount        = subMesh->positionArr->length / 3;

    mesh->vertexCountOffset    += subMesh->vertexCount;
    mesh->positionDataLength   += subMesh->positionArr->length;
    mesh->uvDataLength         += subMesh->uvArr->length;
    mesh->opacityDataLength    += subMesh->vertexCount;
    mesh->rgbDataLength        += subMesh->positionArr->length;
    mesh->indexDataLength      += subMesh->indexArr->length;

    AArrayListAdd(mesh->childList, subMesh);

    return subMesh;
}


static SubMesh* AddChildWithData(Mesh* mesh, Array(float)* positionArr, Array(float)* uvArr, Array(short)* indexArr)
{
	return AddChild
           (
               mesh,
               ASubMesh->CreateWithData(positionArr, uvArr, indexArr)
           );
}


static SubMesh* AddChildWithQuad(Mesh* mesh, Quad* quad)
{
    return AddChild
           (
			   mesh,
			   ASubMesh->CreateWithQuad(mesh->texture, quad)
           );
}


static inline void ReleaseBuffer(Mesh* mesh)
{
	free(mesh->vertexArr);
	free(mesh->indexArr);

	mesh->vertexArr = NULL;
	mesh->indexArr  = NULL;

	if (AGraphics->isUseVBO)
	{
		glDeleteBuffers(mesh_buffer_num, mesh->vboIds);
		mesh->vboIds[mesh_buffer_index]  = 0;
        mesh->vboIds[mesh_buffer_vertex] = 0;

        if (AGraphics->isUseVAO)
        {
            glDeleteVertexArrays(1, &mesh->vaoId);
            mesh->vaoId = 0;
        }
	}
}


static void GenerateBuffer(Mesh* mesh)
{
	free(mesh->vertexArr);
	free(mesh->indexArr);

	InitBuffer(mesh);

	if (AGraphics->isUseVBO)
	{
        if (mesh->vboIds[mesh_buffer_vertex] == 0)
        {
            glGenBuffers(mesh_buffer_num, mesh->vboIds);
        }

        // vertex
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vboIds[mesh_buffer_vertex]);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertexArr->length * sizeof(float), mesh->vertexArr->data, GL_DYNAMIC_DRAW);

        // index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIds[mesh_buffer_index]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexArr->length * sizeof(short), mesh->indexArr->data, GL_STATIC_DRAW);

        // vertexArr and indexArr data pointer changed
        // so we clear all sub data update
        AArrayList->Clear(mesh->vboSubDataList);

//--------------------------------------------------------------------------------------------------

        if (AGraphics->isUseVAO)
        {
            if (mesh->vaoId == 0)
            {
                glGenVertexArrays(1, &mesh->vaoId);
            }

            glBindVertexArray(mesh->vaoId);

/*
----------------------------------------------------------------------------------------------------
    with vao has own state
----------------------------------------------------------------------------------------------------
*/

            // load the vertex data
            glBindBuffer(GL_ARRAY_BUFFER,         mesh->vboIds[mesh_buffer_vertex]);
            // load the vertex index
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIds[mesh_buffer_index]);

            glEnableVertexAttribArray(AShaderMesh->attribPosition);
            glEnableVertexAttribArray(AShaderMesh->attribTexcoord);
            glEnableVertexAttribArray(AShaderMesh->attribOpacity);
            glEnableVertexAttribArray(AShaderMesh->attribRGB);

            BindVBO(mesh);

            // go back to normal state
            glBindVertexArray(0);
        }
	}
}


static void Release(Mesh* mesh)
{
	ReleaseBuffer(mesh);

	for (int i = 0; i < mesh->childList->size; i++)
	{
		free(AArrayListGet(mesh->childList, i, SubMesh*));
	}

	AArrayList ->Release(mesh->childList);
	AArrayList ->Release(mesh->vboSubDataList);
    AArrayQueue->Release(mesh->drawRangeQueue);
}


static void Clear(Mesh* mesh)
{
    for (int i = 0; i < mesh->childList->size; i++)
    {
        free(AArrayListGet(mesh->childList, i, SubMesh*));
    }

    AArrayList ->Clear(mesh->childList);
    AArrayList ->Clear(mesh->vboSubDataList);
    AArrayQueue->Clear(mesh->drawRangeQueue);

    mesh->vertexCountOffset  = 0;
    mesh->positionDataLength = 0;
    mesh->uvDataLength       = 0;
    mesh->rgbDataLength      = 0;
    mesh->opacityDataLength  = 0;
    mesh->indexDataLength    = 0;
}


struct AMesh AMesh[1] =
{
	Create,
	Init,
	InitWithCapacity,
	Release,
	Clear,

	AddChildWithData,
	AddChildWithQuad,
	ReorderChildren,
	GenerateBuffer,
	Render,
};
