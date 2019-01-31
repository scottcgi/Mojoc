/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2019-1-30
 * Update: 2019-1-30
 * Author: scott.cgi
 */

#ifndef THREAD_H
#define  THREAD_H


/**
 * Thread running function.
 */
typedef void* (*ThreadRun)(void* param);

/**
 * Manage and control thread.
 */
struct AThread
{
    /**
     * Start a thread with PTHREAD_CREATE_DETACHED.
     * the param will passed to ThreadRun.
     *
     * return 0 success start a thread, else return error id.
     */
    int (*StartThread)(ThreadRun Run, void* param);
};


extern struct AThread AThread[1];


#endif
