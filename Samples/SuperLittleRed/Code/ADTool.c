/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2017-2-25
 * Update   : 2019-7-31
 * Author   : scott.cgi
 */


#include "Engine/Toolkit/Platform/Platform.h"
#include "ADTool.h"


//------------------------
#ifdef IS_PLATFORM_ANDROID
//------------------------


#include "Engine/Application/Platform/Android/JniTool.h"


static void Show(ADType type)
{
}


static void Hide(ADType type)
{
}


#elif defined(IS_PLATFORM_IOS)


static void Show(ADType type)
{
}

static void Hide(ADType type)
{
}


//---------------------------
#endif // IS_PLATFORM_ANDROID
//---------------------------


struct AADTool AADTool[1] =
{{
    Show,
    Hide,
}};


