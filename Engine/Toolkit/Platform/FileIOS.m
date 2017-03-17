/*
 *
 *
 *  Created on: 2013-08-29
 *      Author: scott.cgi
 */

#include "Engine/Toolkit/Platform/Platform.h"

//--------------------------------------------------------------------------------------------------
#ifdef is_platform_ios
//--------------------------------------------------------------------------------------------------

#include <stddef.h>
#include <stdio.h>
#include <Foundation/Foundation.h>

#include "Engine/Toolkit/Platform/File.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Platform/Log.h"

static File* Open(const char* filePath)
{
    NSString* fpath    = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:filePath] ofType:@""];;
    FILE*     file     = fopen([fpath cStringUsingEncoding:NSMacOSRomanStringEncoding], "rb");
    
    ALogA(file != NULL, "AFile open error, file path = %s", filePath);
    
	return (File*) file;
}


static int OpenFileDescriptor(const char* filePath, long* outStart, long* outLength)
{
    ALogA(NULL, "AFile OpenFileDescriptor not supported !")
    
    return 0;
}


static void Close(File* filePtr)
{
    fclose((FILE*) filePtr);
}


static long GetLength(File* filePtr)
{
    fseek((FILE*) filePtr, 0, SEEK_END);
    long length = ftell((FILE*) filePtr);
    fseek((FILE*) filePtr, 0, SEEK_SET);
    
	return length;
}


static int Read(File* filePtr, void* buffer, size_t count)
{
	return (int) fread(buffer, count, 1, (FILE*) filePtr);
}

static int Seek(File* filePtr, long offset, int whence)
{
	return fseek((FILE*) filePtr, offset, whence);
}


static char* ReadBuffer(const char* filePath, long* outLength)
{
    void* filePtr = Open(filePath);
    long  length  = GetLength(filePtr);
    *outLength    = length;
    
    ALogD("file length = %ld", length);
    
    char* buffer = (char*) malloc(length);
    
    Read (filePtr, buffer, length);
    Close(filePtr);
    
    return buffer;
}

static char* ReadString(const char* filePath)
{
    void* filePtr  = Open(filePath);
    long  length   = GetLength(filePtr);
    
    ALogD("file length = %ld", length);
    
    char* buffer   = (char*) malloc(length + 1);
    buffer[length] = '\0';
    
    Read (filePtr, buffer, length);
    Close(filePtr);
    
    return buffer;
}

static int GetDirLength(const char* filePath)
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

struct AFile AFile[1] =
{
	Open,
    OpenFileDescriptor,
	Close,
	GetLength,
	Read,
	Seek,
	ReadBuffer,
	ReadString,
	GetDirLength,
};

//--------------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------
