//
// Created by scott.cgi on 2017/3/24.
//

#include "Engine/Application/Input.h"
#include "Engine/Graphics/OpenGL/GLTool.h"

#define finger_count 10
static  InputTouch touches[finger_count];

//--------------------------------------------------------------------------------------------------

static InputTouch* GetTouch(int fingerId, float x, float y, InputTouchType type)
{
    InputTouch* touch = touches + fingerId;

    touch->fingerId   = fingerId;
    touch->x          = AGLToolToGLX(x);
    touch->y          = AGLToolToGLY(y);
    touch->type       = type;

    return touch;
}


struct AInput AInput[1] =
{
    GetTouch,
};
