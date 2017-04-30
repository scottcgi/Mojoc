/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2013-08-29
 * Author : scott.cgi
 */

#include "Engine/Toolkit/Platform/Platform.h"


//--------------------------------------------------------------------------------------------------
#ifdef is_platform_ios
//--------------------------------------------------------------------------------------------------


#include <stdio.h>
#include <Foundation/Foundation.h>

#include "Engine/Toolkit/Platform/File.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Platform/Log.h"


static File* Open(char* filePath)
{
    NSString* fpath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:filePath] ofType:@""];
    FILE*     file  = fopen([fpath cStringUsingEncoding:NSMacOSRomanStringEncoding], "rb");
    
    ALogA(file != NULL, "AFile open error, file path = %s", filePath);
    
	return (File*) file;
}


static int OpenFileDescriptor(char* filePath, long* outStart, long* outLength)
{
    ALogA(NULL, "AFile OpenFileDescriptor not supported !");
    return 0;
}


static void Close(File* file)
{
    fclose((FILE*) file);
}


static long GetLength(File* file)
{
    fseek((FILE*) file, 0, SEEK_END);
    long length = ftell((FILE*) file);
    fseek((FILE*) file, 0, SEEK_SET);
    
	return length;
}


static int Read(File* file, void* buffer, size_t count)
{
	return (int) fread(buffer, count, 1, (FILE*) file);
}


static int Seek(File* file, long offset, int whence)
{
	return fseek((FILE*) file, offset, whence);
}


struct AFile AFile[1] =
{
	Open,
    OpenFileDescriptor,
	Close,
	GetLength,
	Read,
	Seek,
};


//--------------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------
