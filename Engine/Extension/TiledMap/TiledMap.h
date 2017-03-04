/*
 *  Created on: 2013-3-1
 *      Author: scott.cgi
 */

#ifndef tiled_map_h
#define tiled_map_h


#include "Mojoc/Toolkit/Utils/Array.h"
#include "Mojoc/Graphics/Draw/Drawable.h"
#include "Mojoc/Toolkit/Utils/ArrayStrMap.h"


typedef struct
{
	/** The layer array SubMesh in tilesetArr, key is layerName */
	ArrayStrMap
	(
		layerName,
		Array(SubMesh*)*
	)
	layerMap[1];

	/**
	 * Has been implemented render if override must call the original
	 */
	Drawable     drawable[1];
	Array(Mesh)* tilesetArr;
}
TiledMap;


struct ATiledMap
{
	TiledMap* (*Create) (const char* filePath);
	void      (*Release)(TiledMap*   tiledMap);

	/**
	 * TiledMap implement Drawable' render
	 */
	void      (*Render) (Drawable*   drawable);
};

extern struct ATiledMap ATiledMap[1];

#endif
