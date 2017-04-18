/*
 * Copyright (C) scott.cgi All Rights Reserved.
 *
 * Since  : 2017-03-20
 * Author : scott.cgi
 */

#ifndef file_tool_h
#define file_tool_h


#include <stddef.h>
#include <stdio.h>


struct AFileTool
{
	/**
	 * The filePath is absolute path
	 */
    FILE* (*Open)              (char* filePath);

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

	/**
	 * The filePath is absolute path
	 *
	 * read all file data into buffer with it length, with close file
	 * return buffer pointer
	 */
	char* (*ReadBuffer)        (char* filePath, long* outLength);

	/**
	 * The filePath is absolute path
	 *
	 * read all file data into buffer with '\0' end with close file
	 * return buffer pointer
	 */
	char* (*ReadString)        (char* filePath);

	/**
	 * Get file directory length in file path, include last slash '/' or '\\'
	 * if no directory return 0
	 */
	int   (*GetDirLength)      (char* filePath);

//--------------------------------------------------------------------------------------------------

    /**
     * The filePath is relative path, indirect use AFile
     *
     * read all file data into buffer with it length, with close file
     * return buffer pointer
     */
    char* (*ReadBufferPlatform)(char* filePath, long* outLength);

    /**
     * The filePath is relative path, indirect use AFile
     *
     * read all file data into buffer with '\0' end with close file
     * return buffer pointer
     */
    char* (*ReadStringPlatform)(char* filePath);
};


extern struct AFileTool AFileTool[1];


#endif
