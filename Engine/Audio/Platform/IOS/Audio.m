/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2017-5-4
 * Author: scott.cgi
 */

#include "Engine/Toolkit/Platform/Platform.h"


//--------------------------------------------------------------------------------------------------
#ifdef is_platform_ios
//--------------------------------------------------------------------------------------------------


#include <stddef.h>
#include <Foundation/Foundation.h>
#include <AudioToolbox/AudioToolbox.h>
#include <OpenAL/OpenAL.h>

#include "Engine/Audio/Platform/Audio.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/ArrayStrMap.h"


static ArrayStrMap(filePath, void*) fileDataMap[1] = AArrayStrMapInit(sizeof(void*),        20);
static ArrayList  (AudioPlayer*)    cacheList  [1] = AArrayListInit  (sizeof(AudioPlayer*), 20);
static ArrayList  (AudioPlayer*)    destroyList[1] = AArrayListInit  (sizeof(AudioPlayer*), 20);
static ArrayList  (AudioPlayer*)    loopList   [1] = AArrayListInit  (sizeof(AudioPlayer*), 5 );


//--------------------------------------------------------------------------------------------------


static inline void* GetAudioData(char* filePath, ALsizei* outDataSize, ALenum* outDataFormat, ALsizei* outSampleRate)
{
    AudioStreamBasicDescription	fileFormat;
    AudioStreamBasicDescription	outputFormat;
    SInt64						fileLengthInFrames = 0;
    UInt32						propertySize       = sizeof(fileFormat);
    ExtAudioFileRef			    audioFileRef       = NULL;
    void*						data               = NULL;

    NSString*                   path               = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:filePath] ofType:nil];
    CFURLRef                    fileUrl            = CFURLCreateWithString(kCFAllocatorDefault, (CFStringRef) path, NULL);
    OSStatus				    error              = ExtAudioFileOpenURL(fileUrl, &audioFileRef);
    
    CFRelease(fileUrl);
    
    if (error != noErr)
    {
        ALogE("Audio GetAudioData ExtAudioFileOpenURL failed, error = %x, filePath = %s", (int) error, filePath);
        goto label_exit;
    }
    
    // get the audio data format
    error = ExtAudioFileGetProperty(audioFileRef, kExtAudioFileProperty_FileDataFormat, &propertySize, &fileFormat);
    
    if (error != noErr)
    {
        ALogE("Audio GetAudioData ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) failed, error = %x, filePath = %s", (int) error, filePath);
        goto label_exit;
    }
    
    if (fileFormat.mChannelsPerFrame > 2)
    {
        ALogE("Audio GetAudioData unsupported format, channel count = %u is greater than stereo, filePath = %s", fileFormat.mChannelsPerFrame, filePath);
        goto label_exit;
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
        ALogE("Audio GetAudioData ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) failed, error = %x, filePath = %s", (int) error, filePath);
        goto label_exit;
    }
    
    // get the total frame count
    propertySize = sizeof(fileLengthInFrames);
    error        = ExtAudioFileGetProperty(audioFileRef, kExtAudioFileProperty_FileLengthFrames, &propertySize, &fileLengthInFrames);
    
    if(error != noErr)
    {
        ALogE("Audio GetAudioData ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) failed, error = %x, filePath = %s", (int) error, filePath);
        goto label_exit;
    }
    
//--------------------------------------------------------------------------------------------------
    
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
            AudioBufferList	dataBuffer;
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
                ALogE("Audio GetAudioData ExtAudioFileRead failed, error = %x, filePath = %s", (int) error, filePath);
                goto label_exit;
            }
        }
        
        AArrayStrMapInsertAt(fileDataMap, filePath, -index - 1, data);
    }
    else
    {
        data = AArrayStrMapGetAt(fileDataMap, index, void*);
    }
    
    
    label_exit:
    
    // dispose the ExtAudioFileRef, it is no longer needed
    if (audioFileRef != 0)
    {
        ExtAudioFileDispose(audioFileRef);
    }
    
    return data;
}


//--------------------------------------------------------------------------------------------------


static ALCdevice*                device                 = NULL;
static ALCcontext*               context                = NULL;
static alBufferDataStaticProcPtr alBufferDataStaticProc = NULL;


struct AudioPlayer
{
    ALuint sourceId;
    ALuint bufferId;
};


//--------------------------------------------------------------------------------------------------


static void Update(float deltaSeconds)
{
    for (int i = destroyList->size - 1; i > -1; i--)
    {
        AudioPlayer* player = AArrayListGet(destroyList, i, AudioPlayer*);
        
        ALint state;
        alGetSourcei(player->sourceId, AL_SOURCE_STATE, &state);
        
        if (state == AL_STOPPED)
        {
            alDeleteSources(1, &player->sourceId);
            alDeleteBuffers(1, &player->bufferId);
            
            AArrayList->Remove(destroyList, i);
            AArrayListAdd(cacheList, player);
        }
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
        ALogE("Audio Init failed, OpenAL can not open device");
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
        ALogE("Audio InitPlayer failed, error = %x, filePath = %s", error, filePath);
    }
    
    alGenBuffers(1, &player->bufferId);
    if((error = alGetError()) != AL_NO_ERROR)
    {
        ALogE("Audio InitPlayer generate buffer failed, error = %x, filePath = %s", error, filePath);
    }
    
    // use the static buffer data API
    // the data will not copy in buffer so can not free data until buffer deleted
    alBufferDataStaticProc(player->bufferId, format, data, size, freq);
    
    if((error = alGetError()) != AL_NO_ERROR)
    {
        ALogE("Audio InitPlayer attach audio data to buffer failed, error = %x, filePath = %s", error, filePath);
    }
    
//--------------------------------------------------------------------------------------------------
    
    alGenSources(1, &player->sourceId);
    if((error = alGetError())!= AL_NO_ERROR)
    {
        ALogE("Audio InitPlayer generate source failed, error = %x, filePath = %s", error, filePath);
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
        ALogE("Audio InitPlayer attach buffer to source failed, error = %x, filePath = %s", error, filePath);
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
        if (player == AArrayListGet(removeList, i, AudioPlayer*))
        {
            AArrayList->RemoveByLast(removeList, i);
            break;
        }
    }
    
    AArrayListAdd(addList, player);
}


static void SetVolume(AudioPlayer* player, int volume)
{
    ALogA(volume >= 0 && volume <= 100, "Audio SetVolume volume %d not in [0, 100]", volume);
    alSourcef(player->sourceId, AL_GAIN, volume / 100.0f);
    
    ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        ALogE("Audio SetVolume error = %x", error);
    }
}


static void SetPlay(AudioPlayer* player)
{
    alSourcePlay(player->sourceId);
    
    ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        ALogE("Audio SetPlay error = %x", error);
    }
}


static void SetPause(AudioPlayer* player)
{
    alSourcePause(player->sourceId);
    
    ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        ALogE("Audio SetPause error = %x", error);
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
    AudioPlayer* player = AArrayListPop(cacheList, AudioPlayer*);
    
    if (player == NULL)
    {
        player = (AudioPlayer*) malloc(sizeof(AudioPlayer));
    }
    
    InitPlayer(filePath, player);
    
    AArrayListAdd(destroyList, player);
    
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
