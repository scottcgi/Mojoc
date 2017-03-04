/*
 *
 *
 *  Created on: 2012-12-29
 *  Author: scott.cgi
 */

#ifndef texture_h
#define texture_h

#include "Mojoc/Graphics/OpenGL/Head/gl3.h"

/**
 * Use openGL world coordinate system
 */
typedef struct
{
	GLuint      id;
	float       width;
	float       height;
	const char* filePath;
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
	void      (*Release)(Texture*    texture);
};

extern struct ATexture ATexture[1];

#endif
