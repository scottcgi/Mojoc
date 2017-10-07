/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-08-27
 * Author : scott.cgi
 * Version: 0.0.0
 */

#ifndef FILE_H
#define FILE_H


#include <stddef.h>
#include "Engine/Toolkit/Head/Define.h"


/**
 * Related to platform directory
 * Android: assets   files
 * IOS:     NSBundle files
 */
typedef struct File File;


struct AFile
{
    /**
     * Open file from platform directory with relative file path
     *
     * relativeFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    File* (*Open)             (char* relativeFilePath);

    /**
     * Open a new file descriptor that can be used to read the asset data
     * if the start or length cannot be represented by a 32-bit number, it will be truncated
     *
     * relativeFilePath:
     *     Android: assets
     *        IOS    : NSBundle
     *
     * returns < 0 if direct fd access is not possible (for example, if the asset is compressed)
     */
    int   (*OpenFileDescriptor)(char* relativeFilePath, long* outStart, long* outLength);

    /**
     * Close an opened file connection, free any related resources
     */
    void  (*Close)             (File* file);

    /**
     * Return file length
     */
    long  (*GetLength)         (File* file);

    /**
     * Read count bytes of data from the current offset
     * return the number of bytes read, zero on EOF, or < 0 on error
     */
    int   (*Read)              (File* file, void* buffer, size_t count);

    /**
     * Seek to the specified offset
     * the whence uses the same constants as fseek()
     *
     * return the new position on success, or -1 on error
     */
    int   (*Seek)              (File* file, long offset, int whence);

//----------------------------------------------------------------------------------------------------------------------

    /**
     * The absolute path related to platform directory, read only
     *
     * Android: internal data directory
     * IOS    : document data directory
     */
    const char* (*GetAbsoluteDirPath)();
};


extern struct AFile AFile[1];


#endif
