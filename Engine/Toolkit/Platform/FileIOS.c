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
#include "Engine/Toolkit/Platform/File.h"

static File* Open(const char* filePath)
{
	return NULL;
}


static int OpenFileDescriptor(const char* filePath, off_t* outStart, off_t* outLength)
{
    return 0;
}


static void Close(File* filePtr)
{
}


static long GetLength(File* filePtr)
{
	return 0;
}


static int Read(File* filePtr, void* buffer, size_t count)
{
	return 0;
}

static int Seek(File* filePtr, long offset, int whence)
{
	return 0;
}


static char* ReadBuffer(const char* filePath, long* outLength)
{
	return NULL;
}

static char* ReadString(const char* filePath)
{
	return NULL;
}

static int GetDirLength(const char* filePath)
{
	return 0;
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
