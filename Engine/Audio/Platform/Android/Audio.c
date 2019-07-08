/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2017-2-16
 * Update : 2019-1-7
 * Author : scott.cgi
 */

 
#include "Engine/Toolkit/Platform/Platform.h"


#ifdef IS_PLATFORM_ANDROID


#include <stdlib.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Toolkit/Utils/ArrayStrSet.h"
#include "Engine/Toolkit/Platform/File.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Audio/Platform/Audio.h"


// engine interfaces
static SLObjectItf engineObject    = NULL;
static SLEngineItf engineEngine    = NULL;
// output mix interfaces
static SLObjectItf outputMixObject = NULL;


struct AudioPlayer
{
    SLObjectItf object;
    SLPlayItf   play;
    SLSeekItf   seek;
    SLVolumeItf volume;
    const char* filePath;
    int         waitCallbackCount;
};


static ArrayList  (AudioPlayer*) cacheList    [1] = AArrayList_Init (AudioPlayer*, 20);
static ArrayList  (AudioPlayer*) destroyList  [1] = AArrayList_Init (AudioPlayer*, 20);
static ArrayList  (AudioPlayer*) loopList     [1] = AArrayList_Init (AudioPlayer*, 5);
static ArrayList  (AudioPlayer*) testErrorList[1] = AArrayList_Init (AudioPlayer*, 5);
static ArrayStrSet(filePath)     filePathSet  [1] = ArrayStrSet_Init(filePath,     20);


enum
{
    AudioPlayer_WaitMax  = 120,
    AudioPlayer_WaitOver = -1,
};


#define CheckError(tag, filePath)     \
    ALog_A                            \
    (                                 \
        result == SL_RESULT_SUCCESS,  \
        "Audio " tag " failed, "      \
        "error = %x, file path = %s", \
        result,                       \
        filePath                      \
    )


static void Update(float deltaSeconds)
{
    while (destroyList->size > 0)
    {
        AudioPlayer* player = AArrayList_Pop(destroyList, AudioPlayer*);

        // removed from loopList
        AAudio->SetLoop(player, false);

        (*player->object)->Destroy(player->object);

        player->play   = NULL;
        player->volume = NULL;
        player->seek   = NULL;
        player->object = NULL;

        AArrayList_Add(cacheList, player);
    }

    for (int i = testErrorList->size - 1; i > -1 ; --i)
    {
        AudioPlayer* player = AArrayList_Get(testErrorList, i, AudioPlayer*);

        if (player->waitCallbackCount == AudioPlayer_WaitOver)
        {
            AArrayList->RemoveByLast(testErrorList, i);
        }
        else if (player->waitCallbackCount >= AudioPlayer_WaitMax)
        {
            // player callback not called, maybe E/libOpenSLES: Error after prepare: 1
            AArrayList->RemoveByLast(testErrorList, i);
            (*player->object)->Destroy(player->object);
            AArrayList_Add(cacheList, player);

            ALog_E("AAudio player = %s not callback normal", player->filePath);
        }
        else
        {
            ++player->waitCallbackCount;
        }
    }
}


static void SetLoopPause()
{
    for (int i = 0; i < loopList->size; ++i)
    {
        AAudio->Pause(AArrayList_Get(loopList, i, AudioPlayer*));
    }
}


static void SetLoopResume()
{
    for (int i = 0; i < loopList->size; ++i)
    {
        AAudio->Play(AArrayList_Get(loopList, i, AudioPlayer*));
    }
}


static void Init()
{
    SLresult result;

    // create engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    CheckError("Init slCreateEngine", "");

    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_TRUE);
    CheckError("Init Realize engineObject", "");

    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    CheckError("Init GetInterface", "");

    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0, NULL, NULL);
    CheckError("Init CreateOutputMix", "");

    // realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    CheckError("Init Realize outputMixObject", "");
}


static SLmillisecond inline GetDuration(AudioPlayer* player)
{
    SLresult      result;
    SLmillisecond msec;

    result = (*player->play)->GetDuration(player->play, &msec);
    CheckError("GetDuration", "");

    return msec;
}


static void PlayerCallback(SLPlayItf caller, void* pContext, SLuint32 event)
{
    AudioPlayer* player = (AudioPlayer*) pContext;

    switch (event)
    {
        case SL_PLAYEVENT_HEADATEND:
            // play finish
            AArrayList_Add(destroyList, player);
            (*player->play)->SetPlayState(player->play, SL_PLAYSTATE_PAUSED);
            player->waitCallbackCount = AudioPlayer_WaitOver;
            break;

        case SL_PLAYEVENT_HEADATNEWPOS:
            // play normal
            player->waitCallbackCount = AudioPlayer_WaitOver;
            // remove SL_PLAYEVENT_HEADATNEWPOS for reduce the number of callback
            (*player->play)->SetCallbackEventsMask(player->play, SL_PLAYEVENT_HEADATEND);
            break;

        default:
            break;
    }
}

static inline void InitPlayer(const char* relativeFilePath, AudioPlayer* player)
{
    off_t start;
    off_t length;
    int   fd = AFile->OpenFileDescriptor(relativeFilePath, &start, &length);

    // configure audio source
    SLDataLocator_AndroidFD locFD     = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
    SLDataFormat_MIME       formatMME = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource            audioSrc  = {&locFD, &formatMME};

    // configure audio sink
    SLDataLocator_OutputMix locOutMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink              audioSnk  = {&locOutMix, NULL};

    // create audio player
    SLInterfaceID           ids[3]    = {SL_IID_SEEK,     SL_IID_PLAY,     SL_IID_VOLUME};
    SLboolean               req[3]    = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    SLresult                result    = (*engineEngine)->CreateAudioPlayer
                                                         (
                                                             engineEngine,
                                                             &player->object,
                                                             &audioSrc,
                                                             &audioSnk,
                                                             3, ids, req
                                                         );

    CheckError("CreatePlayer CreateAudioPlayer", relativeFilePath);

    // realize the player
    result = (*player->object)->Realize(player->object, SL_BOOLEAN_FALSE);
    CheckError("CreatePlayer player Realize", relativeFilePath);

    // get the player interface
    result = (*player->object)->GetInterface(player->object, SL_IID_PLAY, &player->play);
    CheckError("CreatePlayer player GetInterface play", relativeFilePath);

    // player callback
    result = (*player->play)->RegisterCallback(player->play, PlayerCallback, player);
    CheckError("CreatePlayer player RegisterCallback", relativeFilePath);

    result = (*player->play)->SetCallbackEventsMask(player->play,  SL_PLAYEVENT_HEADATEND | SL_PLAYEVENT_HEADATNEWPOS);
    CheckError("CreatePlayer player SetCallbackEventsMask", relativeFilePath);

    // get the seek interface
    result = (*player->object)->GetInterface(player->object, SL_IID_SEEK, &player->seek);
    CheckError("CreatePlayer player GetInterface seek", relativeFilePath);

    // disable looping
    result = (*player->seek)->SetLoop(player->seek, SL_BOOLEAN_FALSE, 0, SL_TIME_UNKNOWN);
    CheckError("CreatePlayer SetLoop", relativeFilePath);

    // get the volume interface
    result = (*player->object)->GetInterface(player->object, SL_IID_VOLUME, &player->volume);
    CheckError("CreatePlayer player GetInterface volume", relativeFilePath);

    player->waitCallbackCount = 0;
    player->filePath          = AArrayStrSet->Get(filePathSet, relativeFilePath);
}


static void SetLoop(AudioPlayer* player, bool isLoop)
{
    SLboolean     isLoopEnabled;
    SLmillisecond startPos;
    SLmillisecond endPos;
    (*player->seek)->GetLoop(player->seek, &isLoopEnabled, &startPos, &endPos);

    if (isLoopEnabled == (SLboolean) isLoop)
    {
        return;
    }

    SLresult result = (*player->seek)->SetLoop(player->seek, (SLboolean) isLoop, 0, GetDuration(player));
    CheckError("SetLoop", player->filePath);

    if (isLoop)
    {
        AArrayList_Add(loopList, player);
    }
    else
    {
        for (int i = 0; i < loopList->size; ++i)
        {
            if (player == AArrayList_Get(loopList, i, AudioPlayer*))
            {
                AArrayList->RemoveByLast(loopList, i);
                break;
            }
        }
    }
}


static void SetVolume(AudioPlayer* player, float volume)
{
    ALog_A
    (
        volume >= 0.0f && volume <= 1.0f,
        "AAudio SetVolume volume %f not in [0.0, 1.0], file path = %s",
        volume,
        player->filePath
    );

    SLresult result = (*player->volume)->SetVolumeLevel(player->volume, (SLmillibel) ((1.0f - volume) * -5000.0f));
    CheckError("SetVolume", player->filePath);
}


static void Play(AudioPlayer* player)
{
    // set the player's state
    SLresult result = (*player->play)->SetPlayState(player->play, SL_PLAYSTATE_PLAYING);
    CheckError("Play", player->filePath);
    AArrayList_Add(testErrorList, player);
}


static void Pause(AudioPlayer* player)
{
    // set the player's state
    SLresult result = (*player->play)->SetPlayState(player->play, SL_PLAYSTATE_PAUSED);
    CheckError("Pause", player->filePath);
}


static void Stop(AudioPlayer* player)
{
    // set the player's state
    SLresult result = (*player->play)->SetPlayState(player->play, SL_PLAYSTATE_STOPPED);
    CheckError("Stop", player->filePath);
    AArrayList_Add(destroyList, player);
    player->waitCallbackCount = AudioPlayer_WaitOver;
}


static bool IsPlaying(AudioPlayer* player)
{
    SLuint32 state;
    (*player->play)->GetPlayState(player->play, &state);

    if (state == SL_PLAYSTATE_PLAYING)
    {
        return true;
    }
    else
    {
        return false;
    }
}


static AudioPlayer* GetPlayer(const char* relativeFilePath)
{
    AudioPlayer* player = AArrayList_Pop(cacheList, AudioPlayer*);

    if (player == NULL)
    {
        player = malloc(sizeof(AudioPlayer));
    }

    InitPlayer(relativeFilePath, player);

    return player;
}


static void Release()
{
    (*engineObject)   ->Destroy(engineObject);
    (*outputMixObject)->Destroy(outputMixObject);

    engineObject    = NULL;
    engineEngine    = NULL;
    outputMixObject = NULL;

    AArrayList  ->Release(cacheList);
    AArrayList  ->Release(destroyList);
    AArrayList  ->Release(loopList);
    AArrayList  ->Release(testErrorList);
    AArrayStrSet->Release(filePathSet);
}

struct AAudio AAudio[1] =
{
    Init,
    Release,
    Update,
    SetLoopPause,
    SetLoopResume,
    GetPlayer,

    SetVolume,
    SetLoop,

    Play,
    Pause,
    Stop,
    IsPlaying,
};


#endif
