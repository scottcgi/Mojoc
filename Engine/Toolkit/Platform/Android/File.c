/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License:
 * https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * Since : 2013-8-29
 * Update: 2019-1-8
 * Author: scott.cgi
 */
 

#include "Engine/Toolkit/Platform/Platform.h"


#ifdef IS_PLATFORM_ANDROID


#include <android/native_activity.h>
#include "Engine/Toolkit/Platform/File.h"
#include "Engine/Toolkit/Platform/Log.h"


extern ANativeActivity* nativeActivity;


static File* Open(char* relativeFilePath)
{
    AAsset* asset = AAssetManager_open(nativeActivity->assetManager, relativeFilePath, AASSET_MODE_UNKNOWN);
    ALog_A(asset != NULL, "AFile open failed, relative file path = %s", relativeFilePath);

    return (File*) asset;
}


static int OpenFileDescriptor(char* relativeFilePath, long* outStart, long* outLength)
{
    AAsset* asset = AAssetManager_open(nativeActivity->assetManager, relativeFilePath, AASSET_MODE_UNKNOWN);

    // open asset as file descriptor
    int fd = AAsset_openFileDescriptor(asset, outStart, outLength);
    ALog_A(fd >= 0, "AFile OpenFileDescriptor failed, relative file path = %s", relativeFilePath);
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


static const char* GetAbsoluteDirPath()
{
    return nativeActivity->internalDataPath;
}


struct AFile AFile[1] =
{
    Open,
    OpenFileDescriptor,
    Close,
    GetLength,
    Read,
    Seek,
    GetAbsoluteDirPath,
};


#endif
