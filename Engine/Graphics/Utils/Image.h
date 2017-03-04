/*
 *
 *
 *  Created on: 2013-08-30
 *      Author: scott.cgi
 */

#ifndef image_h
#define image_h

struct AImage
{
	/**
	 * Read png pixel data from file, caller must be free it
	 */
     void* (*CreatePixelDataFromPng)(const char* filePath, float* outWidth, float* outHeight);
};

extern struct AImage AImage[1];

#endif
