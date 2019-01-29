/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2017-03-20
 * Update: 2019-1-8
 * Author: scott.cgi
 */

 
#ifndef FILE_TOOL_H
#define  FILE_TOOL_H


#include <stddef.h>
#include <stdio.h>


/**
 * A tool for access file by file string path.
 */
struct AFileTool
{
    /**
     * Get file directory length in file path string, include last slash '/' or '\\'.
     * return 0 when no directory.
     */
    int   (*GetDirLength)            (const char* filePath);

    /**
     * Read all file data into malloc buffer, and close file.
     *
     * outSize: the create data size.
     *
     * if file not exist
     *     return NULL
     * else
     *     return buffer ptr, and need to free it after using.
     */
    void* (*CreateDataFromAbsolute)  (const char* absoluteFilePath, long* outSize);

    /**
     * Read all file data into malloc buffer, end with '\0', and close file.
     *
     * if file not exist
     *     return NULL
     * else
     *     return buffer ptr, and need to free it after using.
     */
    char* (*CreateStringFromAbsolute)(const char* absoluteFilePath);

    /**
     * Indirect use AFile, read all file data into malloc buffer, and close file.
     *
     * outSize: the create data size.
     *
     * resourceFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     *
     * return buffer ptr, and need to free it after using.
     */
    void* (*CreateDataFromResource)  (const char* resourceFilePath, long* outSize);

    /**
     * Indirect use AFile, read all file data into malloc buffer, end with '\0', and close file.
     *
     * resourceFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     *
     * return buffer ptr, and need to free it after using.
     */
    char* (*CreateStringFromResource)(const char* resourceFilePath);

    /**
     * Read all file data into malloc buffer, and close file.
     * the relativeFilePath is relative internalDataPath from AFile->GetInternalDataPath().
     *
     * internalDataPath:
     *     Android: internal data directory
     *     IOS    : document data directory
     *
     *  outSize: the create data size.
     *
     * if file not exist
     *     return NULL
     * else
     *     return buffer ptr, and need to free it after using.
     */
    void* (*CreateDataFromRelative)  (const char* relativeFilePath, long* outSize);

    /**
     * Write data into relativeDirFilePath, and close file.
     * the relativeFilePath is relative internalDataPath from AFile->GetInternalDataPath().
     *
     * internalDataPath:
     *     Android: internal data directory
     *     IOS    : document data directory
     *
     * data: will be write to relativeFilePath
     * size: the write data size
     *
     * if file not exist will created.
     */
     void (*WriteDataToRelative)     (const char* relativeFilePath, void* data, size_t size);
};


extern struct AFileTool AFileTool[1];


#endif
