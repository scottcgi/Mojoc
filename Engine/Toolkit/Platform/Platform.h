/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2015-5-6
 * Update: 2019-1-8
 * Author: scott.cgi
 */

 
#ifndef PLATFORM_H
#define PLATFORM_H


#define PLATFORM_ANDROID 0
#define PLATFORM_IOS     1
#define PLATFORM_TARGET  PLATFORM_ANDROID


#if PLATFORM_TARGET == PLATFORM_ANDROID
    #define IS_PLATFORM_ANDROID
#elif PLATFORM_TARGET == PLATFORM_IOS
    #define IS_PLATFORM_IOS
#endif


#endif
