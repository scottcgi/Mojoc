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
 * The Mojoc game engine is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2019-5-8
 * Author: scott.cgi
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
