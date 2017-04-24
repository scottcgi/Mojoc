/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2017-3-24
 * Author: scott.cgi
 */

#ifndef input_h
#define input_h


#include "Engine/Toolkit/Head/MacroDefine.h"


typedef enum
{
    input_touch_up     = 1,
    input_touch_down   = 2,
    input_touch_move   = 3,
    input_touch_cancel = 4,
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
