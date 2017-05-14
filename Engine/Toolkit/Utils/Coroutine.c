/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2016/11/13
 * Author: scott.cgi
 */

#include <stdlib.h>
#include "Engine/Toolkit/Head/MacroDefine.h"
#include "Engine/Toolkit/Utils/Coroutine.h"
#include "Engine/Toolkit/Platform/Log.h"


static ArrayList(Coroutine*) coroutineRunningList[1] = AArrayListInit(Coroutine*, 25);
static ArrayList(Coroutine*) coroutineCacheList  [1] = AArrayListInit(Coroutine*, 25);


static Coroutine* StartCoroutine(CoroutineRun Run)
{
    Coroutine* coroutine = AArrayListPop(coroutineCacheList, Coroutine*);

    if (coroutine == NULL)
    {
        coroutine = (Coroutine*) malloc(sizeof(Coroutine));

        AArrayList->Init(sizeof(void*),      coroutine->params);
        coroutine->params->increase = 4;

        AArrayList->Init(sizeof(Coroutine*), coroutine->waits);
        coroutine->waits->increase  = 4;
    }
    else
    {
        AArrayList->Clear(coroutine->params);
        AArrayList->Clear(coroutine->waits);
    }

    coroutine->Run          = Run;
    coroutine->step         = NULL;
    coroutine->waitValue    = 0.0f;
    coroutine->curWaitValue = 0.0f;
    coroutine->waitType     = coroutine_wait_null;
    coroutine->state        = coroutine_state_ready;

    AArrayListAdd(coroutineRunningList, coroutine);

    return coroutine;
}


static void Update(float deltaSeconds)
{
    for (int i = coroutineRunningList->size - 1; i > -1; i--)
    {
        Coroutine* coroutine = AArrayListGet(coroutineRunningList, i, Coroutine*);

        if (coroutine->waitType == coroutine_wait_coroutine)
        {
            continue;
        }
        else if (coroutine->curWaitValue >= coroutine->waitValue)
        {
            coroutine->Run(coroutine);

            if (coroutine->state == coroutine_state_finish)
            {
                AArrayList->RemoveByLast(coroutineRunningList, i);

                // add to cache
                AArrayListAdd(coroutineCacheList, coroutine);

                // set waiting coroutines execute forward
                for (int j = 0; j < coroutine->waits->size; j++)
                {
                    Coroutine* wait = AArrayListGet(coroutine->waits, j, Coroutine*);

                    ALog_A
                    (
                        wait->state != coroutine_state_finish,
                        "Coroutine [%p] can not finish before wait coroutine [%p] finish",
                        wait, coroutine
                    );

                    wait->waitType = coroutine_wait_null;
                }

                continue;
            }
        }
        else
        {
            switch (coroutine->waitType)
            {
                case coroutine_wait_frame:
                    coroutine->curWaitValue += 1.0f;
                    break;

                case coroutine_wait_second:
                    coroutine->curWaitValue += deltaSeconds;
                    break;
            }
        }
    }
}


struct ACoroutine ACoroutine[1] =
{
    StartCoroutine,
    Update,
};
