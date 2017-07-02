/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2015-5-6
 * Author : scott.cgi
 * Version: 0.0.0
 */


#ifndef PLATFORM_H
#define PLATFORM_H


#define PLATFORM_ANDROID 0
#define PLATFORM_IOS     1
#define PLATFORM_TARGET  PLATFORM_ANDROID


//----------------------------------------------------------------------------------------------------------------------


#if PLATFORM_TARGET == PLATFORM_ANDROID

    #define IS_PLATFORM_ANDROID

#elif PLATFORM_TARGET == PLATFORM_IOS

    #define IS_PLATFORM_IOS

#endif


//----------------------------------------------------------------------------------------------------------------------


#endif
