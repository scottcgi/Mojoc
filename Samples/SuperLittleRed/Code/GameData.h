/*
 * Copyright (c) 2017-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2017-1-7
 * Author : scott.cgi
 */

#ifndef GAME_DATA_H
#define GAME_DATA_H


struct AGameData
{
    int                     stone;
    int                     wood;
    int                     coin;

//----------------------------------------------------------------------------------------------------------------------
    float                   speed;
    float                   maxHP;
    int                     maxEnergy;

//----------------------------------------------------------------------------------------------------------------------

    int                     recoverEnergy;
    float                   dizzyDistance;
    int                     dizzyTime;

    float                   stonePower;
    float                   woodTime;

    float                   arrowSpeed;
    float                   arrowPower;

//----------------------------------------------------------------------------------------------------------------------

    int                     hitFloorCostEnergy;
    int                     arrowCostEnergy;
};


extern struct AGameData AGameData[1];


#endif
