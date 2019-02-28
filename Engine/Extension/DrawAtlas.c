/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2017-1-2017
 * Update: 2019-2-1
 * Author: scott.cgi
 */


#include "Engine/Extension/DrawAtlas.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Toolkit/HeaderUtils/Struct.h"
#include "Engine/Toolkit/Platform/Log.h"


static ArrayList(DrawAtlas*) drawAtlasCacheList[1] = AArrayList_Init(DrawAtlas*, 10);


static DrawAtlas* Get(const char* filePath)
{
    TextureAtlas* textureAtlas = ATextureAtlas->Get(filePath);

    ALog_A
    (
        textureAtlas->textureList->size == 1,
        "DrawAtlas not support TextureAtlas has multiple texture"
    );

    DrawAtlas* drawAtlas = AArrayList_Pop(drawAtlasCacheList, DrawAtlas*);

    if (drawAtlas == NULL)
    {
        drawAtlas = malloc(sizeof(DrawAtlas));

        AMesh->InitWithCapacity
        (
            AArrayList_Get(textureAtlas->textureList, 0, Texture*),
            20,
            drawAtlas->mesh
        );

        AArrayList->Init(sizeof(Drawable*), drawAtlas->quadList);
    }
    else
    {
        AMesh     ->Clear(drawAtlas->mesh);
        AArrayList->Clear(drawAtlas->quadList);
    }

    drawAtlas->textureAtlas = textureAtlas;

    return drawAtlas;
}


static Drawable* GetQuad(DrawAtlas* drawAtlas, const char* quadName)
{
    TextureAtlasQuad* atlasQuad = ATextureAtlas_GetQuad(drawAtlas->textureAtlas, quadName);
    ALog_A(atlasQuad != NULL, "ADrawAtlas GetQuad not found quadName = %s", quadName);

    Drawable* drawable = AArrayList_Pop(drawAtlas->quadList, Drawable*);

    if (drawable == NULL)
    {
        drawable = AMesh->AddChildWithQuad(drawAtlas->mesh, atlasQuad->quad)->drawable;
        AMesh->GenerateBuffer(drawAtlas->mesh);
    }
    else
    {
        ADrawable->Init(drawable);

        ASubMesh->SetWithQuad
        (
            AStruct_GetParent(drawable, SubMesh),
            drawAtlas->mesh->texture,
            atlasQuad->quad
        );
    }

    return drawable;
}


static void Release(DrawAtlas* drawAtlas)
{
    ALog_A
    (
        drawAtlas->textureAtlas != NULL,
        "ADrawAtlas Release drawAtlas %p already released",
        drawAtlas
    );

    for (int i = 0; i < drawAtlas->quadList->size; ++i)
    {
        ADrawable_SetInvisible(AArrayList_Get(drawAtlas->quadList, i, Drawable*));
    }

    drawAtlas->textureAtlas = NULL;
    AArrayList_Add(drawAtlasCacheList, drawAtlas);
}


static void ReleaseQuad(DrawAtlas* drawAtlas, Drawable* drawable)
{
    SubMesh* subMesh = AStruct_GetParent(drawable, SubMesh);

    ALog_A
    (
        drawAtlas->mesh == subMesh->parent,
        "ADrawAtlas %s ReleaseQuad drawable not in this drawAtlas",
        drawAtlas->textureAtlas->filePath
    );

    ADrawable_SetInvisible(drawable);
    AArrayList_Add(drawAtlas->quadList, drawable);
}


struct ADrawAtlas ADrawAtlas[1] =
{
    Get,
    GetQuad,
    Release,
    ReleaseQuad,
};
