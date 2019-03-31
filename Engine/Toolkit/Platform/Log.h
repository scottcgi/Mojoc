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
 * Since : 2012-12-20
 * Update: 2019-1-8
 * Author: scott.cgi
 */

 
#ifndef LOG_H
#define LOG_H


#include <assert.h>
#include "Engine/Toolkit/Platform/Platform.h"


#ifdef IS_PLATFORM_ANDROID
    #include <android/log.h>
    #define  ALog_D(...)    __android_log_print(ANDROID_LOG_DEBUG, "Mojoc debug", __VA_ARGS__)
    #define  ALog_W(...)    __android_log_print(ANDROID_LOG_WARN , "Mojoc warn" , __VA_ARGS__)
    #define  ALog_E(...)    __android_log_print(ANDROID_LOG_ERROR, "Mojoc error", __VA_ARGS__)
    #define  ALog_A(e, ...) e ? (void) 0 : ALog_E(__VA_ARGS__),  assert(e);
#elif defined(IS_PLATFORM_IOS)
    #include <stdio.h>
    #define  ALog_D(...)
    #define  ALog_W(...)
    #define  ALog_E(...)    printf(__VA_ARGS__), printf("\n");
    #define  ALog_A(e, ...) e ? (void) 0 : printf(__VA_ARGS__), printf("\n"),  assert(e);
#endif


#endif

