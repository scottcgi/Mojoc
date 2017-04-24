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


static ArrayStrMap(filePath, Texture*) textureMap[1] = AArrayStrMapInit(Texture*, 20);


static void Release(Texture* texture)
{
	glDeleteTextures(1, (GLuint[]) {texture->id});

	bool isRemoved = AArrayStrMap->TryRemove(textureMap, texture->filePath);
    ALogA(isRemoved, "Texture release not found %s in textureMap", texture->filePath);

    free(texture);
}


static Texture* Get(char* filePath)
{
	Texture* texture = AArrayStrMapGet(textureMap, filePath, Texture*);

	if (texture == NULL)
	{
		texture           = (Texture*) malloc(sizeof(Texture));
		AGLTool->LoadTexture(filePath, texture);

		texture->filePath = AArrayStrMapGetKey
							(
								AArrayStrMapPut(textureMap, filePath, texture),
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

