/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2013-8-29
 * Update : 2019-1-8
 * Author : scott.cgi
 */
 

#include "Engine/Toolkit/Platform/Platform.h"


#ifdef IS_PLATFORM_ANDROID


#include <string.h>
#include <android/native_activity.h>
#include "Engine/Toolkit/Platform/File.h"
#include "Engine/Toolkit/Platform/Log.h"


extern ANativeActivity* nativeActivity;


static File* Open(const char* resourceFilePath)
{
    AAsset* asset = AAssetManager_open(nativeActivity->assetManager, resourceFilePath, AASSET_MODE_UNKNOWN);
    return (File*) asset;
}


static int OpenFileDescriptor(const char* resourceFilePath, long* outStart, long* outLength)
{
    AAsset* asset = AAssetManager_open(nativeActivity->assetManager, resourceFilePath, AASSET_MODE_UNKNOWN);
    int     fd    = AAsset_openFileDescriptor(asset, (off_t) outStart, (off_t) outLength);
    AAsset_close(asset);

    return fd;
}


static void Close(File* file)
{
    AAsset_close((AAsset*) file);
}


static long GetLength(File* file)
{
    return (long) AAsset_getLength((AAsset*) file);
}


static int Read(File* file, void* buffer, size_t count)
{
    return AAsset_read((AAsset*) file, buffer, count);
}


static long Seek(File* file, long offset, int whence)
{
    return (long) AAsset_seek((AAsset*) file, (off_t) offset, whence);
}


static const char* GetInternalDataPath(int* outPathLength)
{
    static int length = -1;

    if (length == -1)
    {
        length = (int) strlen(nativeActivity->internalDataPath);
    }

    if (outPathLength != NULL)
    {
        *outPathLength = length;
    }

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
    GetInternalDataPath,
};


#endif
