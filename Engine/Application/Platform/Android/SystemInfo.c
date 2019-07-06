/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License.
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub.
 * This Mojoc game engine is a serious project with extreme code pursuit, and will continue to iterate.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2018-12-6
 * Author : scott.cgi
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
