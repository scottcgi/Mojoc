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
     * if file not exist
     *     return NULL
     * else
     *     return buffer ptr, and need to free it after using.
     */
    char* (*CreateDataFromAbsolute)  (const char* absoluteFilePath, long* outLength);

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
     * relativeFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     *
     * return buffer ptr, and need to free it after using.
     */
    char* (*CreateDataFromRelative)  (const char* relativeFilePath, long* outLength);

    /**
     * Indirect use AFile, read all file data into malloc buffer, end with '\0', and close file.
     *
     * relativeFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     *
     * return buffer ptr, and need to free it after using.
     */
    char* (*CreateStringFromRelative)(const char* relativeFilePath);

    /**
     * Read all file data into malloc buffer, and close file.
     * the relativeDirFilePath is relative AFile->GetAbsoluteDirPath().
     *
     * absoluteDirPath:
     *     Android: internal data directory
     *     IOS    : document data directory
     *
     * if file not exist
     *     return NULL
     * else
     *     return buffer ptr, and need to free it after using.
     */
    char* (*CreateDataFromDir)       (const char* relativeDirFilePath, long* outLength);

    /**
     * Write data into relativeDirFilePath, and close file.
     * the relativeDirFilePath is relative AFile->GetAbsoluteDirPath().
     *
     * absoluteDirPath:
     *     Android: internal data directory
     *     IOS    : document data directory
     *
     * if file not exist will created.
     */
     void (*WriteDataToDir)          (const char* relativeDirFilePath, void* data, size_t length);
};


extern struct AFileTool AFileTool[1];


#endif
