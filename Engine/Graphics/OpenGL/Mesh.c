/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2016-8-5
 * Update: 2019-1-23
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

        memcpy
        (
            (char*)  mesh->indexArr->data + indexDataOffset,
            subMesh->indexArr->data,
            (size_t) subMesh->indexDataSize
        );

        indexDataOffset += subMesh->indexDataSize;
    }

    if (AGraphics->isUseVBO)
    {
        // update all index data
        VBOSubData* subData = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
        subData->target     = GL_ELEMENT_ARRAY_BUFFER;
        subData->offset     = 0;
        subData->size       = mesh->indexDataSize;
        subData->data       = mesh->indexArr->data;
    }
}


static void Draw(Drawable* meshDrawable)
{
    Mesh* mesh             = AStruct_GetParentWithName  (meshDrawable, Mesh, drawable);
    bool  isChangedOpacity = ADrawable_CheckState(meshDrawable, DrawableState_OpacityChanged);
    bool  isChangedRGB     = ADrawable_CheckState(meshDrawable, DrawableState_RGBChanged);

    for (int i = 0; i < mesh->childList->size; ++i)
    {
        SubMesh* subMesh = AArrayList_Get(mesh->childList, i, SubMesh*);

        bool isDrawnBefore = ADrawable_CheckState(subMesh->drawable, DrawableState_DrawChanged);
        ADrawable->Draw(subMesh->drawable);
        bool isDrawnAfter  = ADrawable_CheckState(subMesh->drawable, DrawableState_DrawChanged);

        if (isDrawnAfter)
        {
            if (ADrawable_CheckState(subMesh->drawable, DrawableState_TransformChanged))
            {
                float* bornData     = subMesh->positionArr->data;
                float* positionData = (float*) ((char*) mesh->vertexArr->data + subMesh->positionDataOffset);

                // the born position data transformed (translate, scale, rotate) by SubMesh modelMatrix
                for (int j = 0; j < subMesh->positionArr->length; j += Mesh_VertexPositionNum)
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
                    subData->size       = subMesh->positionDataSize;
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
                    subData->size       = subMesh->vertexDataSize;
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
                    int index          = j * Mesh_VertexRGBNum;
                    rgbData[index]     = r;
                    rgbData[index + 1] = g;
                    rgbData[index + 2] = b;
                }

                if (AGraphics->isUseVBO)
                {
                    VBOSubData* subData = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
                    subData->target     = GL_ARRAY_BUFFER;
                    subData->offset     = mesh->rgbDataOffset  + subMesh->rgbDataOffset;
                    subData->size       = subMesh->positionDataSize; // equals rgbDataSize
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
                memset(opacityData, 0, (size_t) subMesh->vertexDataSize);
            }

            if (AGraphics->isUseVBO)
            {
                VBOSubData* subData = AArrayList_GetPtrAdd(mesh->vboSubDataList, VBOSubData);
                subData->target     = GL_ARRAY_BUFFER;
                subData->offset     = mesh->opacityDataOffset + subMesh->opacityDataOffset;
                subData->size       = subMesh->vertexDataSize; // equals opacityDataSize
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
        (GLuint) AShaderMesh->attribPosition,
        Mesh_VertexPositionNum,
        GL_FLOAT,
        false,
        Mesh_VertexPositionStride,
        0
    );

    // load the texture coordinate
    glVertexAttribPointer
    (
        (GLuint) AShaderMesh->attribTexcoord,
        Mesh_VertexUVNum,
        GL_FLOAT,
        false,
        Mesh_VertexUVStride,
        (GLvoid*) (intptr_t) mesh->uvDataOffset
    );

    // load the opacity
    glVertexAttribPointer
    (
        (GLuint) AShaderMesh->attribOpacity,
        Mesh_VertexOpacityNum,
        GL_FLOAT,
        false,
        Mesh_VertexOpacityStride,
        (GLvoid*) (intptr_t) mesh->opacityDataOffset
    );

    // load the rgb
    glVertexAttribPointer
    (
        (GLuint) AShaderMesh->attribRGB,
        Mesh_VertexRGBNum,
        GL_FLOAT,
        false,
        Mesh_VertexRGBStride,
        (GLvoid*) (intptr_t) mesh->rgbDataOffset
    );
}


static void Render(Drawable* drawable)
{
    Mesh* mesh = AStruct_GetParent(drawable, Mesh);

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

    // all children SubMesh under Mesh matrix
    AShaderMesh->Use(drawable->mvpMatrix);

    glBindTexture(GL_TEXTURE_2D, mesh->texture->id);

    // update SubMesh data in vbo
    if (mesh->vboSubDataList->size > 0)
    {
        // load the vertex data
        glBindBuffer(GL_ARRAY_BUFFER,         mesh->vboIds[Mesh_BufferVertex]);

        // load the vertex index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIds[Mesh_BufferIndex]);

        // without vao state update sub data
        if (AGraphics->isUseMapBuffer)
        {
            for (int i = 0; i < mesh->vboSubDataList->size; ++i)
            {
                VBOSubData* subData   = AArrayList_GetPtr(mesh->vboSubDataList, i, VBOSubData);
                void*       mappedPtr = glMapBufferRange
                                        (
                                            subData->target,
                                            subData->offset,
                                            subData->size,
                                            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT // NOLINT(hicpp-signed-bitwise)
                                        );

                memcpy(mappedPtr, subData->data, (size_t) subData->size);
                glUnmapBuffer(subData->target);
            }
        }
        else
        {
            for (int i = 0; i < mesh->vboSubDataList->size; ++i)
            {
                VBOSubData* subData = AArrayList_GetPtr(mesh->vboSubDataList, i, VBOSubData);
                glBufferSubData(subData->target, subData->offset, subData->size, subData->data);
            }
        }

        AArrayList->Clear(mesh->vboSubDataList);

        if (AGraphics->isUseVAO)
        {
            // clear VBO bind
            glBindBuffer(GL_ARRAY_BUFFER,         0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            goto UseVAO;
        }

        goto UseVBO;
    }

    if (AGraphics->isUseVAO) // if isUseVAO then the isUseVBO must be true
    {
        UseVAO:

        glBindVertexArray(mesh->vaoId);

        glDrawElements
        (
            GL_TRIANGLES,
            toChild->indexOffset - fromChild->indexOffset + toChild->indexArr->length,
            GL_UNSIGNED_SHORT,
            (GLvoid*) (intptr_t) fromChild->indexDataOffset // (intptr_t) for fix xcode warning
        );

        // clear VAO bind
        glBindVertexArray(0);
    }
    else if (AGraphics->isUseVBO)
    {
        // load the vertex data
        glBindBuffer(GL_ARRAY_BUFFER,         mesh->vboIds[Mesh_BufferVertex]);
        // load the vertex index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIds[Mesh_BufferIndex]);

        UseVBO:

        BindVBO(mesh);

        glDrawElements
        (
            GL_TRIANGLES,
            toChild->indexOffset - fromChild->indexOffset + toChild->indexArr->length,
            GL_UNSIGNED_SHORT,
            (GLvoid*) (intptr_t) fromChild->indexDataOffset // (intptr_t) for fix xcode warning
        );

        // clearAddChildWithData VBO bind
        glBindBuffer(GL_ARRAY_BUFFER,         0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else
    {
        // load the position
        glVertexAttribPointer
        (
            (GLuint) AShaderMesh->attribPosition,
            Mesh_VertexPositionNum,
            GL_FLOAT,
            false,
            Mesh_VertexPositionStride,
            mesh->vertexArr->data
        );

        // load the texture coordinate
        glVertexAttribPointer
        (
            (GLuint) AShaderMesh->attribTexcoord,
            Mesh_VertexUVNum,
            GL_FLOAT,
            false,
            Mesh_VertexUVStride,
            (char*) mesh->vertexArr->data + mesh->uvDataOffset
        );

        // load the opacity
        glVertexAttribPointer
        (
            (GLuint) AShaderMesh->attribOpacity,
            Mesh_VertexOpacityNum,
            GL_FLOAT,
            false,
            Mesh_VertexOpacityStride,
            (char*) mesh->vertexArr->data + mesh->opacityDataOffset
        );

        // load the rgb
        glVertexAttribPointer
        (
            (GLuint) AShaderMesh->attribRGB,
            Mesh_VertexRGBNum,
            GL_FLOAT,
            false,
            Mesh_VertexRGBStride,
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


static inline void ResetData(Mesh* mesh)
{
    mesh->vertexCount        = 0;
    mesh->vertexDataSize     = 0;
    mesh->indexDataSize      = 0;
    mesh->uvDataOffset       = 0;
    mesh->rgbDataOffset      = 0;
    mesh->opacityDataOffset  = 0;
    mesh->positionDataLength = 0;
    mesh->uvDataLength       = 0;
    mesh->rgbDataLength      = 0;
    mesh->opacityDataLength  = 0;
    mesh->indexDataLength    = 0;
}


static void Init(Texture* texture, Mesh* outMesh)
{
    Quad quad[1];
    AQuad->Init(texture->width, texture->height, quad);

    Drawable* drawable                 = outMesh->drawable;
    ADrawable->Init(drawable);

    // override
    drawable->Draw                     = Draw;
    drawable->Render                   = Render;

    ADrawable_AddState(drawable, DrawableState_IsUpdateMVPMatrix);

    outMesh->texture                   = texture;
    outMesh->vboIds[Mesh_BufferIndex]  = 0;
    outMesh->vboIds[Mesh_BufferVertex] = 0;

    outMesh->vaoId                     = 0;
    outMesh->vertexArr                 = NULL;
    outMesh->indexArr                  = NULL;

    ResetData(outMesh);

    AArrayQueue->Init(sizeof(int),        outMesh->drawRangeQueue);
    AArrayList ->Init(sizeof(SubMesh*),   outMesh->childList);
    AArrayList ->Init(sizeof(VBOSubData), outMesh->vboSubDataList);
    outMesh->vboSubDataList->increase   = outMesh->childList->increase * 4;
}

static inline void InitBuffer(Mesh* mesh)
{
    mesh->vertexArr         = AArray->Create
                              (
                                  sizeof(float),
                                  mesh->positionDataLength +
                                  mesh->uvDataLength       +
                                  mesh->opacityDataLength  +
                                  mesh->rgbDataLength
                              );
    mesh->indexArr          = AArray->Create(sizeof(short), mesh->indexDataLength);

    mesh->vertexDataSize    = mesh->vertexArr->length       * sizeof(float);
    mesh->indexDataSize     = mesh->indexArr->length        * sizeof(short);
    mesh->uvDataOffset      = mesh->positionDataLength      * sizeof(float);
    mesh->opacityDataOffset = mesh->uvDataOffset            + mesh->uvDataLength      * sizeof(float);
    mesh->rgbDataOffset     = mesh->opacityDataOffset       + mesh->opacityDataLength * sizeof(float);
    char* uvData            = (char*) mesh->vertexArr->data + mesh->uvDataOffset;

    for (int i = 0; i < mesh->childList->size; ++i)
    {
        SubMesh* subMesh = AArrayList_Get(mesh->childList, i, SubMesh*);

        memcpy
        (
            (char*)  mesh->indexArr->data + subMesh->indexDataOffset,
            subMesh->indexArr->data,
            (size_t) subMesh->indexDataSize
        );
        
        memcpy
        (
          (char*)  mesh->vertexArr->data  + subMesh->positionDataOffset,
          subMesh->positionArr->data,
          (size_t) subMesh->positionDataSize
        );

        memcpy(uvData + subMesh->uvDataOffset, subMesh->uvArr->data, (size_t) subMesh->uvDataSize);

        // make drawable rgb and opacity update to buffer
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
    Mesh* mesh = malloc(sizeof(Mesh));
    Init(texture, mesh);

    return mesh;
}


static inline SubMesh* AddChild(Mesh* mesh, SubMesh* subMesh)
{
    for (int i = 0; i < subMesh->indexArr->length; ++i)
    {
        // each child index add before children vertex count
        AArray_Get(subMesh->indexArr, i, short) += mesh->vertexCount;
    }

    subMesh->index              = mesh->childList->size;
    subMesh->parent             = mesh;

    subMesh->positionDataOffset = mesh->positionDataLength * sizeof(float);
    subMesh->uvDataOffset       = mesh->uvDataLength       * sizeof(float);
    subMesh->opacityDataOffset  = mesh->opacityDataLength  * sizeof(float);
    subMesh->rgbDataOffset      = mesh->rgbDataLength      * sizeof(float);
    subMesh->indexDataOffset    = mesh->indexDataLength    * sizeof(short);
    subMesh->indexOffset        = mesh->indexDataLength;

    mesh->vertexCount          += subMesh->vertexCount;
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
    return AddChild(mesh, ASubMesh->CreateWithData(positionArr, uvArr, indexArr));
}


static SubMesh* AddChildWithQuad(Mesh* mesh, Quad* quad)
{
    return AddChild(mesh, ASubMesh->CreateWithQuad(mesh->texture, quad));
}


static inline void ReleaseBuffer(Mesh* mesh)
{
    free(mesh->vertexArr);
    free(mesh->indexArr);

    mesh->vertexArr = NULL;
    mesh->indexArr  = NULL;

    if (AGraphics->isUseVBO)
    {
        glDeleteBuffers(Mesh_BufferNum, mesh->vboIds);
        mesh->vboIds[Mesh_BufferIndex]  = 0;
        mesh->vboIds[Mesh_BufferVertex] = 0;

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
        if (mesh->vboIds[Mesh_BufferVertex] == 0)
        {
            glGenBuffers(Mesh_BufferNum, mesh->vboIds);
        }

        // vertex
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vboIds[Mesh_BufferVertex]);
        glBufferData
        (
            GL_ARRAY_BUFFER,
            mesh->vertexDataSize,
            mesh->vertexArr->data,
            GL_DYNAMIC_DRAW
        );

        // index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIds[Mesh_BufferIndex]);
        glBufferData
        (
            GL_ELEMENT_ARRAY_BUFFER,
            mesh->indexDataSize,
            mesh->indexArr->data,
            GL_STATIC_DRAW
        );

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
            glBindBuffer(GL_ARRAY_BUFFER,         mesh->vboIds[Mesh_BufferVertex]);
            // load the vertex index
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIds[Mesh_BufferIndex]);

            glEnableVertexAttribArray((GLuint) AShaderMesh->attribPosition);
            glEnableVertexAttribArray((GLuint) AShaderMesh->attribTexcoord);
            glEnableVertexAttribArray((GLuint) AShaderMesh->attribOpacity);
            glEnableVertexAttribArray((GLuint) AShaderMesh->attribRGB);

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

    ResetData(mesh);
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
