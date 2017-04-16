/*
 *
 *
 *  Created on: 2012-12-29
 *  Author: scott.cgi
 */

#ifndef texture_h
#define texture_h


#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Head/MacroDefine.h"


/**
 * Use openGL world coordinate system
 */
typedef struct
{
	GLuint      get_only id;
	float       get_only width;
	float       get_only height;
	const char* get_only filePath;
}
Texture;


/**
 * Cache all texture with filePath as key
 */
struct ATexture
{
	/**
	 * Get Texture by filePath, not found will create one
	 */
	Texture*  (*Get)    (const char* filePath);

	/**
	 * Release texture memory both in GPU and CPU
	 * removed from cache
	 */
	void      (*Release)(Texture* texture);
};


extern struct ATexture ATexture[1];


#endif
