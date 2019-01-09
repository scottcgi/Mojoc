/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License:
 * https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * Since : 2017-03-20
 * Update: 2019-1-8
 * Author: scott.cgi
 */

 
#ifndef FILE_TOOL_H
#define  FILE_TOOL_H


#include <stddef.h>
#include <stdio.h>


struct AFileTool
{
    /**
     * Get file directory length in file path string, include last slash '/' or '\\'.
     * return 0 when no directory.
     */
    int   (*GetDirLength)       (const char* filePath);

    /**
     * Read all file data into malloc buffer, and close file.
     * return buffer ptr, and need to free it after using.
     */
    char* (*CreateDataFrom)     (const char* absoluteFilePath, long* outLength);

    /**
     * Read all file data into malloc buffer, end with '\0' , and close file.
     * return buffer ptr, and need to free it after using.
     */
    char* (*CreateStringFrom)   (const char* absoluteFilePath);

    /**
     * Indirect use AFile, read all file data into malloc buffer, and close file.
     *
     * relativeFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     *
     * return buffer ptr, and need to free it after using.
     */
    char* (*CreateDataFromRes)  (const char* relativeFilePath, long* outLength);

    /**
     * Indirect use AFile, read all file data into malloc buffer, end with '\0', and close file.
     *
     * relativeFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     *
     * return buffer ptr, and need to free it after using.
     */
    char* (*CreateStringFromRes)(const char* relativeFilePath);

    /**
     * Indirect use AFile's GetAbsoluteDirPath(),
     * read all file data into malloc buffer, and close file.
     *
     * relativeDirFilePath:
     *     Android: internal data directory
     *     IOS    : document data directory
     *
     * if file not exist
     *     return NULL
     * else
     *     return buffer ptr, and need to free it after using.
     */
    char* (*CreateDataFromDir)  (const char* relativeDirFilePath, int* outLength);


    /**
     * Indirect use AFile's GetAbsoluteDirPath(),
     * write data into relativeFilePath, and close file.
     *
     * relativeDirFilePath:
     *     Android: internal data directory
     *     IOS    : document data directory
     *
     * if file not exist will created.
     */
     void (*WriteDataToDir)     (const char* relativeDirFilePath, void* data, int length);
};


extern struct AFileTool AFileTool[1];


#endif
