//
// Created by scott.cgi on 2017/3/24.
//

#ifndef input_h
#define input_h

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
     */
    InputTouch* (*GetTouch)(int fingerId, float x, float y, InputTouchType type);
};

extern struct AInput AInput[1];

#endif
