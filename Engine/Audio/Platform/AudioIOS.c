/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2017-3-12
 * Author : scott.cgi
 */

#include "Engine/Toolkit/Platform/Platform.h"


//--------------------------------------------------------------------------------------------------
#ifdef is_platform_ios
//--------------------------------------------------------------------------------------------------


#include <stddef.h>
#include "Engine/Audio/Platform/Audio.h"


static void Update(float deltaSeconds)
{

}


static void SetLoopPause()
{

}


static void SetLoopResume()
{

}


static void Init()
{

}


static inline void InitPlayer(char* filePath, AudioPlayer* player)
{

}


static void SetLoop(AudioPlayer* player, bool isLoop)
{

}


static void SetVolume(AudioPlayer* player, int volume)
{

}


static void SetPlay(AudioPlayer* player)
{

}


static void SetPause(AudioPlayer* player)
{

}


static bool IsPlaying(AudioPlayer* player)
{
    return false;
}


static AudioPlayer* GetPlayer(char* filePath)
{
    return NULL;
}


struct AAudio AAudio[1] =
{
    Init,
    Update,
    SetLoopPause,
    SetLoopResume,
    GetPlayer,

    SetVolume,
    SetLoop,

    SetPlay,
    SetPause,
    IsPlaying,
};


//--------------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------
