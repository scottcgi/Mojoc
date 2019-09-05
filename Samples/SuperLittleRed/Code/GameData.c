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


#include "GameData.h"


struct AGameData AGameData[1] =
{{
    .stone              = 0,
    .wood               = 0,
    .coin               = 0,

    .maxScore           = 0,
    .speed              = 1.2f,
    .maxHP              = 1,
    .maxEnergy          = 100,

    .recoverEnergy      = 5,
    .dizzyDistance      = 0.4f,
    .dizzyTime          = 2,

    .stonePower         = 0.05f,
    .woodTime           = 5.0f,

    .arrowSpeed         = 1.0f,
    .arrowPower         = 1.0f,

    .hitFloorCostEnergy = 10,
    .arrowCostEnergy    = 5,

    .tmp0               = 0,
    .playGameCount      = 0,
    .maxRoundCount      = 0,
    .maxRoundTime       = 0.0f,
    .maxRoundKillCount  = 0,

    .weaponIndex        = StoreWeapon_Wood,
    .dressIndex         = StoreDress_RedGirl,

    .buyWeapons         = {StoreWeapon_Wood,   -1, -1, -1, -1, -1, -1, -1, -1, -1},
    .buyDresses         = {StoreDress_RedGirl, -1, -1, -1, -1, -1},

    .maxProperty        =
    {
      1.7f,
      11.0f,
      200.0f,
      10.0f,
      0.9f,
      7.0f,
      0.15f,
      15.0f,
      1.5f,
      2.0f,
    },

    .increaseProperty   =
    {
        0.1f,
        2.0f,
        20.0f,
        1.0f,
        0.1f,
        1.0f,
        0.02f,
        2.0f,
        0.1f,
        0.2f,
    },
}};