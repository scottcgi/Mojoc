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
 * Since    : 2019-8-5
 * Update   : 2019-8-5
 * Author   : scott.cgi
 */


#include "Engine/Toolkit/Platform/Platform.h"


//------------------------
#ifdef IS_PLATFORM_ANDROID
//------------------------


#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Application/Platform/Vibrator.h"
#include "Engine/Application/Platform/Android/JniTool.h"


static void Vibrate(long milliseconds)
{
    jobject vibrator    = AJniTool->CallNativeActivityMethod
                          (
                              "getSystemService",
                              "(Ljava/lang/String;)Ljava/lang/Object;",
                              AJniTool->GetStaticField("android/content/Context", "VIBRATOR_SERVICE", "Ljava/lang/String;").l
                          ).l;

    AJniTool->CallMethod(vibrator, "vibrate", "(J)V", milliseconds);
}


struct AVibrator AVibrator[1] =
{{
     Vibrate,
}};


//---------------------------
#endif // IS_PLATFORM_ANDROID
//---------------------------
