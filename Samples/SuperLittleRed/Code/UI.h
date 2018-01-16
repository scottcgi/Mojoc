/*
 * Copyright (c) 2017-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2016-12-20
 * Author: scott.cgi
 */

#ifndef UI_H
#define UI_H


#include "Engine/Application/Component.h"
#include "Engine/Extension/Spine/SkeletonAnimationPlayer.h"


struct AUI
{
    Component               component   [1];
    SkeletonAnimationPlayer uiFailPlayer[1];

    void (*Init)    ();
    void (*Run)     ();
    void (*ShowFail)();
};


extern struct AUI AUI[1];


#endif
