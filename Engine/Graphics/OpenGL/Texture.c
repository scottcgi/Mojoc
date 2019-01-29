/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-1-2
 * Update: 2019-1-19
 * Author: scott.cgi
 */


#include <stdlib.h>
#include "Engine/Graphics/OpenGL/Texture.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Toolkit/Utils/ArrayStrMap.h"
#include "Engine/Toolkit/Platform/Log.h"


static ArrayStrMap(filePath, Texture*) textureCacheMap[1] = AArrayStrMap_Init(Texture*, 25);


static Texture* Get(const char* resourceFilePath)
{
    Texture* texture = AArrayStrMap_Get(textureCacheMap, resourceFilePath, Texture*);

    if (texture == NULL)
    {
        texture = malloc(sizeof(Texture));
        AGLTool->LoadTexture(resourceFilePath, texture);

        texture->filePath = AArrayStrMap_GetKey
                           (
                               AArrayStrMap_TryPut(textureCacheMap, resourceFilePath, texture),
                               textureCacheMap->valueTypeSize
                           );
    }

    return texture;
}


static void Destroy(Texture* texture)
{
    glDeleteTextures(1, (GLuint[]) {texture->id});

    bool isRemoved = AArrayStrMap->TryRemove(textureCacheMap, texture->filePath);
    ALog_A(isRemoved, "ATexture Destroy not found %s in textureCacheMap", texture->filePath);

    free(texture);
}


struct ATexture ATexture[1] =
{
    Get,
    Destroy,
};

