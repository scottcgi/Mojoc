/*
 * Copyright (c) 2017-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since    : 2015-9-16
 * Author   : scott.cgi
 */

#ifndef GAME_MAP_H
#define GAME_MAP_H


#include "Engine/Application/Component.h"
#include "Engine/Extension/Spine/SkeletonAnimationPlayer.h"


struct AGameMap
{
    Component                component[1];
    SkeletonAnimationPlayer* mapPlayer;

    float                    scaleX;
    float                    scaleY;

    float                    minX;
    float                    maxX;

    float                    groundY;
    float                    moveAreaY;
    float*                   groundPosData;
    Drawable*                groundPosDrawable;

    Drawable*                backDrawable;
    Drawable*                middleDrawable;
    Drawable*                beforeDrawable;
    Drawable*                skyDrawable;

    void (*Init)     ();
    void (*Run)      ();
    void (*RandomMap)();
};


extern struct AGameMap AGameMap[1];


#endif
