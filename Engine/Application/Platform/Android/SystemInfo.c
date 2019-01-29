/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2018-12-6
 * Author: scott.cgi
 */


#include "Engine/Toolkit/Platform/Platform.h"


#ifdef IS_PLATFORM_ANDROID


#include <android/configuration.h>
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Application/Platform/SystemInfo.h"


extern AConfiguration* nativeActivityConfig;


static void GetLanguageCode(char* outLanguageCode)
{
    AConfiguration_getLanguage(nativeActivityConfig, outLanguageCode);
}


struct ASystemInfo ASystemInfo[1] =
{
    GetLanguageCode,
};


#endif
