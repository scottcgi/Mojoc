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


static File* Open(char* relativeFilePath)
{
    NSString* path = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:relativeFilePath] ofType:nil];
    FILE*     file = fopen([path cStringUsingEncoding:NSMacOSRomanStringEncoding], "rb");
    
    ALogA(file != NULL, "AFile open error, file path = %s", relativeFilePath);
    
	return (File*) file;
}


static int OpenFileDescriptor(char* relativeFilePath, long* outStart, long* outLength)
{
    ALogA(false, "AFile OpenFileDescriptor not supported !");
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


static const char* GetAbsoluteDirPath()
{
    static char* absoluteDirPath = NULL;
    
    if (absoluteDirPath == NULL)
    {
        NSString*   str = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
        absoluteDirPath = malloc(str.length);
        
        memcpy(absoluteDirPath, str.UTF8String, str.length);
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


//--------------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------
