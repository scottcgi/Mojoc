/*
 *
 *
 *  Created on: 2017-03-20
 *      Author: scott.cgi
 */

#include <string.h>

#include "Engine/Toolkit/Utils/FileTool.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Platform/File.h"


static FILE* Open(char* filePath)
{
    FILE* file = fopen(filePath, "rb");
    ALogA(file != NULL, "AFileTool open error, file path = %s", filePath);
    
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


static char* ReadBuffer(char* filePath, long* outLength)
{
    void* file   = Open(filePath);
    long  length = GetLength(file);
    char* buffer = (char*) malloc(length);
    *outLength   = length;

    Read (file, buffer, length);
    Close(file);

    return buffer;
}


static char* ReadString(char* filePath)
{
    void* file     = Open(filePath);
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


static char* ReadBufferPlatform(char* filePath, long* outLength)
{
    File* file   = AFile->Open(filePath);
    long  length = AFile->GetLength(file);
    char* buffer = (char*) malloc(length);
    *outLength   = length;

    AFile->Read (file, buffer, length);
    AFile->Close(file);

    return buffer;
}


static char* ReadStringPlatform(char* filePath)
{
    File* file     = AFile->Open(filePath);
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
	ReadBuffer,
	ReadString,
	GetDirLength,

    ReadBufferPlatform,
    ReadStringPlatform,
};
