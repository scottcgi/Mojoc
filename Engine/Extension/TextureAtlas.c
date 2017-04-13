/*
 * TextureAtlas.c
 *
 *  Created on: 2016-7-22
 *      Author: scott.cgi
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Engine/Toolkit/Head/MacroDefine.h"
#include "Engine/Extension/TextureAtlas.h"
#include "Engine/Toolkit/Utils/BufferReader.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Toolkit/Utils/FileTool.h"


static ArrayStrMap(filePath, TextureAtlas*) textureAtlasMap[1] = AArrayStrMapInit(TextureAtlas*, 10);

static inline void ReadFind(char* buffer, ArrayRange* range, ArrayRange* line, const char* str)
{
	ABufferReader->ReadLine(buffer, range, line);
	bool isFound = ABufferReader->TryFindString(buffer, line, str);
	ALogA(isFound, "Can't find string = %s", str);
}

#define Read(str) ReadFind(buffer, range, line, str)

static void Init(const char* filePath, TextureAtlas* outTextureAtlas)
{
	AArrayStrMap->InitWithCapacity(sizeof(TextureAtlasQuad),   20, outTextureAtlas->quadMap);
	AArrayList  ->InitWithCapacity(sizeof(Texture*),           5,  outTextureAtlas->textureList);

	long  size;
	char* buffer = AFileTool->ReadBufferPlatform(filePath, &size);

	ArrayRange range[1] = {0, size - 1};
	ArrayRange line [1];

	ABufferReader->ReadLine(buffer, range, line);

	while (range->start < range->end)
	{
		// read image name
		ABufferReader->ReadLine(buffer, range, line);

		int  fileDirLen = AFileTool->GetDirLength(filePath);
		int  pathLen    = fileDirLen + (line->end - line->start);
		char path[pathLen + 1];

		path[pathLen] = '\0';

		if (fileDirLen != 0)
		{
			memcpy(path, filePath, fileDirLen);
			memcpy(path + fileDirLen, buffer + line->start, pathLen - fileDirLen);
		}

		ALogD("texture atlas image = %s", path);

		Texture* texture = ATexture->Get(path);
		AArrayListAdd(outTextureAtlas->textureList, texture);

		Read("size");
		Read("format");
		Read("filter");
		Read("repeat");

		while (true)
		{
			// texture quad name
			ABufferReader->ReadLine(buffer, range, line);

			if (line->start == line->end)
			{
				break;
			}

			// make the line to string
			buffer[line->end] = '\0';
			const char* textureQuadName = buffer + line->start;

			ALogD("texture atlas quad = %s", textureQuadName);

			Read("rotate");
			bool isRotate = ABufferReader->TryFindString(buffer, line, "true");
			ALogA(isRotate == false, "Not support rotation");

			char* str;

			Read("xy:");
			// make the line to string
			buffer[line->end] = '\0';

			str               = strtok(buffer + line->start, ",");
			ALogA(str != NULL, "Can't find x number in xy");
			int x             = atoi(str);

			str               = strtok(NULL, ",");
			ALogA(str != NULL, "Can't find y number in xy");
			int y             = atoi(str);

			Read("size:");
			// make the line to string
			buffer[line->end] = '\0';

			str               = strtok(buffer + line->start, ",");
			ALogA(str != NULL, "Can't find width number in size");
			int width         = atoi(str);

			str               = strtok(NULL, ",");
			ALogA(str != NULL, "Can't find height number in size");
			int height        = atoi(str);

			ALogD("x = %d, y = %d, width = %d, height = %d", x, y, width, height);

			Read("orig");
			Read("offset");
			Read("index");

			TextureAtlasQuad atlasQuad[1];
			AQuad->Init(AGLToolToGLWidth(width), AGLToolToGLHeight(height), atlasQuad->quad);

            atlasQuad->textureIndex         = outTextureAtlas->textureList->size - 1;
            atlasQuad->quad->offsetTextureX = AGLToolToGLWidth (x);
            atlasQuad->quad->offsetTextureY = AGLToolToGLHeight(y);
            atlasQuad->atlas                = outTextureAtlas;

			AArrayStrMap->Put(outTextureAtlas->quadMap, textureQuadName, atlasQuad);
		}
	}

	AArrayList->Shrink(outTextureAtlas->quadMap->arrayList);
	AArrayList->Shrink(outTextureAtlas->textureList);
}

#undef Read

static void Release(TextureAtlas* textureAtlas)
{
	AArrayStrMap->Release(textureAtlas->quadMap);
	AArrayList  ->Release(textureAtlas->textureList);

    bool isRemoved = AArrayStrMap->TryRemove(textureAtlasMap, textureAtlas->filePath);
    ALogA(isRemoved, "TextureAtlas release not found %s", textureAtlas->filePath);

    free(textureAtlas);
}

static TextureAtlas* Get(const char* filePath)
{
    TextureAtlas* textureAtlas = AArrayStrMapGet(textureAtlasMap, filePath, TextureAtlas*);

    if (textureAtlas == NULL)
    {
        textureAtlas           = (TextureAtlas*) malloc(sizeof(TextureAtlas));
        textureAtlas->filePath = AArrayStrMapGetKey
                                 (
                                    AArrayStrMapPut(textureAtlasMap, filePath, textureAtlas),
                                    textureAtlasMap->typeSize
                                 );

        Init(filePath, textureAtlas);
    }

    return textureAtlas;
}

struct ATextureAtlas ATextureAtlas[1] =
{
	Get,
	Release,
};
