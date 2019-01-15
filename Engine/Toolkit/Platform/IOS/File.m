/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-8-29
 * Update: 2019-1-8
 * Author: scott.cgi
 */

 
#include "Engine/Toolkit/Platform/Platform.h"


#ifdef IS_PLATFORM_IOS


#include <stdio.h>
#include <Foundation/Foundation.h>

#include "Engine/Toolkit/Platform/File.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Platform/Log.h"


static File* Open(const char* relativeFilePath)
{
    NSString* path = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:relativeFilePath] ofType:nil];
    FILE*     file  = fopen([path cStringUsingEncoding:NSMacOSRomanStringEncoding], "rb");
    
    ALog_A(file != NULL, "AFile open error, file path = %s", relativeFilePath);
    
    return (File*) file;
}


static int OpenFileDescriptor(const char* relativeFilePath, long* outStart, long* outLength)
{
    FILE* file  = (FILE*) Open(relativeFilePath);
    int   fd   = fileno(file);
    
    fseek(file, 0, SEEK_END);
    *outLength = ftell(file);
    
    fseek(file, 0, SEEK_SET);
    *outStart  = ftell(file);
    
    return fd;
}


static void Close(File* file)
{
    fclose((FILE*) file);
}


static long GetLength(File* file)
{
    FILE* f = (FILE*) file;
    
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    return length;
}


static int Read(File* file, void* buffer, size_t count)
{
    FILE*  f    = (FILE*) file;
    size_t read = fread(buffer, count, 1, f);

    if (ferror(f) != 0)
    {
        clearerr(f);
        return -1;
    }

    return (int) read;
}


static long Seek(File* file, long offset, int whence)
{
    FILE* f = (FILE*) file;

    if (fseek(f, offset, whence) == 0)
    {
        fseek(f, 0, SEEK_SET);
        return ftell(f); // success return new position
    }

    return -1; // error
}


static const char* GetAbsoluteDirPath(int* outPathLength)
{
    static const char* absoluteDirPath = NULL;
    static int         length          = -1;
    
    if (absoluteDirPath == NULL)
    {
        NSString* str   = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
        absoluteDirPath = malloc(str.length);
        memcpy(absoluteDirPath, str.UTF8String, str.length);
        length          = str.length;
    }

    if (outPathLength != NULL)
    {
        *outPathLength = length;
    }
    
    return absoluteDirPath;
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
