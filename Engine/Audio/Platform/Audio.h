/*
 * Copyright (c) 2017-2018 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2017-2-16
 * Author: scott.cgi
 */

#ifndef AUDIO_H
#define AUDIO_H


#include <stdbool.h>


typedef struct AudioPlayer AudioPlayer;
struct AAudio
{
    /**
     * Init audio engine
     */
    void         (*Init)            ();

    /**
     * Destroy audio engine
     */
    void         (*Release)         ();

    /**
     * Every frame update
     */
    void         (*Update)          (float deltaSeconds);

    /**
     * Pause all loop AudioPlayer
     */
    void         (*SetLoopPause)    ();

    /**
     * Resume all loop AudioPlayer
     */
    void         (*SetLoopResume)   ();

    /**
     * Cache by filePath
     */
    AudioPlayer* (*GetPlayer)       (char* filePath);

    /**
     * The volume range in [0.0, 1.0]
     */
    void         (*SetVolume)       (AudioPlayer* player, float volume);
    void         (*SetLoop)         (AudioPlayer* player, bool  isLoop);

    void         (*SetPlay)         (AudioPlayer* player);
    void         (*SetPause)        (AudioPlayer* player);
    bool         (*IsPlaying)       (AudioPlayer* player);
};


extern struct AAudio AAudio[1];


#endif
