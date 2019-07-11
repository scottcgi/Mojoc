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
 * Since    : 2013-4-20
 * Update   : 2019-1-19
 * Author   : scott.cgi
 */


#include <stdlib.h>
#include <memory.h>

#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/Sprite.h"
#include "Engine/Toolkit/HeaderUtils/Struct.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderSprite.h"
#include "Engine/Graphics/Graphics.h"
#include "Engine/Graphics/OpenGL/GLTool.h"


static void Render(Drawable* drawable)
{
    Sprite* sprite = AStruct_GetParent(drawable, Sprite);

    AShaderSprite->Use(drawable->mvpMatrix, sprite->drawable->blendColor);

    glBindTexture(GL_TEXTURE_2D, sprite->texture->id);

    if (sprite->isDeformed)
    {
        sprite->isDeformed = false;
        
        if (AGraphics->isUseVBO)
        {
            // load the vertex data
            glBindBuffer(GL_ARRAY_BUFFER, sprite->vboIds[Sprite_BufferVertex]);

            // without vao state update sub data
            if (AGraphics->isUseMapBuffer)
            {
                void* mappedPtr = glMapBufferRange
                                  (
                                      GL_ARRAY_BUFFER,
                                      0,
                                      sprite->vertexDataSize,
                                      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT
                                  );

                memcpy(mappedPtr, sprite->vertexArr->data, (size_t) sprite->vertexDataSize);
                glUnmapBuffer(GL_ARRAY_BUFFER);
            }
            else
            {
                glBufferSubData(GL_ARRAY_BUFFER, 0, sprite->vertexDataSize , sprite->vertexArr->data);
            }

            if (AGraphics->isUseVAO)
            {
                // clear VBO bind
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                goto UseVAO;
            }
            
            goto UseVBO;
        }

        goto UseNormal;
    }

    if (AGraphics->isUseVAO)
    {
        UseVAO:
        
        glBindVertexArray(sprite->vaoId);
        glDrawElements(GL_TRIANGLES, sprite->indexCount, GL_UNSIGNED_SHORT, 0);
        // clear VAO bind
        glBindVertexArray(0);
    }
    else if (AGraphics->isUseVBO)
    {
        glBindBuffer(GL_ARRAY_BUFFER,         sprite->vboIds[Sprite_BufferVertex]);

        UseVBO:

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->vboIds[Sprite_BufferIndex]);

        // load the position and texture coordinate
        glVertexAttribPointer
        (
            (GLuint) AShaderSprite->attribPositionTexcoord,
            Sprite_VertexNum,
            GL_FLOAT,
            false,
            Sprite_VertexStride,
            0
        );

        glDrawElements(GL_TRIANGLES, sprite->indexCount, GL_UNSIGNED_SHORT, 0);

        // clear VBO bind
        glBindBuffer(GL_ARRAY_BUFFER,         0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else
    {
        UseNormal:
        
        // load the position and texture coordinate
        glVertexAttribPointer
        (
            (GLuint) AShaderSprite->attribPositionTexcoord,
            Sprite_VertexNum,
            GL_FLOAT,
            false,
            Sprite_VertexStride,
            sprite->vertexArr->data
        );

        glDrawElements(GL_TRIANGLES, sprite->indexCount, GL_UNSIGNED_SHORT, sprite->indexArr->data);
    }
}


static void Release(Sprite* sprite)
{
    free(sprite->vertexArr);
    free(sprite->indexArr);

    sprite->indexArr  = NULL;
    sprite->vertexArr = NULL;
    sprite->texture   = NULL;

    if (AGraphics->isUseVBO)
    {
        glDeleteBuffers(Sprite_BufferNum, sprite->vboIds);
        sprite->vboIds[Sprite_BufferVertex] = 0;
        sprite->vboIds[Sprite_BufferIndex]  = 0;

        if (AGraphics->isUseVAO)
        {
            glDeleteVertexArrays(1, &sprite->vaoId);
            sprite->vaoId = 0;
        }
    }
}


static inline void InitSprite(Sprite* sprite, Texture* texture, Array(Quad)* quadArr)
{
    Drawable* drawable = sprite->drawable;
    ADrawable->Init(drawable);

    // calculate and cache drawable mvp matrix
    ADrawable_AddState(drawable, DrawableState_IsUpdateMVPMatrix);

    AQuad->GetMaxSize(quadArr, &drawable->width, &drawable->height);

    sprite->texture                     = texture;
    sprite->uvWidth                     = AGLTool_ToUVWidth(drawable->width,  texture->width);
    sprite->uvHeight                    = AGLTool_ToUVWidth(drawable->height, texture->height);
    sprite->vboIds[Sprite_BufferVertex] = 0;
    sprite->vboIds[Sprite_BufferIndex]  = 0;
    sprite->vaoId                       = 0;
    sprite->indexCount                  = quadArr->length * Quad_IndexNum;
    sprite->vertexArr                   = AArray->Create(sizeof(float), quadArr->length * Quad_Position2UVNum);
    sprite->indexArr                    = AArray->Create(sizeof(short), sprite->indexCount);
    sprite->vertexDataSize              = sprite->vertexArr->length * sizeof(float);
    sprite->isDeformed                  = false;
    
    drawable->Render                    = Render;

    for (int i = 0; i < quadArr->length; ++i)
    {
        AQuad->GetPosition2UV
        (
            AArray_GetPtr(quadArr, i, Quad),
            texture,
            (float*) sprite->vertexArr->data + i * Quad_Position2UVNum
        );
        
        AQuad->GetIndex(i * Quad_VertexNum, (short*) sprite->indexArr->data + i * Quad_IndexNum);
    }

    if (AGraphics->isUseVBO)
    {
        if (sprite->vboIds[Sprite_BufferVertex] == 0)
        {
            glGenBuffers(Sprite_BufferNum, sprite->vboIds);
        }

        // vertex
        glBindBuffer(GL_ARRAY_BUFFER, sprite->vboIds[Sprite_BufferVertex]);
        glBufferData
        (
            GL_ARRAY_BUFFER,
            sprite->vertexDataSize,
            sprite->vertexArr->data,
            GL_STATIC_DRAW
        );

        // index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->vboIds[Sprite_BufferIndex]);
        glBufferData
        (
            GL_ELEMENT_ARRAY_BUFFER,
            sprite->indexArr->length * sizeof(short),
            sprite->indexArr->data,
            GL_STATIC_DRAW
        );

        if (AGraphics->isUseVAO)
        {
            if (sprite->vaoId == 0)
            {
                glGenVertexArrays(1, &sprite->vaoId);
            }

            glBindVertexArray(sprite->vaoId);

            // with vao has own state
            glBindBuffer(GL_ARRAY_BUFFER,         sprite->vboIds[Sprite_BufferVertex]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->vboIds[Sprite_BufferIndex]);
            glEnableVertexAttribArray((GLuint) AShaderSprite->attribPositionTexcoord);

            // load the position and texture coordinate
            glVertexAttribPointer
            (
                (GLuint) AShaderSprite->attribPositionTexcoord,
                Sprite_VertexNum,
                GL_FLOAT,
                false,
                Sprite_VertexStride,
                0
            );

            // go back to normal state
            glBindVertexArray(0);
        }
    }
}



#define CheckDeformLength(arr)                                                                      \
    ALog_A                                                                                          \
    (                                                                                               \
        (arr)->length == length,                                                                    \
        "ASprite Deform " #arr " length = %d must equals the half length = %d of sprite vertexArr", \
        (arr)->length,                                                                              \
        sprite->vertexArr->length                                                                   \
    )


static void Deform(Sprite* sprite, Array(float)* positionDeformArr, Array(float)* uvDeformArr)
{
    int    length   = sprite->vertexArr->length >> 1;
    float* vertices = sprite->vertexArr->data;

    if (positionDeformArr != NULL && uvDeformArr != NULL)
    {
        CheckDeformLength(positionDeformArr);
        CheckDeformLength(uvDeformArr);

        float* positions = positionDeformArr->data;
        float* uvs       = uvDeformArr->data;

        for (int i = 0; i < positionDeformArr->length; i += Sprite_VertexPositionNum)
        {
            int      i1      = i +  1;
            int      j       = i << 1;        // to vertexArr position index
            vertices[j]     += positions[i];  // x
            vertices[j + 1] += positions[i1]; // y
            vertices[j + 2] += uvs      [i];  // u
            vertices[j + 3] += uvs      [i1]; // v
        }
    }
    else if (positionDeformArr != NULL)
    {
        CheckDeformLength(positionDeformArr);
        
        float* positions = positionDeformArr->data;

        for (int i = 0; i < positionDeformArr->length; i += Sprite_VertexPositionNum)
        {
            int      j       = i << 1;           // to vertexArr position index
            vertices[j]     += positions[i];     // x
            vertices[j + 1] += positions[i + 1]; // y
        }
    }
    else if (uvDeformArr != NULL)
    {
        CheckDeformLength(uvDeformArr);

        float* uvs = uvDeformArr->data;

        for (int i = 0; i < uvDeformArr->length; i += Sprite_VertexPositionNum)
        {
            int      j       = i << 1;     // to vertexArr position index
            vertices[j + 2] += uvs[i];     // x
            vertices[j + 3] += uvs[i + 1]; // y
        }
    }
    else
    {
        ALog_A(false, "ASprite Deform the positionDeformArr and uvDeformArr cannot both NULL");
    }

    sprite->isDeformed = true;
}


#define CheckDeformByIndex(tag, len)                                                                  \
    ALog_A                                                                                            \
    (                                                                                                 \
        (len) <= length,                                                                              \
        "ASprite DeformByIndex the " tag " = %d must less than half length = %d of sprite vertexArr", \
        (len),                                                                                        \
        sprite->vertexArr->length                                                                     \
    )


static inline int GetPositionIndex(int index)
{
    int offset             = index % Sprite_VertexPositionNum; // (0 or 1), offset of one vertex data
    int positionStartIndex = (index - offset) << 1; // to vertexArr position start index

    // position index
    return positionStartIndex + offset;
}


static void DeformByIndex(Sprite* sprite, Array(float)* positionDeformArr, Array(float)* uvDeformArr, Array(int)* indexArr)
{
    int length = sprite->vertexArr->length >> 1;

    CheckDeformByIndex("indexArr length", indexArr->length);

    float* vertices = sprite->vertexArr->data;
    int*   indices  = indexArr->data;

    if (positionDeformArr != NULL && uvDeformArr != NULL)
    {
        CheckDeformByIndex("positionDeformArr length", positionDeformArr->length);
        CheckDeformByIndex("uvDeformArr length",       uvDeformArr->length);

        float* positions = positionDeformArr->data;
        float* uvs       = uvDeformArr->data;

        for (int i = 0; i < indexArr->length; ++i)
        {
            int      index                              = GetPositionIndex(indices[i]);
            vertices[index]                            += positions[i];
            vertices[index + Sprite_VertexPositionNum] += uvs      [i];
        }
    }
    else if (positionDeformArr != NULL)
    {
        CheckDeformByIndex("positionDeformArr length", positionDeformArr->length);

        float* positions = positionDeformArr->data;

        for (int i = 0; i < indexArr->length; ++i)
        {
            vertices[GetPositionIndex(indices[i])] += positions[i];
        }
    }
    else if (uvDeformArr != NULL)
    {
        CheckDeformByIndex("uvDeformArr length", uvDeformArr->length);

        float* uvs = uvDeformArr->data;

        for (int i = 0; i < indexArr->length; ++i)
        {
            vertices[GetPositionIndex(indices[i]) + Sprite_VertexPositionNum] += uvs[i];
        }
    }
    else
    {
        ALog_A(false, "ASprite DeformByIndex the positionDeformArr and uvDeformArr cannot both NULL");
    }

    sprite->isDeformed = true;
}


#undef CheckDeformLength
#undef CheckDeformByIndex


static void Init(Texture* texture, Sprite* outSprite)
{
    Quad quad[1];
    AQuad->Init(texture->width, texture->height, quad);
    InitSprite(outSprite, texture, (Array(Quad)[1]) {quad, 1}); // equals AArray_Make(Quad, 1, *quad)
}


static Sprite* Create(Texture* texture)
{
    Sprite* sprite = malloc(sizeof(Sprite));
    Init(texture, sprite);

    return sprite;
}


static void InitWithQuad(Texture* texture, Quad* quad, Sprite* outSprite)
{
    InitSprite(outSprite, texture, (Array(Quad)[1]) {quad, 1}); // equals AArray_Make(Quad, 1, *quad)
}


static Sprite* CreateWithQuad(Texture* texture, Quad* quad)
{
    Sprite* sprite = malloc(sizeof(Sprite));
    InitWithQuad(texture, quad, sprite);

    return sprite;
}


static Sprite* CreateWithQuadArray(Texture* texture, Array(Quad)* quadArr)
{
    Sprite* sprite = malloc(sizeof(Sprite));
    InitSprite(sprite, texture, quadArr);

    return sprite;
}


static void InitWithQuadArray(Texture* texture, Array(Quad)* quadArr, Sprite* outSprite)
{
    InitSprite(outSprite, texture, quadArr);
}


static Sprite* CreateWithFile(const char* resourceFilePath)
{
    return Create(ATexture->Get(resourceFilePath));
}


static void InitWithFile(const char* resourceFilePath, Sprite* outSprite)
{
    Init(ATexture->Get(resourceFilePath), outSprite);
}


struct ASprite ASprite[1] =
{{
    Create,
    Init,

    CreateWithFile,
    InitWithFile,

    CreateWithQuad,
    InitWithQuad,

    CreateWithQuadArray,
    InitWithQuadArray,

    Release,
    Deform,
    DeformByIndex,
    Render,
}};
