/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2017-1-7
 * Update   : 2019-2-16
 * Author   : scott.cgi
 */


#ifndef GAME_DATA_H
#define GAME_DATA_H


#include "UI.h"


struct AGameData
{
    int                     stone;
    int                     wood;
    int                     coin;
    int                     maxScore;

    float                   speed;
    float                   maxHP;
    int                     maxEnergy;
    int                     recoverEnergy;
    float                   dizzyDistance;
    int                     dizzyTime;
    float                   stonePower;
    float                   woodTime;
    float                   arrowSpeed;
    float                   arrowPower;

    int                     hitFloorCostEnergy;
    int                     arrowCostEnergy;

    int                     tmp0;
    int                     playGameCount;
    int                     maxRoundCount;
    float                   maxRoundTime;
    int                     maxRoundKillCount;

    int                     weaponIndex;
    int                     dressIndex;

    int                     buyWeapons      [StoreWeapon_Length];
    int                     buyDresses      [StoreDress_Length];

    float                   maxProperty     [StoreProperty_Length];
    float                   increaseProperty[StoreProperty_Length];
};


extern struct AGameData AGameData[1];


#endif
