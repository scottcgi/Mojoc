/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2017-03-20
 * Author : scott.cgi
 * Version: 0.0.0
 */


#ifndef FILE_TOOL_H
#define FILE_TOOL_H


#include <stddef.h>
#include <stdio.h>


struct AFileTool
{
    /**
     * Get file directory length in file path string, include last slash '/' or '\\'
     *
     * return 0 when no directory
     */
    int   (*GetDirLength)      (char* filePath);

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Read all file data into malloc buffer, with close file
     *
     * return buffer pointer, need to free it after using
     */
    char* (*CreateDataFrom)    (char* absoluteFilePath, long* outLength);

    /**
     * Read all file data into malloc buffer, with '\0' end, with close file
     *
     * return buffer pointer, need to free it after using
     */
    char* (*CreateStringFrom)  (char* absoluteFilePath);

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Indirect use AFile
     * read all file data into malloc buffer, with close file
     *
     * relativeFilePath:
     *     Android: assets
     *        IOS : NSBundle
     *
     * return buffer pointer, need to free it after using
     */
    char* (*CreateDataFromRes)  (char* relativeFilePath, long* outLength);

    /**
     * Indirect use AFile
     * read all file data into malloc buffer, with '\0' end, with close file
     *
     * relativeFilePath:
     *     Android: assets
     *        IOS : NSBundle
     *
     * return buffer pointer, need to free it after using
     */
    char* (*CreateStringFromRes)(char* relativeFilePath);

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Indirect use AFile's GetAbsoluteDirPath()
     * read all file data into malloc buffer, with close file
     *
     * relativeDirFilePath:
     *     Android: internal data directory
     *     IOS    : document data directory
     *
     * if file not exist
     *     return NULL
     * else
     *     return buffer pointer, need to free it after using
     */
    char* (*CreateDataFromDir)  (char* relativeDirFilePath, int* outLength);


    /**
     * Indirect use AFile's GetAbsoluteDirPath()
     * write data into relativeFilePath, with close file
     *
     * relativeDirFilePath:
     *     Android: internal data directory
     *     IOS    : document data directory
     *
     * if file not exist will created
     */
     void (*WriteDataToDir)     (char* relativeDirFilePath, void* data, int length);
};


extern struct AFileTool AFileTool[1];


#endif
