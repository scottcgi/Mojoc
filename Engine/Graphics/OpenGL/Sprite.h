/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2013-4-20
 * Author: scott.cgi
 */

#ifndef sprite_h
#define sprite_h


#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Graphics/Draw/Quad.h"
#include "Engine/Graphics/OpenGL/Texture.h"
#include "Engine/Toolkit/Utils/Array.h"


typedef struct
{
	Drawable      drawable[1];

    /**
     * Sprite render texture
     */
	Texture*      texture;

	/**
	 * All vertex index count
	 */
	int           indexCount;

	/**
	 * If use VBO is NULL else buffer all vertex data
	 */
	Array(float)* vertexArr;

	/**
	 * If use VBO is NULL else buffer all index data
	 */
	Array(short)* indexArr;

	/**
	 * If use VBO is array buffer id
	 */
	GLuint        vboIds[mesh_buffer_num];

	/**
	 * If use VAO is generated id else 0
	 */
	GLuint        vaoId;
}
Sprite;


struct ASprite
{
	Sprite* (*Create)             (Texture*    texture);
	void    (*Init)               (Texture*    texture,  Sprite* outSprite);

	Sprite* (*CreateWithFile)     (char*       filePath);
	void    (*InitWithFile)       (char*       filePath, Sprite* outSprite);

	Sprite* (*CreateWithQuad)     (Texture*    texture,  Quad*   quad);
	void    (*InitWithQuad)       (Texture*    texture,  Quad*   quad, Sprite* outSprite);


	Sprite* (*CreateWithQuadArray)(Texture*    texture, Array(Quad)* quadArr);
	void    (*InitWithQuadArray)  (Texture*    texture, Array(Quad)* quadArr, Sprite* outSprite);

	void    (*Release)            (Sprite*     sprite);

	/**
	 * Sprite implement Drawable's render
	 */
	void    (*Render)             (Drawable*   drawable);
};


extern struct ASprite ASprite[1];


static inline void ASpriteDraw(Sprite* sprite)
{
	ADrawable->Draw(sprite->drawable);
}


#endif
