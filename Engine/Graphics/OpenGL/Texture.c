/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2013-1-2
 * Author: scott.cgi
 */

#include <stdlib.h>
#include "Engine/Graphics/OpenGL/Texture.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Toolkit/Utils/ArrayStrMap.h"
#include "Engine/Toolkit/Platform/Log.h"


static ArrayStrMap(filePath, Texture*) textureMap[1] = AArrayStrMap_Init(Texture*, 20);


static void Release(Texture* texture)
{
	glDeleteTextures(1, (GLuint[]) {texture->id});

	bool isRemoved = AArrayStrMap->TryRemove(textureMap, texture->filePath);
    ALog_A(isRemoved, "Texture release not found %s in textureMap", texture->filePath);

    free(texture);
}


static Texture* Get(char* filePath)
{
	Texture* texture = AArrayStrMap_Get(textureMap, filePath, Texture*);

	if (texture == NULL)
	{
		texture           = (Texture*) malloc(sizeof(Texture));
		AGLTool->LoadTexture(filePath, texture);

		texture->filePath = AArrayStrMap_GetKey
							(
								AArrayStrMap_TryPut(textureMap, filePath, texture),
								textureMap->valueTypeSize
							);
	}

	return texture;
}


struct ATexture ATexture[1] =
{
	Get,
	Release,
};

