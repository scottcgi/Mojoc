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

#include <pthread.h>
#include "Engine/Toolkit/Utils/Thread.h"


static int StartThread(ThreadRun Run, void* param)
{
    pthread_t      thread[1];
    pthread_attr_t attr  [1];
    pthread_attr_init            (attr);
    pthread_attr_setdetachstate  (attr,   PTHREAD_CREATE_DETACHED);
    int resultId = pthread_create(thread, attr, Run, param);
    pthread_attr_destroy         (attr);
    
    return resultId;
}


struct AThread AThread[1] =
{
    StartThread,
};