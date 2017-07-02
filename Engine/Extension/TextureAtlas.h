/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2016-7-22
 * Author : scott.cgi
 * Version: 0.0.0
 */


#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H


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


static TextureAtlasQuad* ATextureAtlas_GetQuad(TextureAtlas* atlas, char* quadName)
{
    return AArrayStrMap_GetPtr(atlas->quadMap, quadName, TextureAtlasQuad);
}


static Texture* ATextureAtlas_GetQuadTexture(TextureAtlasQuad* atlasQuad)
{
    return AArrayList_Get(atlasQuad->atlas->textureList, atlasQuad->textureIndex, Texture*);
}


#endif
