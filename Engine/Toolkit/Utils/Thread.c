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
 * Since  : 2019-1-30
 * Update : 2019-1-30
 * Author : scott.cgi
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