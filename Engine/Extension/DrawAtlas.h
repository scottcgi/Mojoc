/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2017-1-5
 * Update   : 2019-2-1
 * Author   : scott.cgi
 */


#ifndef DRAW_ATLAS_H
#define DRAW_ATLAS_H


#include "Engine/Extension/TextureAtlas.h"
#include "Engine/Graphics/OpenGL/Mesh.h"


/**
 * Draw with TextureAtlas.
 */
typedef struct
{
    /**
     * The TextureAtlas that DrawAtlas used.
     */
    TextureAtlas*       textureAtlas;

    /**
     * The Mesh that TextureAtlas used.
     */
    Mesh                mesh[1];

    /**
     * Quads in texture which are SubMesh of Mesh.
     */
    ArrayList(SubMesh*) quadList[1];
}
DrawAtlas;


struct ADrawAtlas
{
    /**
     * Get DrawAtlas by filePath.
     *
     * filePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    DrawAtlas* (*Get)          (const char* filePath);
    SubMesh*   (*GetQuad)      (DrawAtlas*  drawAtlas, const char* quadName);

    /**
     * Make drawAtlas can reuse in Get method.
     */
    void       (*Release)      (DrawAtlas* drawAtlas);

    /**
     * Make quad can reuse in GetQuad method.
     */
    void       (*ReleaseQuad)  (DrawAtlas* drawAtlas, SubMesh* subMesh);
};


extern struct ADrawAtlas ADrawAtlas[1];


/**
 * Draw the mesh of atlas.
 */
static inline void ADrawAtlas_Draw(DrawAtlas* drawAtlas)
{
    AMesh_Draw(drawAtlas->mesh);
}


#endif
