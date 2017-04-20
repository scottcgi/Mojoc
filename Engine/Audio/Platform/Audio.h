/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2017-2-16
 * Author : scott.cgi
 */

#ifndef audio_h
#define audio_h

#include <stdbool.h>


typedef struct AudioPlayer AudioPlayer;


struct AAudio
{
    /**
     * Init audio engine
     */
    void         (*Init)            ();

    /**
     * Every frame update
     */
    void         (*Update)          (float deltaTime);

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
     * The volume range in [0, 100]
     */
    void         (*SetVolume)       (AudioPlayer* player, int  volume);
    void         (*SetLoop)         (AudioPlayer* player, bool isLoop);

    void         (*SetPlay)         (AudioPlayer* player);
    void         (*SetPause)        (AudioPlayer* player);
    bool         (*IsPlaying)       (AudioPlayer* player);
};


extern struct AAudio AAudio[1];


#endif
