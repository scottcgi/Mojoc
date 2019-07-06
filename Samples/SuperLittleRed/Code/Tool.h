/*
 * Copyright (c) 2017-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2015-4-26
 * Author : scott.cgi
 */

#ifndef TOOL_H
#define TOOL_H


#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Physics/PhysicsBody.h"


#define SCREEN_DESIGN_WIDTH  960
#define SCREEN_DESIGN_HEIGHT 540


struct ATool
{
    float globalScaleX;
    float globalScaleY;

    void (*UpdateBox)    (PhysicsBody* body, Drawable* drawable);
    void (*InitBox)      (PhysicsBody* body, float x,  float y);

    void (*ShakeX)       (Drawable*    drawable);
    void (*ShakeZ)       (Drawable*    drawable);
    void (*Scale2)       (Drawable*    drawable);

    void (*SetTimeToBuff)(char buff[], int seconds);
};


extern struct ATool ATool[1];


#endif
