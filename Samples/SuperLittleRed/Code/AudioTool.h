/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2017-2-20
 * Author : scott.cgi
 * Version: 0.1
 */


#ifndef AUDIO_TOOL_H
#define AUDIO_TOOL_H


#include "Engine/Audio/Platform/Audio.h"


typedef enum
{
    AudioId_ClickBtn,
    AudioId_CurtainSlideShow,
    AudioId_CurtainSlideHide,
    AudioId_Shoot,
    AudioId_HurtEnemy,
    AudioId_HurtGround,
    AudioId_Drop,
    AudioId_Coin,
    AudioId_PickHeart,
    AudioId_PickStone,
    AudioId_PickWood,
    AudioId_PickHero,
    AudioId_Die,
    AudioId_CurtainRing,
    AudioId_ShootFast,
    AudioId_FallDown,
    AudioId_NoArrow,
    AudioId_HeroDie,
    AudioId_BG1,
    AudioId_BG2,

    AudioId_Length,
}
AudioId;


struct AAudioTool
{
    AudioPlayer* (*Play)       (AudioId id);
    void         (*StartUIBG)  ();
    void         (*StartGameBG)();
};


extern struct AAudioTool AAudioTool[1];


#endif
