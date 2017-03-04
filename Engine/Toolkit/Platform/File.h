/*
 *
 *
 *  Created on: 2013-08-27
 *      Author: scott.cgi
 */

#ifndef file_h
#define file_h

#include <stddef.h>

typedef struct File File;

struct AFile
{
	/**
	 * Return filePtr
	 */
    File* (*Open)       (const char* filePath);

    /**
     * Open a new file descriptor that can be used to read the asset data. If the
     * start or length cannot be represented by a 32-bit number, it will be
     * truncated. If the file is large, use AAsset_openFileDescriptor64 instead.
     *
     * Returns < 0 if direct fd access is not possible (for example, if the asset is
     * compressed).
     */
    int   (*OpenFileDescriptor)(const char* filePath, long* outStart, long* outLength);

	/**
	 * Close an opened file connection, free any related resources
	 */
	void  (*Close)      (File* filePtr);

	/**
	 * Return file length
	 */
	long  (*GetLength)  (File* filePtr);

	/**
	 * Read count bytes of data from the current offset
	 * return the number of bytes read, zero on EOF, or < 0 on error
	 */
	int   (*Read)       (File* filePtr, void* buffer, size_t count);

	/**
	 * Seek to the specified offset
	 * 'whence' uses the same constants as fseek()
	 *
	 * return the new position on success, or -1 on error
	 */
	int   (*Seek)        (File* filePtr, long offset, int whence);

	/**
	 * Read all file data into buffer with it length, with close filePtr
	 * return buffer pointer
	 */
	char* (*ReadBuffer)  (const char* filePath, long* outLength);

	/**
	 * Read all file data into buffer with '\0' end with close filePtr
	 * return buffer pointer
	 */
	char* (*ReadString)  (const char* filePath);

	/**
	 * Get file directory length in file path, include last slash '/' or '\\'
	 * if no directory return 0
	 */
	int   (*GetDirLength)(const char* filePath);
};

extern struct AFile AFile[1];

#endif
