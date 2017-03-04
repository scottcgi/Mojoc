/*
 * SubMesh.c
 *
 *  Created on: 2016-8-7
 *      Author: scott.cgi
 */

#include "Mojoc/Graphics/Graphics.h"
#include "Mojoc/Graphics/OpenGL/SubMesh.h"
#include "Mojoc/Graphics/OpenGL/Mesh.h"
#include "Mojoc/Toolkit/Platform/Log.h"

static inline void InitSubMesh(SubMesh* subMesh, float width, float height)
{
	ADrawable->Init(subMesh->drawable);

	subMesh->parent             = NULL;
	subMesh->index              = -1;

	subMesh->positionDataOffset = 0;
	subMesh->uvDataOffset       = 0;
	subMesh->indexDataOffset    = 0;
	subMesh->opacityDataOffset  = 0;
	subMesh->rgbDataOffset      = 0;
	subMesh->indexOffset        = 0;
    subMesh->vertexCount        = 0;

	subMesh->drawable->width    = width;
	subMesh->drawable->height   = height;
}


static SubMesh* CreateWithData(Array(float)* positionArr, Array(float)* uvArr, Array(short)* indexArr)
{
	int      indexLength    = indexArr->length    * sizeof(short);
	int      uvLength       = uvArr->length       * sizeof(float);
	int      positionLength = positionArr->length * sizeof(float);
	SubMesh* subMesh        = (SubMesh*) malloc
								         (
								            sizeof(SubMesh)  +
								            indexLength      +
								            uvLength         +
								            positionLength
								         );

	subMesh->uvArr->length       = uvArr->length;
	subMesh->uvArr->data         = (char*) subMesh + sizeof(SubMesh);
	memcpy(subMesh->uvArr->data, uvArr->data, uvLength);

	subMesh->positionArr->length = positionArr->length;
	subMesh->positionArr->data   = (char*) subMesh->uvArr->data + uvLength;
	memcpy(subMesh->positionArr->data, positionArr->data, positionLength);


	subMesh->indexArr->length    = indexArr->length;
	subMesh->indexArr->data      = (char*) subMesh->positionArr->data + positionLength;
	memcpy(subMesh->indexArr->data, indexArr->data, indexLength);


//--------------------------------------------------------------------------------------------------

    float* positionData = AArrayGetData(subMesh->positionArr, float);
    float  minX         = positionData[0];
    float  maxX         = positionData[0];
    float  minY         = positionData[1];
    float  maxY         = positionData[1];

    for (int i = mesh_vertex_position3_size; i < subMesh->positionArr->length; i += mesh_vertex_position3_size)
    {
        float x = positionData[i];
        float y = positionData[i + 1];

        if (x < minX)
        {
            minX = x;
        }
        else if (x > maxX)
        {
            maxX = x;
        }

        if (y < minY)
        {
            minY = y;
        }
        else if (y > maxY)
        {
            maxY = y;
        }
    }

    InitSubMesh(subMesh, maxX - minX, maxY - minY);

	return subMesh;
}

static void SetWithQuad(SubMesh* subMesh, Texture* texture, Quad* quad)
{
	subMesh->drawable->width  = quad->width;
	subMesh->drawable->height = quad->height;

	AQuad->GetQuadUV       (quad, texture, subMesh->uvArr->data);
	AQuad->GetQuadPosition3(quad, subMesh->positionArr->data);

	Mesh* mesh = subMesh->parent;

	ALogA(mesh != NULL, "SubMesh SetWithQuad must has parent");

	int uvDataLen = subMesh->uvArr->length * sizeof(float);

	memcpy
	(
		(char*) mesh->vertexArr->data + mesh->uvDataOffset + subMesh->uvDataOffset,
		subMesh->uvArr->data,
		uvDataLen
	);

	if (AGraphics->isUseVBO)
	{
		VBOSubData* subData = AArrayListGetPtrAdd(mesh->vboSubDataList, VBOSubData);
		subData->target     = GL_ARRAY_BUFFER;
		subData->offset     = mesh->uvDataOffset + subMesh->uvDataOffset;
		subData->length     = uvDataLen;
		subData->data       = subMesh->uvArr->data;
	}
}

static SubMesh* CreateWithQuad(Texture* texture, Quad* quad)
{
	SubMesh* subMesh = (SubMesh*) malloc
							      (
							  	    sizeof(SubMesh)          +
							  	    quad_index_num_byte      +
							  	    quad_uv_num_byte         +
							  	    quad_position3_num_byte
							      );


	subMesh->indexArr->length     = quad_index_num;
	subMesh->indexArr->data       = (char*) subMesh + sizeof(SubMesh);
	AQuad->GetQuadIndex(0, subMesh->indexArr->data);

	subMesh->uvArr->length        = quad_uv_num;
	subMesh->uvArr->data          = (char*) subMesh->indexArr->data + quad_index_num_byte;
	AQuad->GetQuadUV(quad, texture, subMesh->uvArr->data);

	subMesh->positionArr->length  = quad_position3_num;
	subMesh->positionArr->data    = (char*) subMesh->uvArr->data + quad_uv_num_byte;
	AQuad->GetQuadPosition3(quad, subMesh->positionArr->data);

    InitSubMesh(subMesh, quad->width, quad->height);

	return subMesh;
}


struct ASubMesh ASubMesh[1] =
{
	CreateWithData,
	CreateWithQuad,
	SetWithQuad,
};
