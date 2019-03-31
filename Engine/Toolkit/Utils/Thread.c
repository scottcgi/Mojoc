/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The project Mojoc is a serious project with extreme code pursuit,
 * and will continue to iterate.
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