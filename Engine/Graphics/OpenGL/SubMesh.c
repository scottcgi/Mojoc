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


#include <string.h>
#include "Engine/Graphics/Graphics.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Graphics/OpenGL/Mesh.h"
#include "Engine/Toolkit/Platform/Log.h"


static inline void InitSubMesh(SubMesh* subMesh, float width, float height)
{
    ADrawable->Init(subMesh->drawable);

    subMesh->parent             = NULL;
    subMesh->index              = -1;

    subMesh->vertexCount        = 0;
    subMesh->indexOffset        = 0;

    subMesh->positionDataOffset = 0;
    subMesh->uvDataOffset       = 0;
    subMesh->indexDataOffset    = 0;
    subMesh->opacityDataOffset  = 0;
    subMesh->rgbDataOffset      = 0;

    subMesh->drawable->width    = width;
    subMesh->drawable->height   = height;
}


static SubMesh* CreateWithData(Array(float)* positionArr, Array(float)* uvArr, Array(short)* indexArr)
{
    int      indexDataSize       = indexArr->length    * sizeof(short);
    int      uvDataSize          = uvArr->length       * sizeof(float);
    int      positionDataSize    = positionArr->length * sizeof(float);
    SubMesh* subMesh             = malloc(sizeof(SubMesh) + indexDataSize + uvDataSize + positionDataSize);

    subMesh->uvArr->length       = uvArr->length;
    subMesh->uvArr->data         = (char*) subMesh + sizeof(SubMesh);
    memcpy(subMesh->uvArr->data, uvArr->data, (size_t) uvDataSize);

    subMesh->positionArr->length = positionArr->length;
    subMesh->positionArr->data   = (char*) subMesh->uvArr->data + uvDataSize;
    memcpy(subMesh->positionArr->data, positionArr->data, (size_t) positionDataSize);


    subMesh->indexArr->length    = indexArr->length;
    subMesh->indexArr->data      = (char*) subMesh->positionArr->data + (size_t) positionDataSize;
    memcpy(subMesh->indexArr->data, indexArr->data, (size_t) indexDataSize);

//----------------------------------------------------------------------------------------------------------------------

    float* positionData = subMesh->positionArr->data;
    float  minX         = positionData[0];
    float  maxX         = minX;
    float  minY         = positionData[1];
    float  maxY         = minY;

    // calculate SubMesh size
    for (int i = Mesh_VertexPositionSize; i < subMesh->positionArr->length; i += Mesh_VertexPositionSize)
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

    AQuad->GetUV       (quad, texture, subMesh->uvArr->data);
    AQuad->GetPosition3(quad, subMesh->positionArr->data);

    Mesh* mesh = subMesh->parent;

    ALog_A(mesh != NULL, "ASubMesh SetWithQuad subMesh must has parent");

    int uvDataSize = subMesh->uvArr->length * sizeof(float);

    memcpy
    (
        (char*)  mesh->vertexArr->data + mesh->uvDataOffset + subMesh->uvDataOffset,
        subMesh->uvArr->data,
        (size_t) uvDataSize
    );

    if (AGraphics->isUseVBO)
    {
        VBOSubData* subData = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
        subData->target     = GL_ARRAY_BUFFER;
        subData->offset     = mesh->uvDataOffset + subMesh->uvDataOffset;
        subData->length     = uvDataSize;
        subData->data       = subMesh->uvArr->data;
    }
}


static SubMesh* CreateWithQuad(Texture* texture, Quad* quad)
{
    SubMesh* subMesh = malloc(sizeof(SubMesh) + Quad_IndexSize + Quad_UVSize + Quad_Position3Size);

    subMesh->indexArr->length     = Quad_IndexNum;
    subMesh->indexArr->data       = (char*) subMesh + sizeof(SubMesh);
    AQuad->GetIndex(0, subMesh->indexArr->data);

    subMesh->uvArr->length        = Quad_UVNum;
    subMesh->uvArr->data          = (char*) subMesh->indexArr->data + Quad_IndexSize;
    AQuad->GetUV(quad, texture, subMesh->uvArr->data);

    subMesh->positionArr->length  = Quad_Position3Num;
    subMesh->positionArr->data    = (char*) subMesh->uvArr->data + Quad_UVSize;
    AQuad->GetPosition3(quad, subMesh->positionArr->data);

    InitSubMesh(subMesh, quad->width, quad->height);

    return subMesh;
}


struct ASubMesh ASubMesh[1] =
{
    CreateWithData,
    CreateWithQuad,
    SetWithQuad,
};
