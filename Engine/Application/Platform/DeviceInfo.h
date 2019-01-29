/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2018-12-6
 * Update: 2019-1-28
 * Author: scott.cgi
 */


#ifndef DEVICE_INFO_H
#define  DEVICE_INFO_H


#include <stdbool.h>


struct ADeviceInfo
{
    /**
     * The outLanguageCode will be filled with an array of two characters.
     */
    void (*GetLanguageCode)(char outLanguageCode[2]);
};


extern struct ADeviceInfo ADeviceInfo[1];


#endif
