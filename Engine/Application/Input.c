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


#define CheckFingerID(tag, fingerID) \
    ALog_A((fingerID) > -1 && (fingerID) < Finger_Num, "AInput " tag " fingerID = %d invalid", fingerID)


static InputTouch touches[Finger_Num];


static InputTouch* SetTouch(int fingerID, float pixelX, float pixelY, InputTouchType type)
{
    CheckFingerID("SetTouch", fingerID);

    InputTouch* touch = touches + fingerID;
    touch->fingerID   = fingerID;
    touch->x          = AGLTool_ToGLX(pixelX);
    touch->y          = AGLTool_ToGLY(pixelY);
    touch->type       = type;

    return touch;
}


static InputTouch* GetTouch(int fingerID)
{
    CheckFingerID("GetTouch", fingerID);
    return touches + fingerID;
}


#undef CheckFingerID


struct AInput AInput[1] =
{{
    SetTouch,
    GetTouch,
}};
