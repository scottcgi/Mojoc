/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2016-7-22
 * Update   : 2019-1-31
 * Author   : scott.cgi
 */


#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H


#include "Engine/Graphics/Draw/Quad.h"
#include "Engine/Toolkit/Utils/ArrayStrMap.h"


/**
 * The atlas including textures and quads info.
 */
typedef struct
{
    /**
     * Texture atlas quad info from file.
     */
    ArrayStrMap(quadName, TextureAtlasQuad) quadMap[1];

    /**
     * Textures in atlas.
     */
    ArrayList  (Texture*)                   textureList[1];

    /**
     * Be key identify TextureAtlas.
     */
    const char*                             filePath;
}
TextureAtlas;


/**
 * Quad in TextureAtlas.
 */
typedef struct
{
    /**
     * Quad belongs to.
     */
    TextureAtlas* atlas;

    /**
     * Quad data in texture.
     */
    Quad          quad[1];

    /**
     * The index of Quad's texture in TextureAtlas textureList.
     */
    int           textureIndex;
}
TextureAtlasQuad;


/**
 * Manage TextureAtlas.
 */
struct ATextureAtlas
{
    /**
     * Get TextureAtlas by TextureAtlas.
     *
     * filePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    TextureAtlas* (*Get)    (const char*   filePath);
    void          (*Release)(TextureAtlas* textureAtlas);
};


extern struct ATextureAtlas ATextureAtlas[1];


/**
 * Get TextureAtlasQuad from atlas by quadName.
 */
static inline TextureAtlasQuad* ATextureAtlas_GetQuad(TextureAtlas* atlas, const char* quadName)
{
    return AArrayStrMap_GetPtr(atlas->quadMap, quadName, TextureAtlasQuad);
}


/**
 * Get Texture from atlasQuad.
 */
static inline Texture* ATextureAtlas_GetQuadTexture(TextureAtlasQuad* atlasQuad)
{
    return AArrayList_Get(atlasQuad->atlas->textureList, atlasQuad->textureIndex, Texture*);
}


#endif
