/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License.
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub.
 * This Mojoc game engine is a serious project with extreme code pursuit, and will continue to iterate.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2017-2-16
 * Update : 2019-1-7
 * Author : scott.cgi
 */


#ifndef AUDIO_H
#define AUDIO_H


#include <stdbool.h>


/**
 * Play one audio file.
 */
typedef struct AudioPlayer AudioPlayer;

/**
 * Manage and control all AudioPlayers.
 */
struct  AAudio
{
    /**
     * Init audio engine.
     */
    void         (*Init)         (void);

    /**
     * Destroy audio engine.
     */
    void         (*Release)      (void);

    /**
     * Every frame update by loop.
     */
    void         (*Update)       (float deltaSeconds);

    /**
     * Pause all looping AudioPlayer.
     */
    void         (*SetLoopPause) (void);

    /**
     * Resume all looping AudioPlayer.
     */
    void         (*SetLoopResume)(void);

    /**
     * AudioPlayer will cache by relativeFilePath.
     *
     * relativeFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    AudioPlayer* (*GetPlayer)    (const char* relativeFilePath);

    /**
     * The volume range in [0.0, 1.0].
     */
    void         (*SetVolume)    (AudioPlayer* player, float volume);
    void         (*SetLoop)      (AudioPlayer* player, bool  isLoop);

    void         (*Play)         (AudioPlayer* player);
    void         (*Pause)        (AudioPlayer* player);
    void         (*Stop)         (AudioPlayer* player);
    bool         (*IsPlaying)    (AudioPlayer* player);
};


extern struct AAudio AAudio[1];


#endif
