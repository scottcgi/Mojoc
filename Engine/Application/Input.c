/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2017-3-24
 * Author: scott.cgi
 */

#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Application/Input.h"
#include "Engine/Graphics/OpenGL/GLTool.h"


#define FINGER_COUNT 10
static  InputTouch touches[FINGER_COUNT];


//----------------------------------------------------------------------------------------------------------------------


static InputTouch* SetTouch(int fingerId, float x, float y, InputTouchType type)
{
    InputTouch* touch = touches + fingerId;

    touch->fingerId   = fingerId;
    touch->x          = AGLTool_ToGLX(x);
    touch->y          = AGLTool_ToGLY(y);
    touch->type       = type;

    return touch;
}


static InputTouch* GetTouch(int fingerId)
{
    ALog_A(fingerId > -1 && fingerId < 10, "AInput GetTouch fingerId = %d invalid", fingerId);
    return touches + fingerId;
}


struct AInput AInput[1] =
{
    SetTouch,
    GetTouch,
};
