/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2014-1-16
 * Author: scott.cgi
 */

#include <stdlib.h>
#include "Engine/Toolkit/Head/MacroDefine.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/ArrayIntMap.h"


static ArrayList(Scheduler*) schedulerRun [1] = AArrayListInit(Scheduler*, 30);
static ArrayList(Scheduler*) schedulerList[1] = AArrayListInit(Scheduler*, 30);


static inline Scheduler* GetScheduler(SchedulerUpdate Update, float intervalTime)
{
    ALogA(Update != NULL, "schedule SchedulerUpdateFunction must not NULL");

    Scheduler* scheduler = AArrayListPop(schedulerList, Scheduler*);

    if (scheduler == NULL)
    {
        scheduler = (Scheduler*) malloc(sizeof(Scheduler));
    }

    AUserDataInit(scheduler->userData);
    scheduler->currentTime  = 0.0f;
    scheduler->intervalTime = intervalTime;
    scheduler->Update       = Update;

	AArrayListAdd(schedulerRun, scheduler);

    return scheduler;
}


static Scheduler* Schedule(SchedulerUpdate Update, float intervalTime)
{
    Scheduler* scheduler = GetScheduler(Update, intervalTime);
    scheduler->isCancel  = false;
    return scheduler;
}


static Scheduler* ScheduleOnce(SchedulerUpdate Update, float intervalTime)
{
    Scheduler* scheduler = GetScheduler(Update, intervalTime);
    scheduler->isCancel  = true;
    return scheduler;
}


static void Update(float deltaSeconds)
{
	for (int i = schedulerRun->size - 1; i > -1; i--)
	{
		Scheduler* scheduler = AArrayListGet(schedulerRun, i, Scheduler*);

		if (scheduler->currentTime < scheduler->intervalTime)
		{
			scheduler->currentTime += deltaSeconds;
		}
		else
		{
			scheduler->Update(scheduler, deltaSeconds);

			if (scheduler->isCancel)
			{
				AArrayList->RemoveByLast(schedulerRun, i);
				AArrayListAdd(schedulerList, scheduler);
			}
			else
			{
				scheduler->currentTime = 0.0f;
			}
		}
	}
}


struct AScheduler AScheduler[1] =
{
	Schedule,
	ScheduleOnce,
	Update,
};
