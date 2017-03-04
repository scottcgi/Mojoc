/**
 *
 *
 * Created on: 2012-12-20
 * Author: scott.cgi
 */

#ifndef log_h
#define log_h

#undef NDEBUG
#undef __cplusplus

#include <android/log.h>
#include <assert.h>

//--------------------------------------------------------------------------------------------------
// The macro can use varargs
//--------------------------------------------------------------------------------------------------

#define ALogD(...) // __android_log_print(ANDROID_LOG_DEBUG, "MojocLib debug", __VA_ARGS__)
#define ALogI(...)    __android_log_print(ANDROID_LOG_INFO,  "MojocLib info",  __VA_ARGS__)
#define ALogW(...)    __android_log_print(ANDROID_LOG_WARN,  "MojocLib warn",  __VA_ARGS__)
#define ALogE(...)    __android_log_print(ANDROID_LOG_ERROR, "MojocLib error", __VA_ARGS__)
#define ALogA(e, ...) e ? (void) 0 : ALogE(__VA_ARGS__),  assert(e);


#endif

