/**
 *
 *
 * Created on: 2012-12-20
 * Author: scott.cgi
 */

#ifndef log_h
#define log_h

#include "Engine/Toolkit/Platform/Platform.h"
#include <assert.h>

//--------------------------------------------------------------------------------------------------
#ifdef is_platform_android
//--------------------------------------------------------------------------------------------------

#include <android/log.h>

#define ALogD(...) // __android_log_print(ANDROID_LOG_DEBUG, "MojocLib debug", __VA_ARGS__)
#define ALogI(...)    __android_log_print(ANDROID_LOG_INFO,  "MojocLib info",  __VA_ARGS__)
#define ALogW(...)    __android_log_print(ANDROID_LOG_WARN,  "MojocLib warn",  __VA_ARGS__)
#define ALogE(...)    __android_log_print(ANDROID_LOG_ERROR, "MojocLib error", __VA_ARGS__)
#define ALogA(e, ...) e ? (void) 0 : ALogE(__VA_ARGS__),  assert(e);

//--------------------------------------------------------------------------------------------------
#elif defined(is_platform_ios)
//--------------------------------------------------------------------------------------------------

#include <stdio.h>

#define ALogD(...) printf(__VA_ARGS__)
#define ALogI(...)
#define ALogW(...)
#define ALogE(...)
#define ALogA(e, ...) e ? (void) 0 : printf(__VA_ARGS__);printf("\n"),  assert(e);

//--------------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------


#endif

