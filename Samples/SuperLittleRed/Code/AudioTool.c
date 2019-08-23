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
 * Update   : 2019-2-17
 * Author   : scott.cgi
 */


#include <stddef.h>
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/HeaderUtils/UserData.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Audio/Platform/Audio.h"
#include "AudioTool.h"
#include "GameMap.h"


static const char* ids[AudioID_Length] =
{
    "Audio/ClickBtn.aac",
    "Audio/CurtainSlideShow.aac",
    "Audio/CurtainSlideHide.aac",
    "Audio/Shoot.aac",
    "Audio/HurtEnemy.aac",
    "Audio/HurtGround.aac",
    "Audio/Drop.aac",
    "Audio/PickCoin.aac",
    "Audio/PickHeart.aac",
    "Audio/PickStone.aac",
    "Audio/PickWood.aac",
    "Audio/HurtHero.aac",
    "Audio/Over.aac",
    "Audio/CurtainRing.aac",
    "Audio/ShootFaster.aac",
    "Audio/FallDown.aac",
    "Audio/NoArrow.aac",
    "Audio/HeroDie.aac",
    "Audio/BG11.aac",
    "Audio/BG21.aac",
    "Audio/BG22.aac",
};


static AudioPlayer* uiBG     = NULL;
static AudioPlayer* gameBG   = NULL;
static float        volume   = 0.0f;
static float        maxBG    = 0.5f;
static float        interval = 0.05f;


static void VolumeUpdate(Scheduler* scheduler, float deltaSeconds)
{
    volume += interval;

    if (volume > maxBG)
    {
        volume = maxBG;
    }

    if (AUserData_GetSlotInt(scheduler->userData, 0) == 0)
    {
        AAudio->SetVolume(uiBG, volume);
    }
    else
    {
        AAudio->SetVolume(gameBG, volume);
    }

    if (volume == maxBG)
    {
        scheduler->isCancel = true;
        volume              = 0;
    }
}


static void StartUIBG()
{
    AudioID bgs[2] = {AudioID_BG21, AudioID_BG22};

    uiBG = AAudioTool->Play(bgs[AMath_RandomInt(0, 1)]);

    AAudio->SetLoop  (uiBG, true);
    AAudio->SetVolume(uiBG, 0);

    if (gameBG != NULL)
    {
        AAudio->Stop(gameBG);
    }

    AUserData_SetSlotInt(AScheduler->Schedule(VolumeUpdate, interval)->userData, 0, 0);
}


static void StartGameBG()
{
    AudioID bgs[1] = {AudioID_BG11};

    gameBG = AAudioTool->Play(bgs[AGameMap->mapIndex]);

    AAudio->SetLoop  (gameBG, true);
    AAudio->SetVolume(gameBG, 0);

    if (uiBG != NULL)
    {
        AAudio->Stop(uiBG);
    }

    AUserData_SetSlotInt(AScheduler->Schedule(VolumeUpdate, interval)->userData, 0, 1);
}


static AudioPlayer* Play(AudioID id)
{
    AudioPlayer* player = AAudio->GetPlayer((char*) ids[id]);

    AAudio->Play     (player);
    AAudio->SetVolume(player, 0.6f);
    return player;
}


static void PauseGameBG(void)
{
    AAudio->Pause(gameBG);
}


static void ResumeGameBG(void)
{
    AAudio->Play(gameBG);
}


struct AAudioTool AAudioTool[1] =
{{
    Play,
    StartUIBG,
    StartGameBG,
    PauseGameBG,
    ResumeGameBG,
}};
