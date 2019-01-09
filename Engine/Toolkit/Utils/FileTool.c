/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License:
 * https://github.com/scottcgi/Mojoc/blob/master/LICENSE
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


static char* CreateDataFrom(const char* absoluteFilePath, long* outLength)
{
    FILE* file   = fopen(absoluteFilePath, "rb");

    fseek(file, 0, SEEK_END);
    long  length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*) malloc(length);
    *outLength   = length;

    fread (buffer, length, 1, file);
    fclose(file);

    return buffer;
}


static char* CreateStringFrom(const char* absoluteFilePath)
{
    FILE* file = fopen(absoluteFilePath, "rb");

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer   = (char*) malloc(length + 1);
    buffer[length] = '\0';

    fread (buffer, length, 1, file);
    fclose(file);
    
    return buffer;
}


static char* CreateDataFromRes(const char* relativeFilePath, long* outLength)
{
    File* file    = AFile->Open(relativeFilePath);
    long  length = AFile->GetLength(file);
    char* buffer = (char*) malloc(length);
    *outLength   = length;

    AFile->Read (file, buffer, length);
    AFile->Close(file);

    return buffer;
}


static char* CreateStringFromRes(const char* relativeFilePath)
{
    File* file      = AFile->Open(relativeFilePath);
    long  length   = AFile->GetLength(file);
    char* buffer   = (char*) malloc(length + 1);
    buffer[length] = '\0';

    AFile->Read (file, buffer, length);
    AFile->Close(file);

    return buffer;
}


static const char* dir = NULL;
static int         len = 0;


static char* CreateDataFromDir(const char* relativeDirFilePath, int* outLength)
{
    if (dir == NULL)
    {
        dir = AFile->GetAbsoluteDirPath();
        len = (int) strlen(dir);
    }

    char path[len + strlen(relativeDirFilePath) + 2];
    sprintf(path, "%s/%s", dir, relativeDirFilePath);

    FILE* f = fopen(path, "rb");

    if (f != NULL)
    {
        fseek(f, 0, SEEK_END);

        int   length = (int) ftell(f);
        char* data   = malloc(length);
        *outLength   = length;

        fseek (f,    0,  SEEK_SET);
        fread (data, length, 1, f);
        fclose(f);

        return data;
    }
    else
    {
        *outLength = 0;
        return NULL;
    }
}


static void WriteDataToDir(const char* relativeDirFilePath, void* data, int length)
{
    ALog_A(data != NULL && length > -1, "AFileTool WriteDataToDir failed, data == NULL or length < 0.");

    if (dir == NULL)
    {
        dir = AFile->GetAbsoluteDirPath();
        len = (int) strlen(dir);
    }

    char path[len + strlen(relativeDirFilePath) + 2];
    sprintf(path, "%s/%s", dir, relativeDirFilePath);

    FILE* f = fopen(path, "wb");
    fwrite(data, length, 1, f);
    fclose(f);
}


struct AFileTool AFileTool[1] =
{
    GetDirLength,
    CreateDataFrom,
    CreateStringFrom,

    CreateDataFromRes,
    CreateStringFromRes,

    CreateDataFromDir,
    WriteDataToDir,
};
