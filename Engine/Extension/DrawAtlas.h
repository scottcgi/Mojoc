/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2017-1-5
 * Author: scott.cgi
 */

#ifndef draw_atlas_h
#define draw_atlas_h

#include "Engine/Extension/TextureAtlas.h"
#include "Engine/Graphics/OpenGL/Mesh.h"


typedef struct
{
    /**
     * DrawAtlas used TextureAtlas
     */
    TextureAtlas*        textureAtlas;

    /**
     * TextureAtlas used mesh
     */
    Mesh                 mesh    [1];

    /**
     * Quad in mesh texture
     */
    ArrayList(Drawable*) quadList[1];
}
DrawAtlas;


struct ADrawAtlas
{
    DrawAtlas* (*Get)          (char*       filePath);
    Drawable*  (*GetQuad)      (DrawAtlas*  drawAtlas, char*     quadName);

    /**
     * Make drawAtlas can reuse in Get method
     */
    void       (*Reuse)        (DrawAtlas*  drawAtlas);

    /**
     * Make quad can reuse in GetQuad method
     */
    void       (*ReuseQuad)    (DrawAtlas* drawAtlas, Drawable* drawable);
};


extern struct ADrawAtlas ADrawAtlas[1];


static inline void ADrawAtlasDraw(DrawAtlas* drawAtlas)
{
    AMeshDraw(drawAtlas->mesh);
}


static inline Drawable* ADrawAtlasGetDrawable(DrawAtlas* drawAtlas)
{
    return drawAtlas->mesh->drawable;
}


#endif
