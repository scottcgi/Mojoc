/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2016-8-7
 * Update   : 2019-1-19
 * Author   : scott.cgi
 */


#include <string.h>

#include "Engine/Graphics/Graphics.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Graphics/OpenGL/Mesh.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLTool.h"


static inline void InitSubMesh(Mesh* parent, SubMesh* subMesh, float width, float height)
{
    ADrawable->Init(subMesh->drawable);

    subMesh->parent             = parent;
    subMesh->index              = -1;
    subMesh->indexOffset        = 0;
    subMesh->positionDataOffset = 0;
    subMesh->uvDataOffset       = 0;
    subMesh->indexDataOffset    = 0;
    subMesh->opacityDataOffset  = 0;
    subMesh->rgbDataOffset      = 0;

    subMesh->vertexCount        = subMesh->positionArr->length / Mesh_VertexPositionNum;
    subMesh->vertexDataSize     = subMesh->vertexCount         * sizeof(float);
    subMesh->drawable->width    = width;
    subMesh->drawable->height   = height;
    subMesh->uvWidth            = AGLTool_ToUVWidth (width,  parent->texture->width);
    subMesh->uvHeight           = AGLTool_ToUVHeight(height, parent->texture->height);
}


static inline void GetSize(Array(float)* array, int dataNum, float* outWidth, float* outHeight)
{
    float* data = array->data;
    float  minX = data[0];
    float  minY = data[1];
    float  maxX = minX;
    float  maxY = minY;

    // calculate array size
    for (int i = dataNum; i < array->length; i += dataNum)
    {
        float x = data[i];
        float y = data[i + 1];

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

    *outWidth  = maxX - minX;
    *outHeight = maxY - minY;
}


static SubMesh* CreateWithData(Mesh* parent, Array(float)* positionArr, Array(float)* uvArr, Array(short)* indexArr)
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

    subMesh->indexDataSize       = indexDataSize;
    subMesh->uvDataSize          = uvDataSize;
    subMesh->positionDataSize    = positionDataSize;

    float width;
    float height;
    GetSize(subMesh->positionArr, Mesh_VertexPositionNum, &width, &height);

    InitSubMesh(parent, subMesh, width, height);

    return subMesh;
}


static void SetUVWithQuad(SubMesh* subMesh, Quad* quad)
{
    subMesh->drawable->width  = quad->width;
    subMesh->drawable->height = quad->height;

    AQuad->GetUV       (quad, subMesh->parent->texture, subMesh->uvArr->data);
    AQuad->GetPosition3(quad, subMesh->positionArr->data);

    Mesh* mesh = subMesh->parent;

    ALog_A(mesh != NULL, "ASubMesh SetUVWithQuad subMesh must has parent");

    memcpy
    (
        (char*)  mesh->vertexArr->data + mesh->uvDataOffset + subMesh->uvDataOffset,
        subMesh->uvArr->data,
        (size_t) subMesh->uvDataSize
    );

    if (AGraphics->isUseVBO)
    {
        VBOSubData* subData = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
        subData->target     = GL_ARRAY_BUFFER;
        subData->offset     = mesh->uvDataOffset + subMesh->uvDataOffset;
        subData->size       = subMesh->uvDataSize;
        subData->data       = subMesh->uvArr->data;
    }
}


static SubMesh* CreateWithQuad(Mesh* parent, Quad* quad)
{
    SubMesh* subMesh = malloc(sizeof(SubMesh) + Quad_IndexSize + Quad_UVSize + Quad_Position3Size);

    subMesh->indexArr->length    = Quad_IndexNum;
    subMesh->indexArr->data      = (char*) subMesh + sizeof(SubMesh);
    AQuad->GetIndex(0, subMesh->indexArr->data);

    subMesh->uvArr->length       = Quad_UVNum;
    subMesh->uvArr->data         = (char*) subMesh->indexArr->data + Quad_IndexSize;
    AQuad->GetUV(quad, parent->texture, subMesh->uvArr->data);

    subMesh->positionArr->length = Quad_Position3Num;
    subMesh->positionArr->data   = (char*) subMesh->uvArr->data    + Quad_UVSize;
    AQuad->GetPosition3(quad, subMesh->positionArr->data);

    subMesh->indexDataSize       = Quad_IndexSize;
    subMesh->uvDataSize          = Quad_UVSize;
    subMesh->positionDataSize    = Quad_Position3Size;

    InitSubMesh(parent, subMesh, quad->width, quad->height);

    return subMesh;
}


static inline void DeformApplyPositionUV(SubMesh *subMesh)
{
    Mesh* mesh = subMesh->parent;
    
    memcpy
    (
        (char*)  mesh->vertexArr->data + subMesh->positionDataOffset,
        subMesh->positionArr->data,
        (size_t) subMesh->positionDataSize
    );

    memcpy
    (
        (char*)  mesh->vertexArr->data + mesh->uvDataOffset + subMesh->uvDataOffset,
        subMesh->uvArr->data,
        (size_t) subMesh->uvDataSize
    );

    if (AGraphics->isUseVBO)
    {
        VBOSubData* subData = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
        subData->target     = GL_ARRAY_BUFFER;
        subData->offset     = subMesh->positionDataOffset;
        subData->size       = subMesh->positionDataSize;
        subData->data       = subMesh->positionArr->data;

        subData             = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
        subData->target     = GL_ARRAY_BUFFER;
        subData->offset     = mesh->uvDataOffset + subMesh->uvDataOffset;
        subData->size       = subMesh->uvDataSize;
        subData->data       = subMesh->uvArr->data;
    }
}


static inline void DeformApplyPosition(SubMesh *subMesh)
{
    Mesh* mesh = subMesh->parent;
    
    memcpy
    (
        (char*)  mesh->vertexArr->data + subMesh->positionDataOffset,
        subMesh->positionArr->data,
        (size_t) subMesh->positionDataSize
    );

    if (AGraphics->isUseVBO)
    {
        VBOSubData* subData = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
        subData->target     = GL_ARRAY_BUFFER;
        subData->offset     = subMesh->positionDataOffset;
        subData->size       = subMesh->positionDataSize;
        subData->data       = subMesh->positionArr->data;
    }
}


static inline void DeformApplyUV(SubMesh* subMesh)
{
    Mesh* mesh = subMesh->parent;
    
    memcpy
    (
        (char*)  mesh->vertexArr->data + mesh->uvDataOffset + subMesh->uvDataOffset,
        subMesh->uvArr->data,
        (size_t) subMesh->uvDataSize
    );

    if (AGraphics->isUseVBO)
    {
        VBOSubData* subData = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
        subData->target     = GL_ARRAY_BUFFER;
        subData->offset     = mesh->uvDataOffset + subMesh->uvDataOffset;
        subData->size       = subMesh->uvDataSize;
        subData->data       = subMesh->uvArr->data;
    }
}


#define CheckDeform(arr1, arr2)                                                           \
    ALog_A                                                                                \
    (                                                                                     \
        (arr1)->length == (arr2)->length,                                                 \
        "ASubMesh Deform the " #arr1 " length = %d must equals the " #arr2 " length = %d",\
        (arr1)->length,                                                                   \
        (arr2)->length                                                                    \
    )


static void Deform(SubMesh* subMesh, Array(float)* positionDeformArr, Array(float)* uvDeformArr)
{
    if (positionDeformArr != NULL && uvDeformArr != NULL)
    {
        CheckDeform(positionDeformArr, subMesh->positionArr);
        CheckDeform(uvDeformArr,       subMesh->uvArr);

        float* positions  = subMesh->positionArr->data;
        float* uvs        = subMesh->uvArr->data;
        float* posDeforms = positionDeformArr->data;
        float* uvDeforms  = uvDeformArr->data;
        
        for (int i = 0, j = 0; i < positionDeformArr->length; i += Mesh_VertexPositionNum, j += Mesh_VertexUVNum)
        {
            int       i1   = i + 1;
            int       i2   = i + 2;
            int       j1   = j + 1;

            positions[i]  += posDeforms[i];  // x
            positions[i1] += posDeforms[i1]; // y
            positions[i2] += posDeforms[i2]; // z

            uvs      [j]  += uvDeforms[j];   // u
            uvs      [j1] += uvDeforms[j1];  // v
        }

        DeformApplyPositionUV(subMesh);
    }
    else if (positionDeformArr != NULL)
    {
        CheckDeform(positionDeformArr, subMesh->positionArr);

        float* positions  = subMesh->positionArr->data;
        float* posDeforms = positionDeformArr->data;

        for (int i = 0; i < positionDeformArr->length; i += Mesh_VertexPositionNum)
        {
            int       i1   = i + 1;
            int       i2   = i + 2;

            positions[i]  += posDeforms[i];  // x
            positions[i1] += posDeforms[i1]; // y
            positions[i2] += posDeforms[i2]; // z
        }

        DeformApplyPosition(subMesh);
    }
    else if (uvDeformArr != NULL)
    {
        CheckDeform(uvDeformArr, subMesh->uvArr);

        float* uvs       = subMesh->uvArr->data;
        float* uvDeforms = uvDeformArr->data;

        for (int j = 0; j < uvDeformArr->length; j += Mesh_VertexUVNum)
        {
            int j1   = j + 1;
            uvs[j]  += uvDeforms[j];   // u
            uvs[j1] += uvDeforms[j1];  // v
        }

        DeformApplyUV(subMesh);
    }
    else
    {
        ALog_A(false, "ASubMesh Deform the positionDeformArr and uvDeformArr cannot both NULL");
    }
}


#define CheckDeformByIndex(arr1, arr2)                                                           \
    ALog_A                                                                                       \
    (                                                                                            \
        (arr1)->length == (arr2)->length,                                                        \
        "ASubMesh DeformByIndex the " #arr1 " length = %d must equals the " #arr2 " length = %d",\
        (arr1)->length,                                                                          \
        (arr2)->length                                                                           \
    )


#define CheckDeformByIndexLength(arr)                                                               \
    ALog_A                                                                                          \
    (                                                                                               \
        (arr)->length == length,                                                                    \
        "ASubMesh DeformByIndex " #arr " length = %d must equals the half length = %d of indexArr", \
        (arr)->length,                                                                              \
        indexArr->length                                                                            \
    )

static void DeformByIndex(SubMesh* subMesh, Array(float)* positionDeformArr, Array(float)* uvDeformArr, Array(int)* indexArr)
{
    if (positionDeformArr != NULL && uvDeformArr != NULL)
    {
        int length = indexArr->length >> 1; // NOLINT(hicpp-signed-bitwise)

        CheckDeformByIndexLength(positionDeformArr);
        CheckDeformByIndexLength(uvDeformArr);

        int*   indices    = indexArr->data;
        float* positions  = subMesh->positionArr->data;
        float* posDeforms = positionDeformArr->data;

        // position first
        for (int i = 0; i < length; ++i)
        {
            positions[indices[i]] += posDeforms[i];
        }

        float* uvs       = subMesh->uvArr->data;
        float* uvDeforms = uvDeformArr->data;

        // uv second
        for (int i = length; i < indexArr->length; ++i)
        {
            uvs[indices[i]] += uvDeforms[i - length];
        }

        DeformApplyPositionUV(subMesh);
    }
    else if (positionDeformArr != NULL)
    {
        CheckDeformByIndex(positionDeformArr,indexArr);

        float* positions  = subMesh->positionArr->data;
        float* posDeforms = positionDeformArr->data;
        int*   indices    = indexArr->data;

        for (int i = 0; i < indexArr->length; ++i)
        {
            positions[indices[i]] += posDeforms[i];
        }

        DeformApplyPosition(subMesh);
    }
    else if (uvDeformArr != NULL)
    {
        CheckDeformByIndex(uvDeformArr, indexArr);

        float* uvs       = subMesh->uvArr->data;
        float* uvDeforms = uvDeformArr->data;
        int*   indices   = indexArr->data;

        for (int i = 0; i < indexArr->length; ++i)
        {
            uvs[indices[i]] += uvDeforms[i];
        }

        DeformApplyUV(subMesh);
    }
    else
    {
        ALog_A(false, "ASubMesh DeformByIndex the positionDeformArr and uvDeformArr cannot both NULL");
    }
}


#undef CheckDeform
#undef CheckDeformByIndex
#undef CheckDeformByIndexLength


struct ASubMesh ASubMesh[1] =
{
    CreateWithData,
    CreateWithQuad,
    SetUVWithQuad,
    Deform,
    DeformByIndex,
};
