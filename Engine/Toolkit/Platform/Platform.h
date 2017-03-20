/*
 *
 *
 *  Created on: 2015-5-6
 *      Author: scott.cgi
 */

#ifndef platform_h
#define platform_h

#define platform_android 0
#define platform_ios     1
#define platform_target  platform_ios

//--------------------------------------------------------------------------------------------------
#if platform_target == platform_android

#define is_platform_android

#elif platform_target == platform_ios

#define is_platform_ios

#endif
//--------------------------------------------------------------------------------------------------




#endif
