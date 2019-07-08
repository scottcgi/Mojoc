/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2016-7-22
 * Update : 2019-1-31
 * Author : scott.cgi
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


static ArrayStrMap(filePath, TextureAtlas*) textureAtlasMap[1] = AArrayStrMap_Init(TextureAtlas*, 20);


#define ReadFind(str)                                          \
    ABufferReader->ReadLine(buffer, range, line);              \
    isFound = ABufferReader->TryFindString(buffer, line, str); \
    ALog_A(isFound, "ATextureAtlas %s cannot find string = %s", filePath, str)


static void Init(const char* filePath, TextureAtlas* outTextureAtlas)
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

        path[pathLen]  = '\0';
        
        if (fileDirLen != 0)
        {
            memcpy(path, filePath, (size_t) fileDirLen);
            memcpy(path + fileDirLen, buffer + line->start, (size_t) (pathLen - fileDirLen));
        }

        Texture* texture = ATexture->Get(path);
        AArrayList_Add(outTextureAtlas->textureList, texture);

        bool isFound;

        ReadFind("size");
        ReadFind("format");
        ReadFind("filter");
        ReadFind("repeat");

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

            ReadFind("rotate");
            isFound = ABufferReader->TryFindString(buffer, line, "true");
            ALog_A(isFound == false, "ATextureAtlas %s not support rotation", filePath);

            char* str;

            ReadFind("xy:");
            // make the line to string
            buffer[line->end] = '\0';

            str               = strtok(buffer + line->start, ",");
            ALog_A(str != NULL, "ATextureAtlas cannot find x number in xy");
            int x             = atoi(str);  // NOLINT(cert-err34-c)

            str               = strtok(NULL, ",");
            ALog_A(str != NULL, "ATextureAtlas cannot find y number in xy");
            int y             = atoi(str);  // NOLINT(cert-err34-c)

            ReadFind("size:");
            // make the line to string
            buffer[line->end] = '\0';

            str               = strtok(buffer + line->start, ",");
            ALog_A(str != NULL, "ATextureAtlas cannot find width number in size");
            int width         = atoi(str);  // NOLINT(cert-err34-c)

            str               = strtok(NULL, ",");
            ALog_A(str != NULL, "ATextureAtlas cannot find height number in size");
            int height        = atoi(str); // NOLINT(cert-err34-c)

            ReadFind("orig");
            ReadFind("offset");
            ReadFind("index");

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


#undef ReadFind


static void Release(TextureAtlas* textureAtlas)
{
    AArrayStrMap->Release(textureAtlas->quadMap);
    AArrayList  ->Release(textureAtlas->textureList);

    bool isRemoved = AArrayStrMap->TryRemove(textureAtlasMap, textureAtlas->filePath);
    ALog_A(isRemoved, "ATextureAtlas release not found %s", textureAtlas->filePath);

    free(textureAtlas);
}


static TextureAtlas* Get(const char* filePath)
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
