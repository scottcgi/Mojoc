/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2017-3-24
 * Update: 2019-1-25
 * Author: scott.cgi
 */


#ifndef INPUT_H
#define INPUT_H


#include "Engine/Toolkit/HeaderUtils/Define.h"


typedef enum
{
    InputTouchType_Up     = 1,
    InputTouchType_Down   = 2,
    InputTouchType_Move   = 3,
    InputTouchType_Cancel = 4,
}
InputTouchType;


/**
 * Input touch event info.
 */
typedef struct
{
    /* Use openGL coordinate. */
    float          x;
    float          y;

    /**
     * Identify touch point, [0, 9] corresponds to 10 fingers.
     */
    int            fingerId;
    
    InputTouchType type;
}
InputTouch;


/**
 * Manage and control InputTouch.
 */
struct AInput
{
    /**
     * Set InputTouch with fingerId, and later can get by fingerId.
     * the x y is in screen pixel coordinate.
     * return the InputTouch that fingerId associated.
     */
    InputTouch* (*SetTouch)(int fingerId, float pixelX, float pixelY, InputTouchType type);

    /**
     * Get the InputTouch that fingerId associated.
     */
    InputTouch* (*GetTouch)(int fingerId);
};


extern struct AInput AInput[1];


#endif
