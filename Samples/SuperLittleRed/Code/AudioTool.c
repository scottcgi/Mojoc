/*
 * Copyright (c)  2017-2018  scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2017-2-20
 * Author : scott.cgi
 */

#include <stddef.h>

#include "Engine/Toolkit/Head/UserData.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Audio/Platform/Audio.h"
#include "AudioTool.h"


static const char* ids[AudioId_Length] =
{
    "Audio/ClickBtn.mp3",
    "Audio/CurtainSlideShow.mp3",
    "Audio/CurtainSlideHide.mp3",
    "Audio/Shoot.mp3",
    "Audio/HurtEnemy.mp3",
    "Audio/HurtGround.mp3",
    "Audio/Drop.mp3",
    "Audio/PickCoin.mp3",
    "Audio/PickHeart.mp3",
    "Audio/PickStone.mp3",
    "Audio/PickWood.mp3",
    "Audio/HurtHero.mp3",
    "Audio/Die.mp3",
    "Audio/CurtainRing.mp3",
    "Audio/ShootFaster.mp3",
    "Audio/FallDown.mp3",
    "Audio/NoArrow.mp3",
    "Audio/HeroDie.mp3",
    "Audio/BG1.mp3",
    "Audio/BG2.mp3",
};


static AudioPlayer* uiBG     = NULL;
static AudioPlayer* gameBG   = NULL;
static float        volume   = 0;
static float        maxBG    = 0.4f;
static float        interval = 0.05f;


static void VolumeUpdate(Scheduler* scheduler, float deltaSeconds)
{
    volume += interval;

    if (volume > maxBG)
    {
        volume = maxBG;
    }

    if (scheduler->userData->slot0->intValue == 0)
    {
        AAudio->SetVolume(uiBG, volume);

        if (gameBG != NULL)
        {
            AAudio->SetVolume(gameBG, maxBG - volume);
        }

    }
    else
    {
        AAudio->SetVolume(gameBG, volume);

        if (uiBG != NULL)
        {
            AAudio->SetVolume(uiBG, maxBG - volume);
        }
    }

    if (volume == maxBG)
    {
        scheduler->isCancel = true;
        volume              = 0;
    }
}


static void StartUIBG()
{
    uiBG = AAudioTool->Play(AudioId_BG2);
    AAudio->SetLoop  (uiBG, true);
    AAudio->SetVolume(uiBG, 0);

    if (gameBG != NULL)
    {
        AAudio->SetLoop  (gameBG, false);
        AAudio->SetVolume(gameBG, 0);
    }

    AScheduler->Schedule(VolumeUpdate, interval)->userData->slot0->intValue = 0;
}


static void StartGameBG()
{
    gameBG = AAudioTool->Play(AudioId_BG1);
    AAudio->SetLoop  (gameBG, true);
    AAudio->SetVolume(gameBG, 0);

    if (uiBG != NULL)
    {
        AAudio->SetLoop  (uiBG, false);
        AAudio->SetVolume(uiBG, 0);
    }

    AScheduler->Schedule(VolumeUpdate, interval)->userData->slot0->intValue = 1;
}


static AudioPlayer* Play(AudioId id)
{
    AudioPlayer* player = AAudio->GetPlayer((char*) ids[id]);

    AAudio->SetPlay  (player);
    AAudio->SetVolume(player, 0.6f);
    return player;
}


struct AAudioTool AAudioTool[1] =
{
    Play,
    StartUIBG,
    StartGameBG,
};
