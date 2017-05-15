/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2016-7-22
 * Author: scott.cgi
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
	ArrayStrMap(atlasQuadName, TextureAtlasQuad) quadMap[1];

	/**
	 * Textures in texture atlas
	 */
	ArrayList  (Texture*)                        textureList[1];

    /**
     * Texture file path be key cached in ArrayStrMap which collect all TextureAtlas
     */
	char*                                        filePath;
}
TextureAtlas;


typedef struct
{
    /**
     * Quad's texture belongs TextureAtlas
     */
    TextureAtlas* atlas;

    /**
     * Quad data in texture
     */
    Quad          quad[1];

    /**
     * Quad's texture index in TextureAtlas
     */
    int           textureIndex;
}
TextureAtlasQuad;


struct ATextureAtlas
{
    TextureAtlas* (*Get)    (char*         filePath);
	void          (*Release)(TextureAtlas* textureAtlas);
};


extern struct ATextureAtlas ATextureAtlas[1];


static TextureAtlasQuad* ATextureAtlasGetQuad(TextureAtlas* atlas, char* quadName)
{
    return AArrayStrMap_GetPtr(atlas->quadMap, quadName, TextureAtlasQuad);
}


static Texture* ATextureAtlasGetQuadTexture(TextureAtlasQuad* atlasQuad)
{
    return AArrayList_Get(atlasQuad->atlas->textureList, atlasQuad->textureIndex, Texture*);
}


#endif
