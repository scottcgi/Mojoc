/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2014-1-16
 * Author: scott.cgi
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <stdbool.h>
#include "Engine/Toolkit/Head/Define.h"
#include "Engine/Toolkit/Head/UserData.h"


typedef struct Scheduler Scheduler;
struct  Scheduler
{
    UserData  userData[1];

    /**
     * Schedule interval time for update method called
     */
    float     intervalTime;

    /**
     * Whether cancel scheduler default false
     * once isCancel set true scheduler will remove and free
     */
    bool      isCancel;

    /**
     * Current schedule time
     */
    float     currentTime;

    /**
     * Called per frame
     */
    void (*Update)(Scheduler* scheduler, float deltaSeconds);
};


typedef void (*SchedulerUpdate)(Scheduler* scheduler, float deltaSeconds);


struct AScheduler
{
    /**
     * Scheduler to be scheduled by intervalTime
     */
    Scheduler* (*Schedule)    (SchedulerUpdate Update, float intervalTime);

    /**
     * Only schedule once by intervalTime
     */
    Scheduler* (*ScheduleOnce)(SchedulerUpdate Update, float intervalTime);

    /**
     * Update per frame calculate schedule time
     */
    void       (*Update)      (float deltaSeconds);
};


extern struct AScheduler AScheduler[1];


#endif
