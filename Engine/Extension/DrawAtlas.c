/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2017-1-2017
 * Author: scott.cgi
 */

#include "Engine/Extension/DrawAtlas.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Toolkit/Head/Struct.h"
#include "Engine/Toolkit/Platform/Log.h"


static ArrayList(DrawAtlas*) drawAtlasList[1] = AArrayList_Init(DrawAtlas*, 10);


static DrawAtlas* Get(char* filePath)
{
    TextureAtlas* textureAtlas = ATextureAtlas->Get(filePath);

    ALog_A
    (
        textureAtlas->textureList->size == 1,
        "DrawAtlas not support TextureAtlas has multiple texture"
    );

    DrawAtlas* drawAtlas = AArrayList_Pop(drawAtlasList, DrawAtlas*);

    if (drawAtlas == NULL)
    {
        drawAtlas = (DrawAtlas*) malloc(sizeof(DrawAtlas));

        AMesh->InitWithCapacity
        (
            AArrayList_Get(textureAtlas->textureList, 0, Texture*),
            20,
            drawAtlas->mesh
        );

        AArrayList->Init
        (
            sizeof(Drawable*),
            drawAtlas->quadList
        );
    }
    else
    {
        AMesh     ->Clear(drawAtlas->mesh);
        AArrayList->Clear(drawAtlas->quadList);
    }

    drawAtlas->textureAtlas = textureAtlas;

    return drawAtlas;
}


static Drawable* GetQuad(DrawAtlas* drawAtlas, char* quadName)
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
            AStruct_GetParent2(drawable, SubMesh),
            drawAtlas->mesh->texture,
            atlasQuad->quad
        );
    }

    return drawable;
}


static void Reuse(DrawAtlas* drawAtlas)
{
    ALog_A(drawAtlas->textureAtlas != NULL, "ADrawAtlas Reuse drawAtlas %p already reused", drawAtlas);

    for (int i = 0; i < drawAtlas->quadList->size; i++)
    {
        ADrawable_SetInVisible
        (
            AArrayList_Get(drawAtlas->quadList, i, Drawable*)
        );
    }

    drawAtlas->textureAtlas = NULL;

    AArrayList_Add(drawAtlasList, drawAtlas);
}


static void ReuseQuad(DrawAtlas* drawAtlas, Drawable* drawable)
{
    SubMesh* subMesh = AStruct_GetParent2(drawable, SubMesh);

    ALog_A
    (
        drawAtlas->mesh == subMesh->parent,
        "ADrawAtlas ReuseQuad drawable %p not in this drawAtlas",
        drawable
    );

    ADrawable_SetInVisible(drawable);
    AArrayList_Add(drawAtlas->quadList, drawable);
}


struct ADrawAtlas ADrawAtlas[1] =
{
    Get,
    GetQuad,
    Reuse,
    ReuseQuad,
};