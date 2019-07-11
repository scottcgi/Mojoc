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
 * Since    : 2017-3-24
 * Update   : 2019-1-25
 * Author   : scott.cgi
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
{{
    SetTouch,
    GetTouch,
}};
