/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2017-3-24
 * Author: scott.cgi
 */

#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Application/Input.h"
#include "Engine/Graphics/OpenGL/GLTool.h"


#define finger_count 10
static  InputTouch touches[finger_count];


//--------------------------------------------------------------------------------------------------


static InputTouch* SetTouch(int fingerId, float x, float y, InputTouchType type)
{
    InputTouch* touch = touches + fingerId;

    touch->fingerId   = fingerId;
    touch->x          = AGLToolToGLX(x);
    touch->y          = AGLToolToGLY(y);
    touch->type       = type;

    return touch;
}


static InputTouch* GetTouch(int fingerId)
{
    ALogA(fingerId > -1 && fingerId < 10, "AInput GetTouch fingerId = %d invalid", fingerId);
    return touches + fingerId;
}


struct AInput AInput[1] =
{
    SetTouch,
    GetTouch,
};
