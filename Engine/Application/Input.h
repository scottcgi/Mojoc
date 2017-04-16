//
// Created by scott.cgi on 2017/3/24.
//

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
    /** OpenGL coordinate */
    float          get_only x;
    float          get_only y;

    /**
     * Identify touch point
     * 0-9 corresponding 10 fingers
     */
    int            get_only fingerId;
    InputTouchType get_only type;
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
