/*
 * TextureAtlas.h
 *
 *  Created on: 2016-7-22
 *      Author: scott.cgi
 */

#ifndef texture_atlas_h
#define texture_atlas_h

#include "Engine/Graphics/Draw/Quad.h"
#include "Engine/Toolkit/Utils/ArrayStrMap.h"


typedef struct
{
	/**
	 * Texture atlas quad info from file
	 */
	ArrayStrMap(atlasQuadName, TextureAtlasQuad) get_only quadMap[1];

	/**
	 * Textures in texture atlas
	 */
	ArrayList  (Texture*)                        get_only  textureList[1];

    /**
     * Texture file path be key cached in ArrayStrMap which collect all TextureAtlas
     */
	const char*                                  get_only  filePath;
}
TextureAtlas;


typedef struct
{
    TextureAtlas* get_only atlas;
    Quad          get_only quad[1];
    int           get_only textureIndex;
}
TextureAtlasQuad;


struct ATextureAtlas
{
    TextureAtlas* (*Get)    (const char*   filePath);
	void          (*Release)(TextureAtlas* textureAtlas);
};


extern struct ATextureAtlas ATextureAtlas[1];


static TextureAtlasQuad* ATextureAtlasGetQuad(TextureAtlas* atlas, const char* quadName)
{
    return AArrayStrMapGetPtr(atlas->quadMap, quadName, TextureAtlasQuad);
}


static Texture* ATextureAtlasGetQuadTexture(TextureAtlasQuad* atlasQuad)
{
    return AArrayListGet(atlasQuad->atlas->textureList, atlasQuad->textureIndex, Texture*);
}


#endif
