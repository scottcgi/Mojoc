/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2017-03-20
 * Update: 2019-1-8
 * Author: scott.cgi
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


static char* CreateDataFromAbsolute(const char* absoluteFilePath, long* outLength)
{
    FILE* file = fopen(absoluteFilePath, "rb");

    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        long  length = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* buffer = malloc((size_t) length);
        *outLength   = length;

        fread (buffer, (size_t) length, 1, file);
        fclose(file);

        return buffer;
    }
    else
    {
        *outLength = 0;
        return NULL;
    }
}


static char* CreateStringFromAbsolute(const char* absoluteFilePath)
{
    FILE* file = fopen(absoluteFilePath, "rb");

    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        long  length   = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* buffer   = (char*) malloc((size_t) length + 1);
        buffer[length] = '\0';

        fread (buffer, (size_t) length, 1, file);
        fclose(file);

        return buffer;
    }
    else
    {
        return NULL;
    }
}


static char* CreateDataFromRelative(const char* relativeFilePath, long* outLength)
{
    File* file    = AFile->Open(relativeFilePath);
    long  length = AFile->GetLength(file);
    char* buffer = (char*) malloc((size_t) length);
    *outLength   = length;

    AFile->Read (file, buffer, (size_t) length);
    AFile->Close(file);

    return buffer;
}


static char* CreateStringFromRelative(const char* relativeFilePath)
{
    File* file      = AFile->Open(relativeFilePath);
    long  length   = AFile->GetLength(file);
    char* buffer   = (char*) malloc((size_t) length + 1);
    buffer[length] = '\0';

    AFile->Read (file, buffer, (size_t) length);
    AFile->Close(file);

    return buffer;
}


static char* CreateDataFromDir(const char* relativeDirFilePath, long* outLength)
{
    int         dirPathLength;
    const char* dirPath = AFile->GetAbsoluteDirPath(&dirPathLength);

    char path[dirPathLength + strlen(relativeDirFilePath) + 2];
    sprintf(path, "%s/%s", dirPath, relativeDirFilePath);

    return CreateDataFromAbsolute(path, outLength);
}


static void WriteDataToDir(const char* relativeDirFilePath, void* data, size_t length)
{
    ALog_A(data != NULL, "AFileTool WriteDataToDir failed, data == NULL.");

    int         dirPathLength;
    const char* dirPath = AFile->GetAbsoluteDirPath(&dirPathLength);

    char path[dirPathLength + strlen(relativeDirFilePath) + 2];
    sprintf(path, "%s/%s", dirPath, relativeDirFilePath);

    FILE* file = fopen(path, "wb");
    fwrite(data, length, 1, file);
    fclose(file);
}


struct AFileTool AFileTool[1] =
{
    GetDirLength,
    CreateDataFromAbsolute,
    CreateStringFromAbsolute,

    CreateDataFromRelative,
    CreateStringFromRelative,

    CreateDataFromDir,
    WriteDataToDir,
};
