/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2014-1-16
 * Author : scott.cgi
 */

#ifndef scheduler_h
#define scheduler_h


#include <stdbool.h>
#include "Engine/Toolkit/Head/MacroDefine.h"


typedef struct Scheduler Scheduler;
struct  Scheduler
{
	void* userData;

	/**
	 * Schedule interval time for update method called
	 */
	float intervalTime;

	/**
	 * Whether cancel scheduler default false
	 * once isCancel set true scheduler will remove and free
	 */
	bool  isCancel;

	/**
	 * Current schedule time
	 */
	float currentTime;

	/**
	 * Called per frame
	 */
	void (*Update)(Scheduler* scheduler, float deltaTime);
};


typedef void (*SchedulerUpdate)(Scheduler* scheduler, float deltaTime);


struct AScheduler
{
	/**
	 * Scheduler to be scheduled by intervalTime
	 */
	Scheduler* (*Schedule)    (SchedulerUpdate Update, float intervalTime, void* userData);

	/**
	 * Only schedule once by intervalTime
	 */
	Scheduler* (*ScheduleOnce)(SchedulerUpdate Update, float intervalTime, void* userData);

	/**
	 * Update per frame calculate schedule time
	 */
	void       (*Update)      (float deltaTime);
};


extern struct AScheduler AScheduler[1];


#endif
