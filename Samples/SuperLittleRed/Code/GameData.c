/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2017-1-7
 * Author : scott.cgi
 * Version: 0.1
 */


#include "GameData.h"


struct AGameData AGameData[1] =
{
    {
        .stone              = 0,
        .wood               = 0,
        .coin               = 0,

        .speed              = 1.2f,
        .maxHP              = 1,
        .maxEnergy          = 100,

        .recoverEnergy      = 4,
        .dizzyDistance      = 0.4f,
        .dizzyTime          = 2,

        .stonePower         = 0.05f,
        .woodTime           = 5.0f,

        .arrowSpeed         = 1.0f,
        .arrowPower         = 1.0f,

        .hitFloorCostEnergy = 10,
        .arrowCostEnergy    = 5,
    }
};