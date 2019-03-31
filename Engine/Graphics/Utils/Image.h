/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The project Mojoc is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2013-08-30
 * Update: 2019-1-19
 * Author: scott.cgi
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
