/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2017-3-24
 * Update   : 2019-1-25
 * Author   : scott.cgi
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
     * IDentify touch point, [0, 9] corresponds to 10 fingers.
     */
    int            fingerID;
    
    InputTouchType type;
}
InputTouch;


/**
 * Manage and control InputTouch.
 */
struct AInput
{
    /**
     * Set InputTouch with fingerID, and later can get by fingerID.
     * the x y is in screen pixel coordinate.
     * return the InputTouch that fingerID associated.
     */
    InputTouch* (*SetTouch)(int fingerID, float pixelX, float pixelY, InputTouchType type);

    /**
     * Get the InputTouch that fingerID associated.
     */
    InputTouch* (*GetTouch)(int fingerID);
};


extern struct AInput AInput[1];


#endif
