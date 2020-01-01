/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2013-08-30
 * Update   : 2019-1-19
 * Author   : scott.cgi
 */


#ifndef IMAGE_H
#define IMAGE_H


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
