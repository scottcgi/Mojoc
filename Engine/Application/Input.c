/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2017-3-24
 * Update: 2019-1-25
 * Author: scott.cgi
 */


#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Application/Input.h"
#include "Engine/Graphics/OpenGL/GLTool.h"


enum
{
    Finger_Num = 10,
};


#define CheckFingerId(tag, fingerId) \
    ALog_A((fingerId) > -1 && (fingerId) < Finger_Num, "AInput " tag " fingerId = %d invalid", fingerId)


static InputTouch touches[Finger_Num];


static InputTouch* SetTouch(int fingerId, float pixelX, float pixelY, InputTouchType type)
{
    CheckFingerId("SetTouch", fingerId);

    InputTouch* touch = touches + fingerId;
    touch->fingerId   = fingerId;
    touch->x          = AGLTool_ToGLX(pixelX);
    touch->y          = AGLTool_ToGLY(pixelY);
    touch->type       = type;

    return touch;
}


static InputTouch* GetTouch(int fingerId)
{
    CheckFingerId("GetTouch", fingerId);
    return touches + fingerId;
}


#undef CheckFingerId


struct AInput AInput[1] =
{
    SetTouch,
    GetTouch,
};
