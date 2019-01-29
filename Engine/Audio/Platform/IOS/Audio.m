/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2017-5-4
 * Update: 2019-1-7
 * Author: scott.cgi
 */


#include "Engine/Toolkit/Platform/Platform.h"


#ifdef IS_PLATFORM_IOS


#include <stddef.h>
#include <Foundation/Foundation.h>
#include <AudioToolbox/AudioToolbox.h>
#include <OpenAL/OpenAL.h>

#include "Engine/Audio/Platform/Audio.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/ArrayStrMap.h"


static ArrayStrMap(filePath, void*)  fileDataMap [1] = AArrayStrMap_Init(void*,        20);
static ArrayList  (AudioPlayer*)    cacheList  [1] = AArrayList_Init  (AudioPlayer*, 20);
static ArrayList  (AudioPlayer*)    destroyList[1] = AArrayList_Init  (AudioPlayer*, 20);
static ArrayList  (AudioPlayer*)    loopList   [1] = AArrayList_Init  (AudioPlayer*, 5 );


//----------------------------------------------------------------------------------------------------------------------


static inline void* GetAudioData(char* filePath, ALsizei* outDataSize, ALenum* outDataFormat, ALsizei* outSampleRate)
{
    AudioStreamBasicDescription  fileFormat;
    AudioStreamBasicDescription  outputFormat;
    SInt64                       fileLengthInFrames = 0;
    UInt32                       propertySize       = sizeof(fileFormat);
    ExtAudioFileRef              audioFileRef       = NULL;
    void*                        data               = NULL;

    NSString*                    path               = [[NSBundle mainBundle] pathForResource:
                                                      [NSString stringWithUTF8String:filePath] ofType:nil];
    CFURLRef                     fileUrl            = CFURLCreateWithString(kCFAllocatorDefault, (CFStringRef) path, NULL);
    OSStatus                     error              = ExtAudioFileOpenURL(fileUrl, &audioFileRef);
    
    CFRelease(fileUrl);
    
    if (error != noErr)
    {
        ALog_E("Audio GetAudioData ExtAudioFileOpenURL failed, error = %x, filePath = %s", (int) error, filePath);
        goto Exit;
    }
    
    // get the audio data format
    error = ExtAudioFileGetProperty(audioFileRef, kExtAudioFileProperty_FileDataFormat, &propertySize, &fileFormat);
    
    if (error != noErr)
    {
        ALog_E
        (
            "Audio GetAudioData ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) failed,"
            "error = %x, filePath = %s",
            (int) error,
             filePath
        );

        goto Exit;
    }
    
    if (fileFormat.mChannelsPerFrame > 2)
    {
        ALog_E
        (
            "Audio GetAudioData unsupported format, channel count = %u is greater than stereo, filePath = %s",
            (unsigned int) fileFormat.mChannelsPerFrame,
             filePath
        );

        goto Exit;
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
    outputFormat.mFormatFlags      = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
    
    // set the desired client (output) data format
    error = ExtAudioFileSetProperty(audioFileRef, kExtAudioFileProperty_ClientDataFormat, sizeof(outputFormat), &outputFormat);
    
    if(error != noErr)
    {
        ALog_E
        (
            "Audio GetAudioData ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) failed,"
            "error = %x, filePath = %s",
            (int) error,
             filePath
        );

        goto Exit;
    }
    
    // get the total frame count
    propertySize = sizeof(fileLengthInFrames);
    error        = ExtAudioFileGetProperty(audioFileRef, kExtAudioFileProperty_FileLengthFrames, &propertySize, &fileLengthInFrames);
    
    if(error != noErr)
    {
        ALog_E
        (
            "Audio GetAudioData ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) failed,"
            "error = %x, filePath = %s",
            (int) error,
             filePath
        );

        goto Exit;
    }
    
//----------------------------------------------------------------------------------------------------------------------
    
    // read all the data into memory
    UInt32 framesToRead = (UInt32) fileLengthInFrames;
    UInt32 dataSize     = framesToRead * outputFormat.mBytesPerFrame;
    
    *outDataSize        = (ALsizei) dataSize;
    *outDataFormat      =  outputFormat.mChannelsPerFrame > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
    *outSampleRate      = (ALsizei) outputFormat.mSampleRate;

    int index           = AArrayStrMap->GetIndex(fileDataMap, filePath);
    
    if (index < 0)
    {
        data = malloc(dataSize);
        
        if (data != NULL)
        {
            AudioBufferList    dataBuffer;
            dataBuffer.mNumberBuffers              = 1;
            dataBuffer.mBuffers[0].mDataByteSize   = dataSize;
            dataBuffer.mBuffers[0].mNumberChannels = outputFormat.mChannelsPerFrame;
            dataBuffer.mBuffers[0].mData           = data;
            
            // read the data into an AudioBufferList
            error = ExtAudioFileRead(audioFileRef, &framesToRead, &dataBuffer);
            
            if(error != noErr)
            {
                free(data);
                data = NULL; // make sure to return NULL
                ALog_E("Audio GetAudioData ExtAudioFileRead failed, error = %x, filePath = %s", (int) error, filePath);
                goto Exit;
            }
        }
        
        AArrayStrMap_InsertAt(fileDataMap, filePath, -index - 1, data);
    }
    else
    {
        data = AArrayStrMap_GetAt(fileDataMap, index, void*);
    }


    Exit:
    
    // dispose the ExtAudioFileRef, it is no longer needed
    if (audioFileRef != 0)
    {
        ExtAudioFileDispose(audioFileRef);
    }
    
    return data;
}


//----------------------------------------------------------------------------------------------------------------------


static ALCdevice*                device                 = NULL;
static ALCcontext*               context                = NULL;
static alBufferDataStaticProcPtr alBufferDataStaticProc = NULL;


struct AudioPlayer
{
    ALuint sourceId;
    ALuint bufferId;
};


//----------------------------------------------------------------------------------------------------------------------


static void Update(float deltaSeconds)
{
    for (int i = destroyList->size - 1; i > -1; i--)
    {
        AudioPlayer* player = AArrayList_Get(destroyList, i, AudioPlayer*);
        
        ALint state;
        alGetSourcei(player->sourceId, AL_SOURCE_STATE, &state);
        
        if (state == AL_STOPPED)
        {
            alDeleteSources(1, &player->sourceId);
            alDeleteBuffers(1, &player->bufferId);
            
            AArrayList->Remove(destroyList, i);
            AArrayList_Add(cacheList, player);
        }
    }
}


static void SetLoopPause()
{
    for (int i = 0; i < loopList->size; i++)
    {
        AAudio->SetPause(AArrayList_Get(loopList, i, AudioPlayer*));
    }
}


static void SetLoopResume()
{
    for (int i = 0; i < loopList->size; i++)
    {
        AAudio->SetPlay(AArrayList_Get(loopList, i, AudioPlayer*));
    }
}


static void Init()
{
    // get static buffer data API
    alBufferDataStaticProc = (alBufferDataStaticProcPtr) alcGetProcAddress(NULL, (const ALCchar*) "alBufferDataStatic");
    
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
        ALog_E("Audio Init failed, OpenAL cannot open device");
    }
    
    // clear any errors
    alGetError();
}


static inline void InitPlayer(char* filePath, AudioPlayer* player)
{
    ALenum  error;
    ALsizei size;
    ALenum  format;
    ALsizei freq;
    void*   data = GetAudioData(filePath, &size, &format, &freq);
    
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        ALog_E("Audio InitPlayer failed, error = %x, filePath = %s", error, filePath);
    }
    
    alGenBuffers(1, &player->bufferId);
    if((error = alGetError()) != AL_NO_ERROR)
    {
        ALog_E("Audio InitPlayer generate buffer failed, error = %x, filePath = %s", error, filePath);
    }
    
    // use the static buffer data API
    // the data will not copy in buffer so cannot free data until buffer deleted
    alBufferDataStaticProc(player->bufferId, format, data, size, freq);
    
    if((error = alGetError()) != AL_NO_ERROR)
    {
        ALog_E("Audio InitPlayer attach audio data to buffer failed, error = %x, filePath = %s", error, filePath);
    }
    
//----------------------------------------------------------------------------------------------------------------------
    
    alGenSources(1, &player->sourceId);
    if((error = alGetError())!= AL_NO_ERROR)
    {
        ALog_E("Audio InitPlayer generate source failed, error = %x, filePath = %s", error, filePath);
    }
    
    // turn Looping off
    alSourcei(player->sourceId,                        AL_LOOPING, AL_FALSE);
    
    // set Source Position
    alSourcefv(player->sourceId, AL_POSITION,          (const ALfloat[]) {0.0f, 0.0f, 0.0f});
    
    // set source reference distance
    alSourcef(player->sourceId,  AL_REFERENCE_DISTANCE, 0.0f);
    
    // attach OpenAL buffer to OpenAL Source
    alSourcei(player->sourceId,  AL_BUFFER,             player->bufferId);
    
    if((error = alGetError()) != AL_NO_ERROR)
    {
        ALog_E("Audio InitPlayer attach buffer to source failed, error = %x, filePath = %s", error, filePath);
    }
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
    
    ArrayList* addList;
    ArrayList* removeList;
    
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
    
    for (int i = 0; i < removeList->size; i++)
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
    ALog_A(volume >= 0.0f && volume <= 1.0f, "Audio SetVolume volume %f not in [0.0, 1.0]", volume);
    alSourcef(player->sourceId, AL_GAIN, volume);
    
    ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        ALog_E("Audio SetVolume error = %x", error);
    }
}


static void SetPlay(AudioPlayer* player)
{
    alSourcePlay(player->sourceId);
    
    ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        ALog_E("Audio SetPlay error = %x", error);
    }
}


static void SetPause(AudioPlayer* player)
{
    alSourcePause(player->sourceId);
    
    ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        ALog_E("Audio SetPause error = %x", error);
    }
}


static bool IsPlaying(AudioPlayer* player)
{
    ALint state;
    alGetSourcei(player->sourceId, AL_SOURCE_STATE, &state);
    
    return state == AL_PLAYING;
}


static AudioPlayer* GetPlayer(char* filePath)
{
    AudioPlayer* player = AArrayList_Pop(cacheList, AudioPlayer*);
    
    if (player == NULL)
    {
        player = malloc(sizeof(AudioPlayer));
    }
    
    InitPlayer(filePath, player);
    
    AArrayList_Add(destroyList, player);
    
    return player;
}


static void Release()
{
    // release context
    alcDestroyContext(context);
    
    // close device
    alcCloseDevice(device);
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

    SetPlay,
    SetPause,
    IsPlaying,
};


#endif
