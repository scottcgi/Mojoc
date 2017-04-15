/*
 *
 *
 *  Created on: 2013-4-20
 *      Author: scott.cgi
 */

#ifndef sprite_h
#define sprite_h


#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Graphics/Draw/Quad.h"
#include "Engine/Graphics/OpenGL/Texture.h"
#include "Engine/Toolkit/Utils/Array.h"


typedef struct
{
	Drawable               drawable[1];
	Texture*               texture;

	/**
	 * All vertex index count
	 */
	int           get_only indexCount;

	/**
	 * If use VBO is NULL else buffer all vertex data
	 */
	Array(float)* get_only vertexArr;

	/**
	 * If use VBO is NULL else buffer all index data
	 */
	Array(short)* get_only indexArr;

	/**
	 * If use VBO is array buffer id
	 */
	GLuint        get_only vboIds[mesh_buffer_num];

	/**
	 * If use VAO is generated id else 0
	 */
	GLuint        get_only vaoId;
}
Sprite;


struct ASprite
{
	Sprite* (*Create)             (Texture*    texture);
	void    (*Init)               (Texture*    texture,  Sprite* out_param sprite);

	Sprite* (*CreateWithFile)     (const char* filePath);
	void    (*InitWithFile)       (const char* filePath, Sprite* out_param sprite);

	Sprite* (*CreateWithQuad)     (Texture*    texture,  Quad*   quad);
	void    (*InitWithQuad)       (Texture*    texture,  Quad*   quad, Sprite* out_param sprite);


	Sprite* (*CreateWithQuadArray)(Texture*    texture, Array(Quad)* quadArr);
	void    (*InitWithQuadArray)  (Texture*    texture, Array(Quad)* quadArr, Sprite* out_param sprite);

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
