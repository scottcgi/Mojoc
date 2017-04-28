/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2017-03-20
 * Author: scott.cgi
 */

#ifndef file_tool_h
#define file_tool_h


#include <stddef.h>
#include <stdio.h>


struct AFileTool
{
	/**
	 * Open file with absolute path
	 */
    FILE* (*Open)              (char* absoluteFilePath);

	/**
	 * Close an opened file connection, free any related resources
	 */
	void  (*Close)             (FILE* file);

	/**
	 * Return file length
	 */
	long  (*GetLength)         (FILE* file);

	/**
	 * Read count bytes of data from the current offset
	 * return the number of bytes read, zero on EOF, or < 0 on error
	 */
	int   (*Read)              (FILE* file, void* buffer, size_t count);

	/**
	 * Seek to the specified offset, he whence use for fseek()
	 * return the new position on success, or -1 on error
	 */
	int   (*Seek)              (FILE* file, long offset, int whence);

//--------------------------------------------------------------------------------------------------

    /**
     * Get file directory length in file path string, include last slash '/' or '\\'
     *
     * if no directory return 0
     */
    int   (*GetDirLength)      (char* filePath);

	/**
	 * Read all file data into malloc buffer, with it length, with close file
	 *
	 * return buffer pointer, needed free it when finish
	 */
	char* (*CreateBuffer)      (char* absoluteFilePath, long* outLength);

	/**
	 * Read all file data into malloc buffer with '\0' end with close file
	 *
	 * return buffer pointer, needed free it when finish
	 */
	char* (*CreateString)      (char* absoluteFilePath);

//--------------------------------------------------------------------------------------------------

    /**
     * Indirect use AFile
     * read all file data into malloc buffer with it length, with close file
     *
     * return buffer pointer, needed free it when finish
     */
    char* (*CreateBufferRelative)(char* relativeFilePath, long* outLength);

    /**
     * Indirect use AFile
     * read all file data into malloc buffer with '\0' end with close file
     *
     * return buffer pointer, needed free it when finish
     */
    char* (*CreateStringRelative)(char* relativeFilePath);
};


extern struct AFileTool AFileTool[1];


#endif
