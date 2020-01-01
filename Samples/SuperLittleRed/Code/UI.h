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
 * Since    : 2016-12-20
 * Update   : 2019-2-18
 * Author   : scott.cgi
 */


#ifndef UI_H
#define UI_H


#include "Engine/Application/Component.h"
#include "Engine/Extension/Spine/SkeletonAnimationPlayer.h"


enum
{
    StoreWeapon_Copper,
    StoreWeapon_Dragon,
    StoreWeapon_Gold,
    StoreWeapon_Heart,
    StoreWeapon_Ice,
    StoreWeapon_Iron,
    StoreWeapon_Lightning,
    StoreWeapon_Silver,
    StoreWeapon_steel,
    StoreWeapon_Wood,
    StoreWeapon_Length,
};


enum
{
    StoreDress_Armour,
    StoreDress_RedGirl,
    StoreDress_Bunny,
    StoreDress_Egypt,
    StoreDress_Luffy,
    StoreDress_Swan,
    StoreDress_Length,
};


enum
{
    StoreProperty_Speed,
    StoreProperty_HP,
    StoreProperty_Energy,
    StoreProperty_EnergyRecovery,
    StoreProperty_DizzyDistance,
    StoreProperty_DizzyTime,
    StoreProperty_StonePower,
    StoreProperty_WoodTime,
    StoreProperty_ArrowSpeed,
    StoreProperty_ArrowPower,
    StoreProperty_Length,
};


struct AUIStore
{
    const char* weaponNames  [StoreWeapon_Length];
    const char* weaponBoxes  [StoreWeapon_Length];
    const char* weaponIcons  [StoreWeapon_Length];
    const char* weaponLocks  [StoreWeapon_Length];
    const char* dressNames   [StoreDress_Length];
    const char* dressBoxes   [StoreDress_Length];
    const char* dressIcons   [StoreDress_Length];
    const char* dressLocks   [StoreDress_Length];
    const char* propertyNames[StoreProperty_Length];
    const char* propertyBoxes[StoreProperty_Length];
};


extern struct AUIStore AUIStore[1];


struct AUI
{
    Component               component       [1];
    SkeletonAnimationPlayer uiFailPlayer    [1];
    SkeletonAnimationPlayer uiMenuPlayer    [1];
    SkeletonAnimationPlayer uiRecordPlayer  [1];
    SkeletonAnimationPlayer uiTutorialPlayer[1];

    void (*Init)    (void);
    void (*Run)     (void);
    void (*ShowFail)(void);
    void (*ShowMenu)(void);
};


extern struct AUI AUI[1];


#endif
