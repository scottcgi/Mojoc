/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2014-1-16
 * Update: 2019-1-25
 * Author: scott.cgi
 */


#ifndef SCHEDULER_H
#define  SCHEDULER_H


#include <stdbool.h>
#include "Engine/Toolkit/HeaderUtils/Define.h"
#include "Engine/Toolkit/HeaderUtils/UserData.h"


typedef struct Scheduler Scheduler;


/**
 * Callback by interval time.
 *
 * scheduler   : caller
 * deltaSeconds: frame delta seconds
 */
typedef void (*SchedulerUpdate)(Scheduler* scheduler, float deltaSeconds);


/**
 * Schedule update function callback by interval time.
 */
struct Scheduler
{
    UserData        userData[1];

    /**
     * Schedule interval time for update method called.
     */
    float            intervalTime;

    /**
     * Whether cancel scheduler, default false.
     * once isCancel set true scheduler will be removed from callback list.
     */
    bool            isCancel;

    /**
     * Current schedule time.
     */
    float            currentTime;

    /**
     * Schedule callback.
     */
    SchedulerUpdate Update;
};


/**
 * Control Scheduler.
 */
struct AScheduler
{
    /**
     * Scheduler to be scheduled by intervalTime.
     */
    Scheduler* (*Schedule)    (SchedulerUpdate Update, float intervalTime);

    /**
     * Only schedule once by intervalTime.
     */
    Scheduler* (*ScheduleOnce)(SchedulerUpdate Update, float intervalTime);

    /**
     * Every frame update by loop.
     */
    void       (*Update)      (float deltaSeconds);
};


extern struct AScheduler AScheduler[1];


#endif
