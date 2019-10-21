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
 * Since    : 2017-5-4
 * Update   : 2019-1-7
 * Author   : scott.cgi
 */


#include "Engine/Toolkit/Platform/Platform.h"


//--------------------
#ifdef IS_PLATFORM_IOS
//--------------------


#include <stddef.h>
#include <Foundation/Foundation.h>
#include <AudioToolbox/AudioToolbox.h>
#include <OpenAL/OpenAL.h>

#include "Engine/Audio/Platform/Audio.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/ArrayStrMap.h"
#include "Engine/Toolkit/Utils/ArrayStrSet.h"


typedef struct
{
    ALsizei size;
    ALenum  format;
    ALsizei sampleRate;
    void*   data;
}
AudioData;


static ArrayStrMap(filePath, AudioData*) audioDataMap[1] = AArrayStrMap_Init(AudioData*,  20);
static ArrayList  (AudioPlayer*)         cacheList  [1] = AArrayList_Init  (AudioPlayer*, 20);
static ArrayList  (AudioPlayer*)         destroyList[1] = AArrayList_Init  (AudioPlayer*, 20);
static ArrayList  (AudioPlayer*)         loopList   [1] = AArrayList_Init  (AudioPlayer*, 5 );
static ArrayStrSet(filePath)             filePathSet[1] = ArrayStrSet_Init (filePath,     20);


#define CheckAudioDataError(tag)               \
    ALog_A                                     \
    (                                          \
        error == noErr,                        \
        "AAudio GetAudioData " tag " failed, " \
        "OSStatus error = %x, file path = %s", \
        (int) error,                           \
        relativeFilePath                       \
    )

#define CheckAudioError(tag, filePath)        \
    ALog_A                                    \
    (                                         \
        (error = alGetError()) == AL_NO_ERROR,\
        "AAudio " tag " failed, "             \
        "alGetError = %x, file path = %s",    \
        error,                                \
        filePath                              \
    )


/**
 * The return data will cached in AudioDataMap.
 */
static inline AudioData* GetAudioData(const char* relativeFilePath)
{
    AudioData* audioData = NULL;
    int        index     = AArrayStrMap->GetIndex(audioDataMap, relativeFilePath);

    if (index < 0)
    {
        AudioStreamBasicDescription fileFormat;
        AudioStreamBasicDescription outputFormat;

        SInt64          fileLengthInFrames = 0;
        UInt32          propertySize       = sizeof(fileFormat);
        ExtAudioFileRef audioFileRef       = NULL;
        NSString*       path               = [[NSBundle mainBundle] pathForResource:
                                             [NSString stringWithUTF8String:relativeFilePath] ofType:nil];

        CFURLRef        fileUrl            = CFURLCreateWithString(kCFAllocatorDefault, (CFStringRef) path, NULL);
        OSStatus        error              = ExtAudioFileOpenURL(fileUrl, &audioFileRef);
        CheckAudioDataError("ExtAudioFileOpenURL");

        CFRelease(fileUrl);

        // get the audio data format
        error = ExtAudioFileGetProperty(audioFileRef, kExtAudioFileProperty_FileDataFormat, &propertySize, &fileFormat);
        CheckAudioDataError("ExtAudioFileGetProperty kExtAudioFileProperty_FileDataFormat");

        if (fileFormat.mChannelsPerFrame > 2)
        {
            ALog_E
            (
                "AAudio GetAudioData unsupported format,"
                "channel count = %u is greater than stereo, relativeFilePath = %s",
                (unsigned int) fileFormat.mChannelsPerFrame,
                relativeFilePath
            );
        }

        // set the client format to 16 bit signed integer (native-endian) data
        // maintain the channel count and sample rate of the original source format
        outputFormat.mSampleRate       = fileFormat.mSampleRate;
        outputFormat.mChannelsPerFrame = fileFormat.mChannelsPerFrame;
        outputFormat.mFormatID         = kAudioFormatLinearPCM;
        outputFormat.mBytesPerPacket   = outputFormat.mChannelsPerFrame * 2;
        outputFormat.mFramesPerPacket  = 1;
        outputFormat.mBytesPerFrame    = outputFormat.mChannelsPerFrame * 2;
        outputFormat.mBitsPerChannel   = 16;
        outputFormat.mFormatFlags      = kAudioFormatFlagsNativeEndian  |
                                         kAudioFormatFlagIsPacked       |
                                         kAudioFormatFlagIsSignedInteger;

        // set the desired client (output) data format
        error = ExtAudioFileSetProperty
                (
                    audioFileRef,
                    kExtAudioFileProperty_ClientDataFormat,
                    sizeof(outputFormat),
                    &outputFormat
                );
        CheckAudioDataError("ExtAudioFileSetProperty kExtAudioFileProperty_ClientDataFormat");

        // get the total frame count
        propertySize = sizeof(fileLengthInFrames);
        error        = ExtAudioFileGetProperty
                       (
                           audioFileRef,
                           kExtAudioFileProperty_FileLengthFrames,
                           &propertySize,
                           &fileLengthInFrames
                       );
        CheckAudioDataError("ExtAudioFileGetProperty kExtAudioFileProperty_FileLengthFrames");

        // read all the data into memory
        UInt32 framesToRead = (UInt32) fileLengthInFrames;
        UInt32 dataSize     = framesToRead * outputFormat.mBytesPerFrame;

//----------------------------------------------------------------------------------------------------------------------

        audioData             = malloc(sizeof(AudioData) + dataSize);
        audioData->size       = (ALsizei) dataSize;
        audioData->format     = outputFormat.mChannelsPerFrame > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
        audioData->sampleRate = (ALsizei) outputFormat.mSampleRate;
        audioData->data       = (char*) audioData + sizeof(AudioData);

        AudioBufferList dataBuffer;
        dataBuffer.mNumberBuffers              = 1;
        dataBuffer.mBuffers[0].mDataByteSize   = dataSize;
        dataBuffer.mBuffers[0].mNumberChannels = outputFormat.mChannelsPerFrame;
        dataBuffer.mBuffers[0].mData           = audioData->data;

        // read the data into an AudioBufferList
        error = ExtAudioFileRead(audioFileRef, &framesToRead, &dataBuffer);
        CheckAudioDataError("ExtAudioFileRead");

        // dispose the ExtAudioFileRef, it is no longer needed
        ExtAudioFileDispose(audioFileRef);

        AArrayStrMap_InsertAt(audioDataMap, relativeFilePath, -index - 1, audioData);
    }
    else
    {
        audioData = AArrayStrMap_GetAt(audioDataMap, index, AudioData*);
    }

    return audioData;
}


#undef CheckAudioDataError


//----------------------------------------------------------------------------------------------------------------------


static ALCdevice*                device                 = NULL;
static ALCcontext*               context                = NULL;
static alBufferDataStaticProcPtr alBufferDataStaticProc = NULL;


/**
 * Defined in Audio.h
 */
struct AudioPlayer
{
    ALuint      sourceId;
    ALuint      bufferId;
    const char* filePath;
};


static void Update(float deltaSeconds)
{
    for (int i = destroyList->size - 1; i > -1; --i)
    {
        AudioPlayer* player = AArrayList_Get(destroyList, i, AudioPlayer*);
        
        ALint state;
        alGetSourcei(player->sourceId, AL_SOURCE_STATE, &state);
        
        if (state == AL_STOPPED)
        {
            alDeleteSources(1, &player->sourceId);
            alDeleteBuffers(1, &player->bufferId);

            player->sourceId = 0;
            player->bufferId = 0;
            player->filePath = NULL;

            AArrayList->Remove(destroyList, i);
            AArrayList_Add(cacheList, player);
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
    // get static buffer data API
    alBufferDataStaticProc = alcGetProcAddress(NULL, "alBufferDataStatic");
    
    // create a new OpenAL Device
    // pass NULL to specify the system’s default output device
    device = alcOpenDevice(NULL);
    
    if (device != NULL)
    {
        // create a new OpenAL Context
        // the new context will render to the OpenAL Device just created
        context = alcCreateContext(device, 0);
        
        if (context != NULL)
        {
            // make the new context the Current OpenAL Context
            alcMakeContextCurrent(context);
        }
    }
    else
    {
        ALog_E("AAudio Init failed, OpenAL cannot open device");
    }
    
    // clear any errors
    ALenum error;
    CheckAudioError("Init", "");
}


static inline void InitPlayer(const char* relativeFilePath, AudioPlayer* player)
{
    ALenum     error;
    AudioData* audioData = GetAudioData(relativeFilePath);
    
    alGenBuffers(1, &player->bufferId);
    CheckAudioError("InitPlayer generate buffer", relativeFilePath);
    
    // use the static buffer data API
    // the data will not copy in buffer so cannot free data until buffer deleted
    alBufferDataStaticProc(player->bufferId, audioData->format, audioData->data, audioData->size, audioData->sampleRate);
    CheckAudioError("InitPlayer attach audio data to buffer", relativeFilePath);

    alGenSources(1, &player->sourceId);
    CheckAudioError("InitPlayer generate source", relativeFilePath);

    // turn Looping off
    alSourcei(player->sourceId,  AL_LOOPING, AL_FALSE);
    
    // set Source Position
    alSourcefv(player->sourceId, AL_POSITION, (const ALfloat[3]) {0.0f, 0.0f, 0.0f});
    
    // set source reference distance
    alSourcef(player->sourceId,  AL_REFERENCE_DISTANCE, 0.0f);
    
    // attach OpenAL buffer to OpenAL Source
    alSourcei(player->sourceId,  AL_BUFFER, player->bufferId);
    CheckAudioError("InitPlayer attach buffer to source", relativeFilePath);

    // set player name
    player->filePath = AArrayStrSet->Get(filePathSet, relativeFilePath);
}


static void SetLoop(AudioPlayer* player, bool isLoop)
{
    ALint isLoopEnabled;
    alGetSourcei(player->sourceId, AL_LOOPING, &isLoopEnabled);
    
    if (isLoopEnabled == isLoop)
    {
        return;
    }
    
    alSourcei(player->sourceId, AL_LOOPING, (ALint) isLoop);

    // move player from removeList to addList
    
    ArrayList(AudioPlayer*)* addList;
    ArrayList(AudioPlayer*)* removeList;

    if (isLoop)
    {
        addList    = loopList;
        removeList = destroyList;
    }
    else
    {
        addList    = destroyList;
        removeList = loopList;
    }

    for (int i = 0; i < removeList->size; ++i)
    {
        if (player == AArrayList_Get(removeList, i, AudioPlayer*))
        {
            AArrayList->RemoveByLast(removeList, i);
            break;
        }
    }

    AArrayList_Add(addList, player);
}


static void SetVolume(AudioPlayer* player, float volume)
{
    ALog_A
    (
        volume >= 0.0f && volume <= 1.0f,
        "AAudio SetVolume volume %f not in [0.0, 1.0], audio file path = %s",
        volume,
        player->filePath
    );

    alSourcef(player->sourceId, AL_GAIN, volume);
    
    ALenum error;
    CheckAudioError("SetVolume", player->filePath);
}


static void Play(AudioPlayer* player)
{
    alSourcePlay(player->sourceId);
    ALenum error;
    CheckAudioError("Play", player->filePath);
}


static void Pause(AudioPlayer* player)
{
    alSourcePause(player->sourceId);
    ALenum error;
    CheckAudioError("Pause", player->filePath);
}


static void Stop(AudioPlayer* player)
{
    // after alSourceStop call alDeleteSources will cause OpenAL leak OALSource:AddPlaybackMessage
    alSourceStop(player->sourceId);
    ALenum error;
    CheckAudioError("Stop", player->filePath);

    // put player into destroyList
    SetLoop(player, false);
}

static bool IsPlaying(AudioPlayer* player)
{
    ALint state;
    alGetSourcei(player->sourceId, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}


static AudioPlayer* GetPlayer(const char* relativeFilePath)
{
    AudioPlayer* player = AArrayList_Pop(cacheList, AudioPlayer*);
    
    if (player == NULL)
    {
        player = malloc(sizeof(AudioPlayer));
    }
    
    InitPlayer(relativeFilePath, player);
    AArrayList_Add(destroyList, player);
    
    return player;
}


#undef CheckAudioError


static void Release()
{
    // release context
    alcDestroyContext(context);
    // close device
    alcCloseDevice(device);

    AArrayStrMap->Release(audioDataMap);
    AArrayList  ->Release(cacheList);
    AArrayList  ->Release(destroyList);
    AArrayList  ->Release(loopList);
    AArrayStrSet->Release(filePathSet);
}


struct AAudio AAudio[1] =
{{
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
}};


//-----------------------
#endif // IS_PLATFORM_IOS
//-----------------------
