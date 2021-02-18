/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2019-8-6
 * Update   : 2021-2-10
 * Author   : scott.cgi
 */


#include "Engine/Toolkit/Platform/Platform.h"


//--------------------
#ifdef IS_PLATFORM_IOS
//--------------------


#include <AudioToolbox/AudioToolbox.h>
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Application/Platform/Vibrator.h"


static void Vibrate(int milliseconds)
{
    if (milliseconds <= 50)
    {
        // 3D Touch Peek
        AudioServicesPlaySystemSound(1519);
    }
    else
    {
        // 3D Touch Pop
        AudioServicesPlaySystemSound(1520);
    }
}


struct AVibrator AVibrator[1] =
{{
     Vibrate,
}};


//-----------------------
#endif // IS_PLATFORM_IOS
//-----------------------
