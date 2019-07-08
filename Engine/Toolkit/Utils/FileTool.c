/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2017-03-20
 * Update   : 2019-1-8
 * Author   : scott.cgi
 */


#include <string.h>
#include "Engine/Toolkit/Utils/FileTool.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Platform/File.h"


static int GetDirLength(const char* filePath)
{
    char* lastForwardSlash  = strrchr(filePath, '/');
    char* lastBackwardSlash = strrchr(filePath, '\\');
    char* lastSlash         = AMath_Max(lastForwardSlash, lastBackwardSlash);

    if (lastSlash != NULL)
    {
        // include last slash
        return (int) (lastSlash - filePath + 1);
    }
    else
    {
        return 0;
    }
}


static void* CreateDataFromAbsolute(const char* absoluteFilePath, long* outSize)
{
    FILE* file = fopen(absoluteFilePath, "rbe"); // mode 'e' to set O_CLOEXEC

    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        long  length = ftell(file);
        void* buffer = malloc((size_t) length);
        *outSize     = length;

        fseek (file, 0, SEEK_SET);
        fread (buffer, (size_t) length, 1, file);
        fclose(file);

        return buffer;
    }
    else
    {
        *outSize = 0;
        return NULL;
    }
}


static char* CreateStringFromAbsolute(const char* absoluteFilePath)
{
    FILE* file = fopen(absoluteFilePath, "rbe"); // mode 'e' to set O_CLOEXEC

    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        long  length   = ftell(file);
        char* buffer   = malloc((size_t) length + 1);
        buffer[length] = '\0';

        fseek (file, 0, SEEK_SET);
        fread (buffer, (size_t) length, 1, file);
        fclose(file);

        return buffer;
    }
    else
    {
        return NULL;
    }
}


static void* CreateDataFromResource(const char* resourceFilePath, long* outSize)
{
    File* file   = AFile->Open(resourceFilePath);
    long  length = AFile->GetLength(file);
    void* buffer = malloc((size_t) length);
    *outSize     = length;

    AFile->Read (file, buffer, (size_t) length);
    AFile->Close(file);

    return buffer;
}


static char* CreateStringFromResource(const char* resourceFilePath)
{
    File* file     = AFile->Open(resourceFilePath);
    long  length   = AFile->GetLength(file);
    char* buffer   = malloc((size_t) length + 1);
    buffer[length] = '\0';

    AFile->Read (file, buffer, (size_t) length);
    AFile->Close(file);

    return buffer;
}


static void* CreateDataFromRelative(const char* relativeDirFilePath, long* outSize)
{
    int         dirPathLength;
    const char* dirPath = AFile->GetInternalDataPath(&dirPathLength);

    char path[dirPathLength + strlen(relativeDirFilePath) + 2];
    sprintf(path, "%s/%s", dirPath, relativeDirFilePath);

    return CreateDataFromAbsolute(path, outSize);
}


static void WriteDataToRelative(const char* relativeDirFilePath, void* data, size_t size)
{
    ALog_A(data != NULL, "AFileTool WriteDataToDir failed, data == NULL");

    int         dirPathLength;
    const char* dirPath = AFile->GetInternalDataPath(&dirPathLength);

    char path[dirPathLength + strlen(relativeDirFilePath) + 2];
    sprintf(path, "%s/%s", dirPath, relativeDirFilePath);

    FILE* file = fopen(path, "wbe"); // mode 'e' to set O_CLOEXEC
    fwrite(data, size, 1, file);
    fclose(file);
}


struct AFileTool AFileTool[1] =
{
    GetDirLength,
    CreateDataFromAbsolute,
    CreateStringFromAbsolute,

    CreateDataFromResource,
    CreateStringFromResource,

    CreateDataFromRelative,
    WriteDataToRelative,
};
