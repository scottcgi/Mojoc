/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The Mojoc game engine is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2013-8-29
 * Update: 2019-1-8
 * Author: scott.cgi
 */

 
#include "Engine/Toolkit/Platform/Platform.h"


#ifdef IS_PLATFORM_IOS


#include <stdio.h>
#include <string.h>
#include <Foundation/Foundation.h>

#include "Engine/Toolkit/Platform/File.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Platform/Log.h"


static File* Open(const char* resourceFilePath)
{
    NSString* path = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:resourceFilePath] ofType:nil];
    FILE*     file = fopen([path cStringUsingEncoding:NSMacOSRomanStringEncoding], "rb");

    return (File*) file;
}


static int OpenFileDescriptor(const char* relativeFilePath, long* outStart, long* outLength)
{
    FILE* file = (FILE*) Open(relativeFilePath);
    int   fd   = fileno(file);

    *outStart  = ftell(file);
    fseek(file, 0, SEEK_END);
    *outLength = ftell(file);
    fseek(file, *outStart , SEEK_SET);

    return fd;
}


static void Close(File* file)
{
    fclose((FILE*) file);
}


static long GetLength(File* file)
{
    FILE* f = (FILE*) file;

    long start  = ftell(f);
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, start, SEEK_SET);
    
    return length;
}


static int Read(File* file, void* buffer, size_t count)
{
    FILE*  f    = (FILE*) file;
    size_t read = fread(buffer, count, 1, f);

    if (ferror(f) != 0)
    {
        perror("AFile Read error");
        clearerr(f);

        return -1; // error
    }

    return (int) read;
}


static long Seek(File* file, long offset, int whence)
{
    FILE* f = (FILE*) file;

    if (fseek(f, offset, whence) == 0)
    {
        return ftell(f); // success return new position
    }

    perror("AFile Seek error");
    clearerr(f);

    return -1; // error
}


static const char* GetInternalDataPath(int* outPathLength)
{
    static char* internalDataPath = NULL;
    static int   length           = -1;
    
    if (internalDataPath == NULL)
    {
        NSString*   str          = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
        const char* cStr         = [str cStringUsingEncoding:NSASCIIStringEncoding];
        length                   = (int) strlen(cStr) + 1;
        internalDataPath         = malloc(length);
        memcpy(internalDataPath, cStr, length);
        --length;
    }

    if (outPathLength != NULL)
    {
        *outPathLength = length;
    }

    return internalDataPath;
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
