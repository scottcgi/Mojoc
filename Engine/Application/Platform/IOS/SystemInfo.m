/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2019-5-8
 * Author : scott.cgi
 */


#include "Engine/Toolkit/Platform/Platform.h"


#ifdef IS_PLATFORM_IOS


#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Application/Platform/SystemInfo.h"


static void GetLanguageCode(char* outLanguageCode)
{
    outLanguageCode[0] = 'e';
    outLanguageCode[1] = 'n';
}


struct ASystemInfo ASystemInfo[1] =
{
    GetLanguageCode,
};


#endif
