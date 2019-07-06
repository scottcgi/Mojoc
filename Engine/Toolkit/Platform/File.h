/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License.
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub.
 * This Mojoc game engine is a serious project with extreme code pursuit, and will continue to iterate.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2013-08-27
 * Update : 2019-1-8
 * Author : scott.cgi
 */

 
#ifndef FILE_H
#define FILE_H


#include <stddef.h>
#include "Engine/Toolkit/HeaderUtils/Define.h"


/**
 * Related to platform directory.
 * 
 * Android: assets   files
 * IOS    : NSBundle files
 */
typedef struct File File;

/**
 * Resource File access API.
 */
struct AFile
{
    /**
     * Open a file from platform resource directory by file path.
     *
     * resourceFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     *
     *  return NULL if open file error.
     */
    File*       (*Open)               (const char* resourceFilePath);

    /**
     * Open a file descriptor from platform resource directory by file path.
     * it can be used to read the file data.
     *
     * resourceFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     *
     * returns < 0 if direct fd access is not possible (for example, if the file is compressed).
     */
    int         (*OpenFileDescriptor) (const char* resourceFilePath, long* outStart, long* outLength);

    /**
     * Close an opened file connection, free any related resources.
     */
    void        (*Close)              (File* file);

    /**
     * Return file length.
     */
    long        (*GetLength)          (File* file);

    /**
     * Read count bytes of data from the current offset.
     * return the number of bytes read, zero on EOF, or < 0 on error, always need to close file.
     */
    int         (*Read)               (File* file, void* buffer, size_t count);

    /**
     * Seek to the specified offset.
     * the whence uses the same constants (SEEK_SET, SEEK_CUR, SEEK_END) as fseek().
     *
     * return the new position on success, or -1 on error, always need to close file.
     */
     long       (*Seek)               (File* file, long offset, int whence);

    /**
     * The internal data path related to platform directory.
     * if outPathLength not NULL, will set the strlen(internalDataPath).
     *
     * internalDataPath:
     *     Android: internal data directory
     *     IOS    : document data directory
     */
    const char* (*GetInternalDataPath)(int* outPathLength);
};


extern struct AFile AFile[1];


#endif
