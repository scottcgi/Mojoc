/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2017-2-16
 * Update: 2019-1-7
 * Author: scott.cgi
 */


#ifndef AUDIO_H
#define  AUDIO_H


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
    void         (*Init)         ();

    /**
     * Destroy audio engine.
     */
    void         (*Release)      ();

    /**
     * Every frame update by loop.
     */
    void         (*Update)       (float deltaSeconds);

    /**
     * Pause all looping AudioPlayer.
     */
    void         (*SetLoopPause) ();

    /**
     * Resume all looping AudioPlayer.
     */
    void         (*SetLoopResume)();

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
    void         (*SetLoop)      (AudioPlayer* player, bool isLoop);

    void         (*SetPlay)      (AudioPlayer* player);
    void         (*SetPause)     (AudioPlayer* player);
    bool         (*IsPlaying)    (AudioPlayer* player);
};


extern struct AAudio AAudio[1];


#endif
