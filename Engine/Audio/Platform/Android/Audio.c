/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2017-2-16
 * Author: scott.cgi
 */

#include "Engine/Toolkit/Platform/Platform.h"


//--------------------------------------------------------------------------------------------------
#ifdef is_platform_android
//--------------------------------------------------------------------------------------------------


#include <stdlib.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Toolkit/Platform/File.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Audio/Platform/Audio.h"


//--------------------------------------------------------------------------------------------------

// engine interfaces
static SLObjectItf engineObject    = NULL;
static SLEngineItf engineEngine    = NULL;
// output mix interfaces
static SLObjectItf outputMixObject = NULL;

//--------------------------------------------------------------------------------------------------

struct AudioPlayer
{
    SLObjectItf object;
    SLPlayItf   play;
    SLSeekItf   seek;
    SLVolumeItf volume;
};


static ArrayList cacheList   [1] = AArrayListInit(sizeof(AudioPlayer*), 20);
static ArrayList destroyList [1] = AArrayListInit(sizeof(AudioPlayer*), 20);
static ArrayList loopList    [1] = AArrayListInit(sizeof(AudioPlayer*), 5);


//--------------------------------------------------------------------------------------------------

static void Update(float deltaSeconds)
{
    while (destroyList->size > 0)
    {
        AudioPlayer* player = AArrayListPop(destroyList, AudioPlayer*);
        AArrayListAdd(cacheList, player);
        (*player->object)->Destroy(player->object);
    }
}


static void SetLoopPause()
{
    for (int i = 0; i < loopList->size; i++)
    {
        AAudio->SetPause(AArrayListGet(loopList, i, AudioPlayer*));
    }
}


static void SetLoopResume()
{
    for (int i = 0; i < loopList->size; i++)
    {
        AAudio->SetPlay(AArrayListGet(loopList, i, AudioPlayer*));
    }
}


static void Init()
{
    SLresult result;

    // create engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    ALogA(result == SL_RESULT_SUCCESS, "Audio Init slCreateEngine error");

    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_TRUE);
    ALogA(result == SL_RESULT_SUCCESS, "Audio Init Realize engineObject error");

    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    ALogA(result == SL_RESULT_SUCCESS, "Audio Init GetInterface error");

    SLInterfaceID ids[0];
    SLboolean     req[0];

    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0, ids, req);
    ALogA(result == SL_RESULT_SUCCESS, "Audio Init CreateOutputMix error");

    // realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    ALogA(result == SL_RESULT_SUCCESS, "Audio Init Realize outputMixObject error");
}


static void PlayerCallback(SLPlayItf caller, void *pContext, SLuint32 event)
{
    // play finish
    if (event == SL_PLAYEVENT_HEADATEND)
    {
        AudioPlayer* player = (AudioPlayer*) pContext;
        AArrayListAdd(destroyList, player);
        (*player->play)->SetPlayState(player->play, SL_PLAYSTATE_PAUSED);
    }
}


static inline void InitPlayer(char* filePath, AudioPlayer* player)
{
    off_t start;
    off_t length;
    int   fd = AFile->OpenFileDescriptor(filePath, &start, &length);

    // configure audio source
    SLDataLocator_AndroidFD locFD      = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
    SLDataFormat_MIME       formatMME  = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource            audioSrc   = {&locFD, &formatMME};

    // configure audio sink
    SLDataLocator_OutputMix locOutMix  = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink              audioSnk   = {&locOutMix, NULL};

    // create audio player
    SLInterfaceID           ids[3]     = {SL_IID_SEEK,     SL_IID_PLAY,     SL_IID_VOLUME};
    SLboolean               req[3]     = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    SLresult                result     = (*engineEngine)->CreateAudioPlayer
                                                          (
                                                               engineEngine,
                                                               &player->object,
                                                               &audioSrc,
                                                               &audioSnk,
                                                               3, ids, req
                                                          );

    ALogA(result == SL_RESULT_SUCCESS, "Audio CreatePlayer CreateAudioPlayer error");

    // realize the player
    result = (*player->object)->Realize(player->object, SL_BOOLEAN_FALSE);
    ALogA(result == SL_RESULT_SUCCESS, "Audio CreatePlayer Realize playerObject error");

    // get the play interface
    result = (*player->object)->GetInterface(player->object, SL_IID_PLAY, &player->play);
    ALogA(result == SL_RESULT_SUCCESS, "Audio CreatePlayer GetInterface play error");

    // player callback
    result = (*player->play)->RegisterCallback(player->play, PlayerCallback, player);
    ALogA(result == SL_RESULT_SUCCESS, "Audio CreatePlayer RegisterCallback error");

    result = (*player->play)->SetCallbackEventsMask(player->play,  SL_PLAYEVENT_HEADATEND);
    ALogA(result == SL_RESULT_SUCCESS, "Audio CreatePlayer SetCallbackEventsMask error");

    // get the seek interface
    result = (*player->object)->GetInterface(player->object, SL_IID_SEEK, &player->seek);
    ALogA(result == SL_RESULT_SUCCESS, "Audio CreatePlayer GetInterface seek error");

    // disable looping
    result = (*player->seek)->SetLoop(player->seek, SL_BOOLEAN_FALSE, 0, SL_TIME_UNKNOWN);
    ALogA(result == SL_RESULT_SUCCESS, "Audio CreatePlayer SetLoop error");

    // get the volume interface
    result = (*player->object)->GetInterface(player->object, SL_IID_VOLUME, &player->volume);
    ALogA(result == SL_RESULT_SUCCESS, "Audio CreatePlayer GetInterface volume error");
}


static void SetLoop(AudioPlayer* player, bool isLoop)
{
    SLresult result = (*player->seek)->SetLoop(player->seek, (SLboolean) isLoop, 0, SL_TIME_UNKNOWN);
    ALogA(result == SL_RESULT_SUCCESS, "Audio SetLoop error");

    if (isLoop)
    {
        AArrayListAdd(loopList, player);
    }
    else
    {
        for (int i = 0; i < loopList->size; i++)
        {
            AudioPlayer* p = AArrayListGet(loopList, i, AudioPlayer*);
            if (player == p)
            {
                AArrayList->RemoveByLast(loopList, i);
                break;
            }
        }
    }
}


static void SetVolume(AudioPlayer* player, int volume)
{
    ALogA(volume >= 0 && volume <= 100, "Audio SetVolume volume %d not in [0, 100]", volume);

    SLresult result = (*player->volume)->SetVolumeLevel(player->volume, (SLmillibel) ((1.0f - volume / 100.0f) * -5000));
    ALogA(result == SL_RESULT_SUCCESS, "Audio SetVolume error");
}


static void SetPlay(AudioPlayer* player)
{
    // set the player's state
    SLresult result = (*player->play)->SetPlayState(player->play, SL_PLAYSTATE_PLAYING);
    ALogA(result == SL_RESULT_SUCCESS, "Audio SetPlay error");
}


static void SetPause(AudioPlayer* player)
{
    // set the player's state
    SLresult result = (*player->play)->SetPlayState(player->play, SL_PLAYSTATE_PAUSED);
    ALogA(result == SL_RESULT_SUCCESS, "Audio SetPause error");
}


static bool IsPlaying(AudioPlayer* player)
{
    SLuint32 state;
    (*player->play)->GetPlayState(player->play, &state);

    if (state == SL_PLAYSTATE_PLAYING)
    {
        return true;
    }
    else if (state == SL_PLAYSTATE_PAUSED)
    {
        return false;
    }
    else
    {
        return false;
    }
}


static AudioPlayer* GetPlayer(char* filePath)
{
    AudioPlayer* player = AArrayListPop(cacheList, AudioPlayer*);

    if (player == NULL)
    {
        player = (AudioPlayer*) malloc(sizeof(AudioPlayer));
    }

    InitPlayer(filePath, player);

    return player;
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
