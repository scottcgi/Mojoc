/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-08-30
 * Author : scott.cgi
 * Version: 0.1
 */


#ifndef IMAGE_H
#define IMAGE_H


#include "Engine/Toolkit/Head/Define.h"


struct AImage
{
	/**
	 * Read png pixel data from file, caller must be free it
	 *
	 * return NULL when error happened
	 */
     void* (*CreatePixelDataFromPng)(char* filePath, float* outWidth, float* outHeight);
};


extern struct AImage AImage[1];


#endif
