/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2017-3-24
 * Author: scott.cgi
 */

#ifndef INPUT_H
#define INPUT_H


#include "Engine/Toolkit/Head/Define.h"


typedef enum
{
    InputTouchType_Up     = 1,
    InputTouchType_Down   = 2,
    InputTouchType_Move   = 3,
    InputTouchType_Cancel = 4,
}
InputTouchType;


typedef struct
{
    /**
     * Use openGL coordinate
     */
    float          x;
    float          y;

    /**
     * Identify touch point
     * 0-9 corresponding 10 fingers
     */
    int            fingerId;
    InputTouchType type;
}
InputTouch;


struct AInput
{
    /**
     * The x, y in screen coordinate
     * return the InputTouch that fingerId associated
     */
    InputTouch* (*SetTouch)(int fingerId, float x, float y, InputTouchType type);

    /**
     * Get the InputTouch that fingerId associated
     */
    InputTouch* (*GetTouch)(int fingerId);
};


extern struct AInput AInput[1];


#endif
