//
// Created by scott.cgi on 2017/1/5.
//

#ifndef draw_atlas_h
#define draw_atlas_h

#include "Mojoc/Extension/TextureAtlas.h"
#include "Mojoc/Graphics/OpenGL/Mesh.h"


typedef struct
{
    TextureAtlas*        textureAtlas;
    Mesh                 mesh    [1];
    ArrayList(Drawable*) quadList[1];
}
DrawAtlas;


struct ADrawAtlas
{
    DrawAtlas* (*Get)          (const char* filePath);
    Drawable*  (*GetQuad)      (DrawAtlas*  drawAtlas, const char* quadName);

    void       (*Reuse)        (DrawAtlas*  drawAtlas);
    void       (*ReuseQuad)    (DrawAtlas*  drawAtlas, Drawable* drawable);
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
