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
 * Since    : 2015-4-13
 * Update   : 2019-2-16
 * Author   : scott.cgi
 */


#ifndef CONFIG_H
#define CONFIG_H


#define APP_DEBUG
#define APP_SHOW_FPS_AND_DRAW_CALL
//#define APP_NO_ENEMY


enum
{
    Screen_DesignWidth     = 960,
    Screen_DesignHeight    = 540,
    
    Enemy_KindsNum         = 1,
    Enemy_EachKindInitNum  = 10,
    Enemy_KillIncrease     = 20,
    Enemy_TimeIncrease     = 15,
    Enemy_BossInterval     = 10,
    Enemy_BossHPX          = 5,
    Enemy_BossSpeedX       = 2,

    AppInit_FunctionsCount = 8,
};


static const float heroEnergyRecoverTime = 1.8f;
static const float enemyDoAITime         = 1.0f;
static const float enemyCreateTime       = 2.0f;


#endif
