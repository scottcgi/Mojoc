/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-4-20
 * Update: 2019-1-19
 * Author: scott.cgi
 */


#include <stdlib.h>
#include "Engine/Graphics/OpenGL/Sprite.h"
#include "Engine/Toolkit/HeaderUtils/Struct.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderSprite.h"
#include "Engine/Graphics/Graphics.h"


static void Render(Drawable* drawable)
{
    Sprite* sprite = AStruct_GetParent(drawable, Sprite);

    AShaderSprite->Use(drawable->mvpMatrix, sprite->drawable->blendColor);

    glBindTexture(GL_TEXTURE_2D, sprite->texture->id);

//----------------------------------------------------------------------------------------------------------------------

    if (AGraphics->isUseVAO)
    {
        glBindVertexArray(sprite->vaoId);
        glDrawElements(GL_TRIANGLES, sprite->indexCount, GL_UNSIGNED_SHORT, 0);
        // clear VAO bind
        glBindVertexArray(0);
    }
    else if (AGraphics->isUseVBO)
    {
        glBindBuffer(GL_ARRAY_BUFFER,         sprite->vboIds[Sprite_BufferVertex]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->vboIds[Sprite_BufferIndex]);

        // load the position and texture coordinate
        glVertexAttribPointer
        (
            AShaderSprite->attribPositionTexcoord,
            Sprite_VertexSize,
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
        // load the position and texture coordinate
        glVertexAttribPointer
        (
            AShaderSprite->attribPositionTexcoord,
            Sprite_VertexSize,
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
    ADrawable_AddState(drawable, DrawableState_IsUpdateMVP);

    AQuad->GetMaxSize(quadArr, &drawable->width, &drawable->height);

    sprite->texture                   = texture;

    sprite->vboIds[Sprite_BufferVertex] = 0;
    sprite->vboIds[Sprite_BufferIndex]  = 0;
    sprite->vaoId                     = 0;

    sprite->indexCount                = quadArr->length * Quad_IndexNum;
    sprite->vertexArr                 = AArray->Create(sizeof(float),  quadArr->length * Quad_Position2UVNum);
    sprite->indexArr                  = AArray->Create(sizeof(short), sprite->indexCount);

    drawable->Render                  = Render;

    for (int i = 0; i < quadArr->length; ++i)
    {
        AQuad->GetPosition2UV
        (
            AArray_GetPtr(quadArr, i, Quad),
            texture,
            (float*) sprite->vertexArr->data + i * Quad_Position2UVNum
        );
        
        AQuad->GetIndex (i * 4, (short*) sprite->indexArr->data + i * Quad_IndexNum);
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
            sprite->vertexArr->length * sizeof(float),
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

//----------------------------------------------------------------------------------------------------------------------

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
            glEnableVertexAttribArray(AShaderSprite->attribPositionTexcoord);

            // load the position and texture coordinate
            glVertexAttribPointer
            (
                AShaderSprite->attribPositionTexcoord,
                Sprite_VertexSize,
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


static void Init(Texture* texture, Sprite* outSprite)
{
    Quad quad[1];
    AQuad->Init(texture->width, texture->height, quad);
    InitSprite(outSprite, texture, (Array[]) {quad, 1});
}


static Sprite* Create(Texture* texture)
{
    Sprite* sprite = malloc(sizeof(Sprite));
    Init(texture, sprite);

    return sprite;
}


static void InitWithQuad(Texture* texture, Quad* quad, Sprite* outSprite)
{
    InitSprite(outSprite, texture, (Array[]) {quad, 1});
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
{
    Create,
    Init,

    CreateWithFile,
    InitWithFile,

    CreateWithQuad,
    InitWithQuad,

    CreateWithQuadArray,
    InitWithQuadArray,

    Release,
    Render,
};
