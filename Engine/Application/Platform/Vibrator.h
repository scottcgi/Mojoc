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
 * Since    : 2019-8-5
 * Update   : 2019-8-5
 * Author   : scott.cgi
 */

#ifndef VIBRATOR_H
#define VIBRATOR_H


/**
 * Control device vibrate.
 *
 * Android: requires <uses-permission android:name="android.permission.VIBRATE"/>
 * IOS    : only support two types that milliseconds in [<= 50 and > 50].
 */
struct AVibrator
{
    void (*Vibrate)(int milliseconds);
};


extern struct AVibrator AVibrator[1];


#endif
