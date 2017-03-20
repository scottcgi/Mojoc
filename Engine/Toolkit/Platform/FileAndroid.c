/*
 *
 *
 *  Created on: 2013-08-29
 *      Author: scott.cgi
 */

#include "Engine/Toolkit/Platform/Platform.h"

//--------------------------------------------------------------------------------------------------
#ifdef is_platform_android
//--------------------------------------------------------------------------------------------------


#include <android/native_activity.h>
#include <string.h>

#include "Engine/Toolkit/Platform/File.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Platform/Log.h"

extern ANativeActivity* nativeActivity;

static File* Open(const char* filePath)
{
	AAsset* asset = AAssetManager_open(nativeActivity->assetManager, filePath, AASSET_MODE_UNKNOWN);
	ALogA(asset != NULL, "AFile open error, file path = %s", filePath);
	ALogD("Open file path = %s", filePath);

	return (File*) asset;
}


static int OpenFileDescriptor(const char* filePath, long* outStart, long* outLength)
{
    AAsset* asset = AAssetManager_open(nativeActivity->assetManager, filePath, AASSET_MODE_UNKNOWN);

    // open asset as file descriptor
    int fd = AAsset_openFileDescriptor(asset, outStart, outLength);
    ALogA(fd >= 0, "AFile OpenFileDescriptor error");
    AAsset_close(asset);

    return fd;
}


static void Close(File* file)
{
	AAsset_close((AAsset*) file);
}


static long GetLength(File* file)
{
	return AAsset_getLength((AAsset*) file);
}


static int Read(File* file, void* buffer, size_t count)
{
	return AAsset_read((AAsset*) file, buffer, count);
}


static int Seek(File* file, long offset, int whence)
{
	return AAsset_seek((AAsset*) file, offset, whence);
}


static char* ReadBuffer(const char* filePath, long* outLength)
{
	void* file    = Open(filePath);
	long  length  = GetLength(file);
	*outLength    = length;

	ALogD("file length = %ld", length);

	char* buffer = (char*) malloc(length);

	Read(file, buffer, length);
	Close(file);

	return buffer;
}


static char* ReadString(const char* filePath)
{
	void* file   = Open(filePath);
	long  length = GetLength(file);

	ALogD("file length = %ld", length);

	char* buffer   = (char*) malloc(length + 1);
	buffer[length] = '\0';

	Read(file, buffer, length);
	Close(file);

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
