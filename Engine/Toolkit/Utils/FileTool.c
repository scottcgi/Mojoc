/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2017-03-20
 * Author: scott.cgi
 */

#include <string.h>

#include "Engine/Toolkit/Utils/FileTool.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Platform/File.h"


static FILE* Open(char* absoluteFilePath)
{
    FILE* file = fopen(absoluteFilePath, "rb");
    ALogA(file != NULL, "AFileTool open error, absolute file path path = %s", absoluteFilePath);
    
	return file;
}


static void Close(FILE* file)
{
    fclose(file);
}


static long GetLength(FILE* file)
{
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
	return length;
}


static int Read(FILE* file, void* buffer, size_t count)
{
	return (int) fread(buffer, count, 1, file);
}


static int Seek(FILE* file, long offset, int whence)
{
	return fseek(file, offset, whence);
}


static char* CreateBuffer(char* absoluteFilePath, long* outLength)
{
    void* file   = Open(absoluteFilePath);
    long  length = GetLength(file);
    char* buffer = (char*) malloc(length);
    *outLength   = length;

    Read (file, buffer, length);
    Close(file);

    return buffer;
}


static char* CreateString(char* absoluteFilePath)
{
    void* file     = Open(absoluteFilePath);
    long  length   = GetLength(file);
    char* buffer   = (char*) malloc(length + 1);
    buffer[length] = '\0';
    
    Read (file, buffer, length);
    Close(file);
    
    return buffer;
}


static int GetDirLength(char* filePath)
{
    char* lastForwardSlash  = strrchr(filePath, '/');
    char* lastBackwardSlash = strrchr(filePath, '\\');
    char* lastSlash         = AMathMax(lastForwardSlash, lastBackwardSlash);
    
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


//--------------------------------------------------------------------------------------------------


static char* CreateBufferRelative(char* relativeFilePath, long* outLength)
{
    File* file   = AFile->Open(relativeFilePath);
    long  length = AFile->GetLength(file);
    char* buffer = (char*) malloc(length);
    *outLength   = length;

    AFile->Read (file, buffer, length);
    AFile->Close(file);

    return buffer;
}


static char* CreateStringRelative(char* relativeFilePath)
{
    File* file     = AFile->Open(relativeFilePath);
    long  length   = AFile->GetLength(file);
    char* buffer   = (char*) malloc(length + 1);
    buffer[length] = '\0';

    AFile->Read (file, buffer, length);
    AFile->Close(file);

    return buffer;
}


struct AFileTool AFileTool[1] =
{
	Open,
	Close,
	GetLength,
	Read,
	Seek,
    GetDirLength,
	CreateBuffer,
	CreateString,

    CreateBufferRelative,
    CreateStringRelative,
};
