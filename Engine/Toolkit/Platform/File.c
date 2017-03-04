/*
 *
 *
 *  Created on: 2013-08-29
 *      Author: scott.cgi
 */

#include <android/native_activity.h>
#include <malloc.h>
#include <string.h>

#include "Mojoc/Toolkit/Platform/File.h"
#include "Mojoc/Toolkit/Math/Math.h"
#include "Mojoc/Toolkit/Platform/Log.h"

extern ANativeActivity* nativeActivity;


static File* Open(const char* filePath)
{
	AAsset* asset = AAssetManager_open(nativeActivity->assetManager, filePath, AASSET_MODE_UNKNOWN);
	ALogA(asset != NULL, "AFile open error, file path = %s", filePath);
	ALogD("Open file path = %s", filePath);

	return (File*) asset;
}


static int OpenFileDescriptor(const char* filePath, off_t* outStart, off_t* outLength)
{
    AAsset* asset = AAssetManager_open(nativeActivity->assetManager, filePath, AASSET_MODE_UNKNOWN);

    // open asset as file descriptor
    int fd = AAsset_openFileDescriptor(asset, outStart, outLength);
    ALogA(fd >= 0, "AFile OpenFileDescriptor error");
    AAsset_close(asset);

    return fd;
}


static void Close(File* filePtr)
{
	AAsset_close((AAsset*) filePtr);
}


static long GetLength(File* filePtr)
{
	return AAsset_getLength((AAsset*) filePtr);
}


static int Read(File* filePtr, void* buffer, size_t count)
{
	return AAsset_read((AAsset*) filePtr, buffer, count);
}

static int Seek(File* filePtr, long offset, int whence)
{
	return AAsset_seek((AAsset*) filePtr, offset, whence);
}


static char* ReadBuffer(const char* filePath, long* outLength)
{
	void* filePtr = Open(filePath);
	long  length  = GetLength(filePtr);
	*outLength    = length;

	ALogD("file length = %ld", length);

	char* buffer = (char*) malloc(length);

	Read(filePtr, buffer, length);
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

	Read(filePtr, buffer, length);
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
		return lastSlash - filePath + 1;
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
