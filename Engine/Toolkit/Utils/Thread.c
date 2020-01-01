/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2019-1-30
 * Update   : 2019-1-30
 * Author   : scott.cgi
 */


#include <pthread.h>
#include "Engine/Toolkit/Utils/Thread.h"


static int StartThread(ThreadRun Run, void* param)
{
    pthread_t      thread[1];
    pthread_attr_t attr  [1];
    pthread_attr_init            (attr);
    pthread_attr_setdetachstate  (attr,   PTHREAD_CREATE_DETACHED);
    int resultID = pthread_create(thread, attr, Run, param);
    pthread_attr_destroy         (attr);
    
    return resultID;
}


struct AThread AThread[1] =
{{
    StartThread,
}};