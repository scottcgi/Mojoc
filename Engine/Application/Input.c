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
 * The project Mojoc is a serious project with extreme code pursuit,
 * and will continue to iterate.
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
