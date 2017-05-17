/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-4-20
 * Author: scott.cgi
 * Version: 0.1
 */


#include <stdlib.h>

#include "Engine/Graphics/OpenGL/MeshDefine.h"
#include "Engine/Graphics/OpenGL/Sprite.h"
#include "Engine/Toolkit/Head/Struct.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderSprite.h"
#include "Engine/Graphics/Graphics.h"


static void Render(Drawable* drawable)
{
	Sprite* sprite = AStruct_GetParent2(drawable, Sprite);

	AShaderSprite->Use(drawable->mvpMatrix, sprite->drawable->blendColor);

	glBindTexture(GL_TEXTURE_2D, sprite->texture->id);

//--------------------------------------------------------------------------------------------------

    if (AGraphics->isUseVAO)
    {
        glBindVertexArray(sprite->vaoId);
        glDrawElements(GL_TRIANGLES, sprite->indexCount, GL_UNSIGNED_SHORT, 0);
        // clear VAO bind
        glBindVertexArray(0);
    }
    else if (AGraphics->isUseVBO)
    {
        glBindBuffer(GL_ARRAY_BUFFER,         sprite->vboIds[MeshBuffer_Vertex]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->vboIds[MeshBuffer_Index]);

        // load the position and texture coordinate
        glVertexAttribPointer
        (
            AShaderSprite->attribPositionTexcoord,
            MeshVertex_Size,
            GL_FLOAT,
            false,
            MeshVertex_VertexStride,
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
			MeshVertex_Size,
			GL_FLOAT,
			false,
			MeshVertex_VertexStride,
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
		glDeleteBuffers(MeshBuffer_Num, sprite->vboIds);
		sprite->vboIds[MeshBuffer_Vertex] = 0;
		sprite->vboIds[MeshBuffer_Index]  = 0;

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
	ADrawable_SetState(drawable, DrawableState_IsUpdateMVP);

	AQuad->MaxSize(quadArr, &drawable->width, &drawable->height);
	sprite->texture                    = texture;

	sprite->vboIds[MeshBuffer_Vertex] = 0;
	sprite->vboIds[MeshBuffer_Index]  = 0;
    sprite->vaoId                      = 0;

	sprite->indexCount                 = quadArr->length * Quad_IndexNum;
	sprite->vertexArr                  = AArray->Create(sizeof(float), quadArr->length * Quad_VertexNum);
	sprite->indexArr                   = AArray->Create(sizeof(short), sprite->indexCount);

	drawable->Render                   = Render;

	for (int i = 0; i < quadArr->length; i++)
	{
		AQuad->GetQuadVertex(AArray_GetPtr(quadArr, i, Quad), texture,  (float*) sprite->vertexArr->data + i * Quad_VertexNum);
		AQuad->GetQuadIndex (i * 4, (short*) sprite->indexArr->data + i * Quad_IndexNum);
	}

	if (AGraphics->isUseVBO)
	{
        if (sprite->vboIds[MeshBuffer_Vertex] == 0)
        {
            glGenBuffers(MeshBuffer_Num, sprite->vboIds);
        }

        // vertex
        glBindBuffer(GL_ARRAY_BUFFER, sprite->vboIds[MeshBuffer_Vertex]);
        glBufferData(GL_ARRAY_BUFFER, sprite->vertexArr->length * sizeof(float), sprite->vertexArr->data, GL_DYNAMIC_DRAW);

        // index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->vboIds[MeshBuffer_Index]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sprite->indexArr->length * sizeof(short), sprite->indexArr->data, GL_STATIC_DRAW);

//--------------------------------------------------------------------------------------------------

        if (AGraphics->isUseVAO)
        {
            if (sprite->vaoId == 0)
            {
                glGenVertexArrays(1, &sprite->vaoId);
            }

            glBindVertexArray(sprite->vaoId);

/*
----------------------------------------------------------------------------------------------------
	with vao has own state
--------------------------------------------------------------------------------------------------
*/

            glBindBuffer(GL_ARRAY_BUFFER,         sprite->vboIds[MeshBuffer_Vertex]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->vboIds[MeshBuffer_Index]);
            glEnableVertexAttribArray(AShaderSprite->attribPositionTexcoord);

            // load the position and texture coordinate
            glVertexAttribPointer
            (
                AShaderSprite->attribPositionTexcoord,
                MeshVertex_Size,
                GL_FLOAT,
                false,
                MeshVertex_VertexStride,
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
	Sprite* sprite = (Sprite*) malloc(sizeof(Sprite));
	Init(texture, sprite);

	return sprite;
}


static void InitWithQuad(Texture* texture, Quad* quad, Sprite* outSprite)
{
	InitSprite(outSprite, texture, (Array[]) {quad, 1});
}


static Sprite* CreateWithQuad(Texture* texture, Quad* quad)
{
	Sprite* sprite = (Sprite*) malloc(sizeof(Sprite));
	InitWithQuad(texture, quad, sprite);

	return sprite;
}


static Sprite* CreateWithQuadArray(Texture* texture, Array(Quad)* quadArr)
{
	Sprite* sprite = (Sprite*) malloc(sizeof(Sprite));
	InitSprite(sprite, texture, quadArr);

	return sprite;
}


static void InitWithQuadArray(Texture* texture, Array(Quad)* quadArr, Sprite* outSprite)
{
	InitSprite(outSprite, texture, quadArr);
}


static Sprite* CreateWithFile(char* filePath)
{
	return Create(ATexture->Get(filePath));
}


static void InitWithFile(char* filePath, Sprite* outSprite)
{
	Init(ATexture->Get(filePath), outSprite);
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
