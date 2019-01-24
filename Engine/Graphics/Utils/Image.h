/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-08-30
 * Update: 2019-1-19
 * Author: scott.cgi
 */


#ifndef IMAGE_H
#define  IMAGE_H


#include "Engine/Toolkit/HeaderUtils/Define.h"


struct AImage
{
    /**
     * Read png pixel data from resourceFilePath, after using the data must be freed.
     * return NULL when error happened.
     *
     * resourceFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
     void* (*CreatePixelDataFromPNG)(const char* resourceFilePath, float* outWidth, float* outHeight);
};


extern struct AImage AImage[1];


#endif
