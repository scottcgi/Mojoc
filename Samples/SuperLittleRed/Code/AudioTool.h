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
 * Since    : 2017-2-20
 * Update   : 2019-2-16
 * Author   : scott.cgi
 */


#ifndef AUDIO_TOOL_H
#define AUDIO_TOOL_H


#include "Engine/Audio/Platform/Audio.h"


typedef enum
{
    AudioID_ClickBtn,
    AudioID_CurtainSlideShow,
    AudioID_CurtainSlideHide,
    AudioID_Shoot,
    AudioID_HurtEnemy,
    AudioID_HurtGround,
    AudioID_Drop,
    AudioID_PickCoin,
    AudioID_PickHeart,
    AudioID_PickStone,
    AudioID_PickWood,
    AudioID_HurtHero,
    AudioID_Over,
    AudioID_CurtainRing,
    AudioID_ShootFast,
    AudioID_FallDown,
    AudioID_NoArrow,
    AudioID_HeroDie,
    AudioID_BG11,
    AudioID_BG21,
    AudioID_BG22,

    AudioID_Length,
}
AudioID;


struct AAudioTool
{
    AudioPlayer* (*Play)        (AudioID id);
    void         (*StartUIBG)   (void);
    void         (*StartGameBG) (void);
    void         (*PauseGameBG) (void);
    void         (*ResumeGameBG)(void);
};


extern struct AAudioTool AAudioTool[1];


#endif
