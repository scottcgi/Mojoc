/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The Mojoc game engine is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2017-1-2017
 * Update: 2019-2-1
 * Author: scott.cgi
 */


#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Extension/DrawAtlas.h"
#include "Engine/Toolkit/HeaderUtils/Struct.h"
#include "Engine/Toolkit/Platform/Log.h"


static ArrayList(DrawAtlas*) drawAtlasCacheList[1] = AArrayList_Init(DrawAtlas*, 10);


static DrawAtlas* Get(const char* filePath)
{
    TextureAtlas* textureAtlas = ATextureAtlas->Get(filePath);

    ALog_A
    (
        textureAtlas->textureList->size == 1,
        "DrawAtlas not support TextureAtlas has multiple textures"
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

        AArrayList->InitWithCapacity(sizeof(SubMesh*), 20, drawAtlas->quadList);
    }
    else
    {
        AMesh     ->Clear(drawAtlas->mesh);
        AArrayList->Clear(drawAtlas->quadList);
    }

    drawAtlas->textureAtlas = textureAtlas;

    return drawAtlas;
}


static SubMesh* GetQuad(DrawAtlas* drawAtlas, const char* quadName)
{
    TextureAtlasQuad* atlasQuad = ATextureAtlas_GetQuad(drawAtlas->textureAtlas, quadName);
    ALog_A(atlasQuad != NULL, "ADrawAtlas GetQuad not found quadName = %s", quadName);

    SubMesh* subMesh = AArrayList_Pop(drawAtlas->quadList, SubMesh*);

    if (subMesh == NULL)
    {
        // cache more quads into quadList when cache empty
        for (int i = 0; i < 5; ++i)
        {
            subMesh = AMesh->AddChildWithQuad(drawAtlas->mesh, atlasQuad->quad);
            ADrawable_SetInvisible(subMesh->drawable);
            AArrayList_Add(drawAtlas->quadList, subMesh);
        }

        subMesh = AMesh->AddChildWithQuad(drawAtlas->mesh, atlasQuad->quad);
        AMesh->GenerateBuffer(drawAtlas->mesh);
    }
    else
    {
        ADrawable->Init(subMesh->drawable);

        ASubMesh->SetUVWithQuad
        (
            subMesh,
            atlasQuad->quad
        );
    }

    return subMesh;
}


static void Release(DrawAtlas* drawAtlas)
{
    ALog_A
    (
        drawAtlas->textureAtlas != NULL,
        "ADrawAtlas Release drawAtlas %p already released",
        drawAtlas
    );

    // quadList clear by Get function
    for (int i = 0; i < drawAtlas->quadList->size; ++i)
    {
        ADrawable_SetInvisible(AArrayList_Get(drawAtlas->quadList, i, SubMesh*)->drawable);
    }

    drawAtlas->textureAtlas = NULL;
    AArrayList_Add(drawAtlasCacheList, drawAtlas);
}


static void ReleaseQuad(DrawAtlas* drawAtlas, SubMesh* subMesh)
{
    ALog_A
    (
        drawAtlas->mesh == subMesh->parent,
        "ADrawAtlas %s ReleaseQuad drawable not in this drawAtlas",
        drawAtlas->textureAtlas->filePath
    );

    ADrawable_SetInvisible(subMesh->drawable);
    AArrayList_Add(drawAtlas->quadList, subMesh);
}


struct ADrawAtlas ADrawAtlas[1] =
{
    Get,
    GetQuad,
    Release,
    ReleaseQuad,
};
