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
 * Since    : 2015-9-16
 * Update   : 2019-2-16
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
    int                      mapIndex;

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

    void (*Init)     (void);
    void (*Run)      (void);
    void (*RandomMap)(void);
};


extern struct AGameMap AGameMap[1];


#endif
