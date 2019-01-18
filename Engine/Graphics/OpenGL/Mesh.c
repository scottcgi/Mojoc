/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2016-8-5
 * Author: scott.cgi
 */

#include <string.h>
#include "Engine/Graphics/OpenGL/Mesh.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderMesh.h"
#include "Engine/Toolkit/HeaderUtils/Struct.h"
#include "Engine/Graphics/Graphics.h"


static void ReorderAllChildren(Mesh* mesh)
{
    ArrayList* children        = mesh->childList;
    // SubMesh keep original indexDataOffset
    int        indexDataOffset = 0;

    for (int i = 0; i < children->size; ++i)
    {
        SubMesh* subMesh = AArrayList_Get(children, i, SubMesh*);

        while (subMesh->index != i)
        {
            subMesh = AArrayList_Get(children, subMesh->index, SubMesh*);
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
        VBOSubData* subData = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
        subData->target     = GL_ELEMENT_ARRAY_BUFFER;
        subData->offset     = 0;
        subData->length     = mesh->indexArr->length * sizeof(short);
        subData->data       = mesh->indexArr->data;
    }
}


static void Draw(Drawable* meshDrawable)
{
    Mesh* mesh             = AStruct_GetParent3  (meshDrawable, Mesh, drawable);
    bool  isChangedOpacity = ADrawable_CheckState(meshDrawable, DrawableState_OpacityChanged);
    bool  isChangedRGB     = ADrawable_CheckState(meshDrawable, DrawableState_RGBChanged);

    for (int i = 0; i < mesh->childList->size; ++i)
    {
        SubMesh* subMesh = AArrayList_Get(mesh->childList, i, SubMesh*);

//----------------------------------------------------------------------------------------------------------------------

        bool isDrawnBefore = ADrawable_CheckState(subMesh->drawable, DrawableState_DrawChanged);
        ADrawable->Draw(subMesh->drawable);
        bool isDrawnAfter  = ADrawable_CheckState(subMesh->drawable, DrawableState_DrawChanged);

//----------------------------------------------------------------------------------------------------------------------

        if (isDrawnAfter)
        {
            if (ADrawable_CheckState(subMesh->drawable, DrawableState_TransformChanged))
            {
                float* bornData     = AArray_GetData(subMesh->positionArr, float);
                float* positionData = (float*) ((char*) mesh->vertexArr->data + subMesh->positionDataOffset);

                // the born position data transformed(translate, scale, rotate) by SubMesh modelMatrix
                for (int j = 0; j < subMesh->positionArr->length; j += MeshVertex_Position3Size)
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
                    VBOSubData* subData = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
                    subData->target     = GL_ARRAY_BUFFER;
                    subData->offset     = subMesh->positionDataOffset;
                    subData->length     = subMesh->positionArr->length * sizeof(float);
                    subData->data       = positionData;
                }
            }

//----------------------------------------------------------------------------------------------------------------------

            if (ADrawable_CheckState(subMesh->drawable, DrawableState_OpacityChanged) || isChangedOpacity)
            {
                float  opacity     = subMesh->drawable->blendColor->a * meshDrawable->blendColor->a;
                float* opacityData = (float*) (
                                                  (char*) mesh->vertexArr->data +
                                                  mesh->opacityDataOffset       +
                                                  subMesh->opacityDataOffset
                                              );

                for (int j = 0; j < subMesh->vertexCount; ++j)
                {
                    opacityData[j] = opacity;
                }

                if (AGraphics->isUseVBO)
                {
                    VBOSubData* subData = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
                    subData->target     = GL_ARRAY_BUFFER;
                    subData->offset     = mesh->opacityDataOffset + subMesh->opacityDataOffset;
                    subData->length     = subMesh->vertexCount * sizeof(float);
                    subData->data       = opacityData;
                }
            }

//----------------------------------------------------------------------------------------------------------------------

            if (ADrawable_CheckState(subMesh->drawable, DrawableState_RGBChanged) || isChangedRGB)
            {
                float  r       = subMesh->drawable->blendColor->r * meshDrawable->blendColor->r;
                float  g       = subMesh->drawable->blendColor->g * meshDrawable->blendColor->g;
                float  b       = subMesh->drawable->blendColor->b * meshDrawable->blendColor->b;

                float* rgbData = (float*) (
                                            (char*) mesh->vertexArr->data +
                                            mesh->rgbDataOffset           +
                                            subMesh->rgbDataOffset
                                          );

                for (int j = 0; j < subMesh->vertexCount; ++j)
                {
                    int index          = j * 3;
                    rgbData[index]     = r;
                    rgbData[index + 1] = g;
                    rgbData[index + 2] = b;
                }

                if (AGraphics->isUseVBO)
                {
                    VBOSubData* subData = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
                    subData->target     = GL_ARRAY_BUFFER;
                    subData->offset     = mesh->rgbDataOffset + subMesh->rgbDataOffset;
                    subData->length     = subMesh->vertexCount * 3 * sizeof(float);
                    subData->data       = rgbData;
                }
            }
        }

//----------------------------------------------------------------------------------------------------------------------

        // test visible changed
        if (isDrawnBefore != isDrawnAfter)
        {
            float* opacityData = (float*) (
                                                (char*) mesh->vertexArr->data +
                                              mesh->opacityDataOffset       +
                                              subMesh->opacityDataOffset
                                          );

            if (ADrawable_CheckState(subMesh->drawable, DrawableState_DrawChanged))
            {
                float opacity = subMesh->drawable->blendColor->a * meshDrawable->blendColor->a;
                for (int j = 0; j < subMesh->vertexCount; ++j)
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
                VBOSubData* subData = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
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
        MeshVertex_Position3Size,
        GL_FLOAT,
        false,
        MeshVertex_Position3Stride,
        0
    );

    // load the texture coordinate
    glVertexAttribPointer
    (
        AShaderMesh->attribTexcoord,
        MeshVertex_UVSize,
        GL_FLOAT,
        false,
        MeshVertex_UVStride,
        (GLvoid*) (intptr_t) mesh->uvDataOffset
    );

    // load the opacity
    glVertexAttribPointer
    (
        AShaderMesh->attribOpacity,
        MeshVertex_OpacitySize,
        GL_FLOAT,
        false,
        MeshVertex_OpacityStride,
        (GLvoid*) (intptr_t) mesh->opacityDataOffset
    );

    // load the rgb
    glVertexAttribPointer
    (
        AShaderMesh->attribRGB,
        MeshVertex_RGBSize,
        GL_FLOAT,
        false,
        MeshVertex_RGBStride,
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
        fromChild = AArrayList_Get(mesh->childList, mesh->fromIndex, SubMesh*);
        toChild   = AArrayList_Get(mesh->childList, mesh->toIndex,   SubMesh*);
    }
    else
    {
        fromChild = AArrayList_Get
                    (
                        mesh->childList,
                        AArrayQueue_PopWithDefault(mesh->drawRangeQueue, int, mesh->fromIndex),
                        SubMesh*
                    );

        toChild   = AArrayList_Get
                    (
                        mesh->childList,
                        AArrayQueue_PopWithDefault(mesh->drawRangeQueue, int, mesh->toIndex),
                        SubMesh*
                    );
    }

    // all SubMesh under Mesh matrix
    AShaderMesh->Use(drawable->mvpMatrix);

    glBindTexture(GL_TEXTURE_2D, mesh->texture->id);

//----------------------------------------------------------------------------------------------------------------------

    if (mesh->vboSubDataList->size > 0)
    {
        // load the vertex data
        glBindBuffer(GL_ARRAY_BUFFER,         mesh->vboIds[MeshBuffer_Vertex]);

        // load the vertex index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIds[MeshBuffer_Index]);

        // in no vao state update sub data
        if (AGraphics->isUseMapBuffer)
        {
            for (int i = 0; i < mesh->vboSubDataList->size; ++i)
            {
                VBOSubData* subData   = AArrayList_GetPtr(mesh->vboSubDataList, i, VBOSubData);
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
            for (int i = 0; i < mesh->vboSubDataList->size; ++i)
            {
                VBOSubData* subData = AArrayList_GetPtr(mesh->vboSubDataList, i, VBOSubData);
                glBufferSubData(subData->target, subData->offset, subData->length, subData->data);
            }
        }

        AArrayList->Clear(mesh->vboSubDataList);

        if (AGraphics->isUseVAO)
        {
            // clean VBO bind
            glBindBuffer(GL_ARRAY_BUFFER,         0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            goto UseVAO;
        }
        else if (AGraphics->isUseVBO)
        {
            goto UseVBO;
        }
    }

//----------------------------------------------------------------------------------------------------------------------

    if (AGraphics->isUseVAO)
    {

        UseVAO:

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
        glBindBuffer(GL_ARRAY_BUFFER,         mesh->vboIds[MeshBuffer_Vertex]);
        // load the vertex index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIds[MeshBuffer_Index]);

        UseVBO:

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
            MeshVertex_Position3Size,
            GL_FLOAT,
            false,
            MeshVertex_Position3Stride,
            mesh->vertexArr->data
        );

        // load the texture coordinate
        glVertexAttribPointer
        (
            AShaderMesh->attribTexcoord,
            MeshVertex_UVSize,
            GL_FLOAT,
            false,
            MeshVertex_UVStride,
            (char*) mesh->vertexArr->data + mesh->uvDataOffset
        );

        // load the opacity
        glVertexAttribPointer
        (
            AShaderMesh->attribOpacity,
            MeshVertex_OpacitySize,
            GL_FLOAT,
            false,
            MeshVertex_OpacityStride,
            (char*) mesh->vertexArr->data + mesh->opacityDataOffset
        );

        // load the rgb
        glVertexAttribPointer
        (
            AShaderMesh->attribRGB,
            MeshVertex_RGBSize,
            GL_FLOAT,
            false,
            MeshVertex_RGBStride,
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

    Drawable* drawable                  = outMesh->drawable;
    ADrawable->Init(drawable);

    // override
    drawable->Draw                      = Draw;
    drawable->Render                    = Render;

    ADrawable_AddState(drawable, DrawableState_IsUpdateMVP);

    outMesh->texture                    = texture;
    outMesh->vboIds[MeshBuffer_Index]   = 0;
    outMesh->vboIds[MeshBuffer_Vertex]  = 0;

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

    for (int i = 0; i < mesh->childList->size; ++i)
    {
        SubMesh* subMesh = AArrayList_Get(mesh->childList, i, SubMesh*);

        memcpy((char*) mesh->indexArr->data  + subMesh->indexDataOffset,    subMesh->indexArr->data,    subMesh->indexArr->length    * sizeof(short));
        memcpy((char*) mesh->vertexArr->data + subMesh->positionDataOffset, subMesh->positionArr->data, subMesh->positionArr->length * sizeof(float));
        memcpy(uvData                        + subMesh->uvDataOffset,       subMesh->uvArr->data,       subMesh->uvArr->length       * sizeof(float));

        // make drawable property update to buffer
        ADrawable_AddState(subMesh->drawable, DrawableState_Draw);
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
    for (int i = 0; i < subMesh->indexArr->length; ++i)
    {
        // each child index add before children vertex count
        AArray_Get(subMesh->indexArr, i, short) += mesh->vertexCountOffset;
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

    AArrayList_Add(mesh->childList, subMesh);

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
        glDeleteBuffers(MeshBuffer_Num, mesh->vboIds);
        mesh->vboIds[MeshBuffer_Index]  = 0;
        mesh->vboIds[MeshBuffer_Vertex] = 0;

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
        if (mesh->vboIds[MeshBuffer_Vertex] == 0)
        {
            glGenBuffers(MeshBuffer_Num, mesh->vboIds);
        }

        // vertex
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vboIds[MeshBuffer_Vertex]);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertexArr->length * sizeof(float), mesh->vertexArr->data, GL_DYNAMIC_DRAW);

        // index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIds[MeshBuffer_Index]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexArr->length * sizeof(short), mesh->indexArr->data, GL_STATIC_DRAW);

        // vertexArr and indexArr data pointer changed
        // so we clear all sub data update
        AArrayList->Clear(mesh->vboSubDataList);


        if (AGraphics->isUseVAO)
        {
            if (mesh->vaoId == 0)
            {
                glGenVertexArrays(1, &mesh->vaoId);
            }

            glBindVertexArray(mesh->vaoId);

            // with vao has own state

            // load the vertex data
            glBindBuffer(GL_ARRAY_BUFFER,         mesh->vboIds[MeshBuffer_Vertex]);
            // load the vertex index
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIds[MeshBuffer_Index]);

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

    for (int i = 0; i < mesh->childList->size; ++i)
    {
        free(AArrayList_Get(mesh->childList, i, SubMesh*));
    }

    AArrayList ->Release(mesh->childList);
    AArrayList ->Release(mesh->vboSubDataList);
    AArrayQueue->Release(mesh->drawRangeQueue);
}


static void Clear(Mesh* mesh)
{
    for (int i = 0; i < mesh->childList->size; ++i)
    {
        free(AArrayList_Get(mesh->childList, i, SubMesh*));
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
    ReorderAllChildren,
    GenerateBuffer,
    Render,
};
