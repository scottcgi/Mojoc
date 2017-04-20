/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2013-4-20
 * Author : scott.cgi
 */

#include <stdlib.h>

#include "Engine/Graphics/OpenGL/MeshDef.h"
#include "Engine/Graphics/OpenGL/Sprite.h"
#include "Engine/Toolkit/Head/Struct.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderSprite.h"
#include "Engine/Graphics/Graphics.h"


static void Render(Drawable* drawable)
{
	Sprite* sprite = AStructGetParent2(drawable, Sprite);

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
        glBindBuffer(GL_ARRAY_BUFFER,         sprite->vboIds[mesh_buffer_vertex]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->vboIds[mesh_buffer_index]);

        // load the position and texture coordinate
        glVertexAttribPointer
        (
            AShaderSprite->attribPositionTexcoord,
            mesh_vertex_size,
            GL_FLOAT,
            false,
            mesh_vertex_stride,
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
			mesh_vertex_size,
			GL_FLOAT,
			false,
			mesh_vertex_stride,
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
		glDeleteBuffers(mesh_buffer_num, sprite->vboIds);
		sprite->vboIds[mesh_buffer_vertex] = 0;
		sprite->vboIds[mesh_buffer_index]  = 0;

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
	ADrawableSetState(drawable, drawable_state_is_update_mvp);

	AQuad->MaxSize(quadArr, &drawable->width, &drawable->height);
	sprite->texture                    = texture;

	sprite->vboIds[mesh_buffer_vertex] = 0;
	sprite->vboIds[mesh_buffer_index]  = 0;
    sprite->vaoId                      = 0;

	sprite->indexCount                 = quadArr->length * quad_index_num;
	sprite->vertexArr                  = AArray->Create(sizeof(float), quadArr->length * quad_vertex_num);
	sprite->indexArr                   = AArray->Create(sizeof(short), sprite->indexCount);

	drawable->Render                   = Render;

	for (int i = 0; i < quadArr->length; i++)
	{
		AQuad->GetQuadVertex(AArrayGetPtr(quadArr, i, Quad), texture,  (float*) sprite->vertexArr->data + i * quad_vertex_num);
		AQuad->GetQuadIndex (i * 4, (short*) sprite->indexArr->data + i * quad_index_num);
	}

	if (AGraphics->isUseVBO)
	{
        if (sprite->vboIds[mesh_buffer_vertex] == 0)
        {
            glGenBuffers(mesh_buffer_num, sprite->vboIds);
        }

        // vertex
        glBindBuffer(GL_ARRAY_BUFFER, sprite->vboIds[mesh_buffer_vertex]);
        glBufferData(GL_ARRAY_BUFFER, sprite->vertexArr->length * sizeof(float), sprite->vertexArr->data, GL_DYNAMIC_DRAW);

        // index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->vboIds[mesh_buffer_index]);
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

            glBindBuffer(GL_ARRAY_BUFFER,         sprite->vboIds[mesh_buffer_vertex]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->vboIds[mesh_buffer_index]);
            glEnableVertexAttribArray(AShaderSprite->attribPositionTexcoord);

            // load the position and texture coordinate
            glVertexAttribPointer
            (
                AShaderSprite->attribPositionTexcoord,
                mesh_vertex_size,
                GL_FLOAT,
                false,
                mesh_vertex_stride,
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
