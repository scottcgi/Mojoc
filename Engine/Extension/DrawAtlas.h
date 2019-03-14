/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2017-1-5
 * Update: 2019-2-1
 * Author: scott.cgi
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
    TextureAtlas*        textureAtlas;

    /**
     * The Mesh that TextureAtlas used.
     */
    Mesh                 mesh[1];

    /**
     * Quads in texture which are SubMesh of Mesh.
     */
    ArrayList(Drawable*) quadList[1];
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
    Drawable*  (*GetQuad)      (DrawAtlas*  drawAtlas, const char* quadName);

    /**
     * Make drawAtlas can reuse in Get method.
     */
    void       (*Release)      (DrawAtlas* drawAtlas);

    /**
     * Make quad can reuse in GetQuad method.
     */
    void       (*ReleaseQuad)  (DrawAtlas* drawAtlas, Drawable* drawable);
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
