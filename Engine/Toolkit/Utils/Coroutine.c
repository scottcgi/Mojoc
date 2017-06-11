/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2016/11/13
 * Author : scott.cgi
 * Version: 0.1
 */


#include <stdlib.h>
#include "Engine/Toolkit/Head/MacroDefine.h"
#include "Engine/Toolkit/Utils/Coroutine.h"
#include "Engine/Toolkit/Platform/Log.h"


static ArrayList(Coroutine*) coroutineRunningList[1] = AArrayList_Init(Coroutine*, 25);
static ArrayList(Coroutine*) coroutineCacheList  [1] = AArrayList_Init(Coroutine*, 25);


static Coroutine* StartCoroutine(CoroutineRun Run)
{
    Coroutine* coroutine = AArrayList_Pop(coroutineCacheList, Coroutine*);

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
    coroutine->waitType     = CoroutineWaitType_Null;
    coroutine->state        = CoroutineState_Ready;

    AArrayList_Add(coroutineRunningList, coroutine);

    return coroutine;
}


static void Update(float deltaSeconds)
{
    for (int i = coroutineRunningList->size - 1; i > -1; i--)
    {
        Coroutine* coroutine = AArrayList_Get(coroutineRunningList, i, Coroutine*);

        if (coroutine->waitType == CoroutineWaitType_Coroutines)
        {
            continue;
        }
        else if (coroutine->curWaitValue >= coroutine->waitValue)
        {
            coroutine->Run(coroutine);

            if (coroutine->state == CoroutineState_Finish)
            {
                AArrayList->RemoveByLast(coroutineRunningList, i);

                // add to cache
                AArrayList_Add(coroutineCacheList, coroutine);

                // set waiting coroutines execute forward
                for (int j = 0; j < coroutine->waits->size; j++)
                {
                    Coroutine* wait = AArrayList_Get(coroutine->waits, j, Coroutine*);

                    ALog_A
                    (
                        wait->state != CoroutineState_Finish,
                        "Coroutine [%p] can not finish before wait coroutine [%p] finish",
                        wait, coroutine
                    );

                    wait->waitType = CoroutineWaitType_Null;
                }

                continue;
            }
        }
        else
        {
            switch (coroutine->waitType)
            {
                case CoroutineWaitType_Frames:
                    coroutine->curWaitValue += 1.0f;
                    break;

                case CoroutineWaitType_Seconds:
                    coroutine->curWaitValue += deltaSeconds;
                    break;
                    
                case CoroutineWaitType_Null:
                    break;
                    
                case CoroutineWaitType_Coroutines:
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
