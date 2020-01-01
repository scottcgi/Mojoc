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
 * Since    : 2018-12-6
 * Update   : 2019-1-28
 * Author   : scott.cgi
 */


#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H


#include <stdbool.h>


/**
 * Get system info in native platform.
 */
struct ASystemInfo
{
    /**
     * The outLanguageCode will be filled with an array of two characters.
     */
    void (*GetLanguageCode)(char outLanguageCode[2]);
};


extern struct ASystemInfo ASystemInfo[1];


#endif
