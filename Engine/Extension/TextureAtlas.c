/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2016-7-22
 * Author: scott.cgi
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Engine/Toolkit/HeaderUtils/Define.h"
#include "Engine/Extension/TextureAtlas.h"
#include "Engine/Toolkit/Utils/BufferReader.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Toolkit/Utils/FileTool.h"


static ArrayStrMap(filePath, TextureAtlas*) textureAtlasMap[1] = AArrayStrMap_Init(TextureAtlas*, 10);


static inline void ReadFind(char* buffer, ArrayRange* range, ArrayRange* line, char* str)
{
    ABufferReader->ReadLine(buffer, range, line);
    bool isFound = ABufferReader->TryFindString(buffer, line, str);
    ALog_A(isFound, "ATextureAtlas cannot find string = %s", str);
}


#define Read(str) ReadFind(buffer, range, line, str)


static void Init(char* filePath, TextureAtlas* outTextureAtlas)
{
    AArrayStrMap->InitWithCapacity(sizeof(TextureAtlasQuad), 20, outTextureAtlas->quadMap);
    AArrayList  ->InitWithCapacity(sizeof(Texture*),         5,  outTextureAtlas->textureList);

    long  size;
    char* buffer = AFileTool->CreateDataFromResource(filePath, &size);

    ArrayRange range[1] = {0, (int) size - 1};
    ArrayRange line [1];

    ABufferReader->ReadLine(buffer, range, line);

    while (range->start < range->end)
    {
        // read image name
        ABufferReader->ReadLine(buffer, range, line);

        int  fileDirLen = AFileTool->GetDirLength(filePath);
        int  pathLen   = fileDirLen + (line->end - line->start);
        char path[pathLen + 1];

        path[pathLen] = '\0';

        if (fileDirLen != 0)
        {
            memcpy(path, filePath, fileDirLen);
            memcpy(path + fileDirLen, buffer + line->start, pathLen - fileDirLen);
        }

        Texture* texture = ATexture->Get(path);
        AArrayList_Add(outTextureAtlas->textureList, texture);

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
            buffer[line->end]     = '\0';
            char* textureQuadName = buffer + line->start;

            Read("rotate");
            bool isRotate = ABufferReader->TryFindString(buffer, line, "true");
            ALog_A(isRotate == false, "ATextureAtlas not support rotation");

            char* str;

            Read("xy:");
            // make the line to string
            buffer[line->end] = '\0';

            str               = strtok(buffer + line->start, ",");
            ALog_A(str != NULL, "ATextureAtlas cannot find x number in xy");
            int x             = atoi(str);

            str               = strtok(NULL, ",");
            ALog_A(str != NULL, "ATextureAtlas cannot find y number in xy");
            int y             = atoi(str);

            Read("size:");
            // make the line to string
            buffer[line->end] = '\0';

            str               = strtok(buffer + line->start, ",");
            ALog_A(str != NULL, "ATextureAtlas cannot find width number in size");
            int width         = atoi(str);

            str               = strtok(NULL, ",");
            ALog_A(str != NULL, "ATextureAtlas cannot find height number in size");
            int height        = atoi(str);

            Read("orig");
            Read("offset");
            Read("index");

            TextureAtlasQuad atlasQuad[1];
            AQuad->Init(AGLTool_ToGLWidth(width), AGLTool_ToGLHeight(height), atlasQuad->quad);

            atlasQuad->textureIndex         = outTextureAtlas->textureList->size - 1;
            atlasQuad->quad->offsetTextureX = AGLTool_ToGLWidth (x);
            atlasQuad->quad->offsetTextureY = AGLTool_ToGLHeight(y);
            atlasQuad->atlas                = outTextureAtlas;

            AArrayStrMap->TryPut(outTextureAtlas->quadMap, textureQuadName, atlasQuad);
        }
    }

    AArrayList->Shrink(outTextureAtlas->quadMap->elementList);
    AArrayList->Shrink(outTextureAtlas->textureList);
    free(buffer);
}


#undef Read


static void Release(TextureAtlas* textureAtlas)
{
    AArrayStrMap->Release(textureAtlas->quadMap);
    AArrayList  ->Release(textureAtlas->textureList);

    bool isRemoved = AArrayStrMap->TryRemove(textureAtlasMap, textureAtlas->filePath);
    ALog_A(isRemoved, "ATextureAtlas release not found %s", textureAtlas->filePath);

    free(textureAtlas);
}


static TextureAtlas* Get(char* filePath)
{
    TextureAtlas* textureAtlas = AArrayStrMap_Get(textureAtlasMap, filePath, TextureAtlas*);

    if (textureAtlas == NULL)
    {
        textureAtlas           = malloc(sizeof(TextureAtlas));
        textureAtlas->filePath = AArrayStrMap_GetKey
                                 (
                                    AArrayStrMap_TryPut(textureAtlasMap, filePath, textureAtlas),
                                    textureAtlasMap->valueTypeSize
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
