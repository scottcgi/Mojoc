//
// Created by scottcgi on 2016/11/13.
//

#include <malloc.h>

#include "Engine/Toolkit/Head/Def.h"
#include "Engine/Toolkit/Utils/Coroutine.h"
#include "Engine/Toolkit/Utils/ArrayIntMap.h"
#include "Engine/Toolkit/Platform/Log.h"


static ArrayIntMap(coroutinePtr, Coroutine*) coroutineMap [1] = AArrayIntMapInit(Coroutine*, 25);
static ArrayList  (Coroutine*)               coroutineList[1] = AArrayListInit  (Coroutine*, 25);


static Coroutine* StartCoroutine(CoroutineRun Run)
{
    Coroutine* coroutine = AArrayListPop(coroutineList, Coroutine*);

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
    coroutine->step         = 0;
    coroutine->waitValue    = 0.0f;
    coroutine->curWaitValue = 0.0f;
    coroutine->waitType     = coroutine_wait_null;
    coroutine->state        = coroutine_state_ready;

    AArrayIntMapPut(coroutineMap, coroutine, coroutine);

    return coroutine;
}

static void Update(float deltaTime)
{
    for (int i = coroutineMap->arrayList->size - 1; i > -1; i--)
    {
        Coroutine* coroutine = AArrayIntMapGetAt(coroutineMap, i, Coroutine*);

        if (coroutine->waitType == coroutine_wait_coroutine)
        {
            continue;
        }
        else if (coroutine->curWaitValue >= coroutine->waitValue)
        {
            coroutine->Run(coroutine);

            if (coroutine->state == coroutine_state_finish)
            {
                AArrayIntMap->RemoveAt(coroutineMap, i);

                // add to cache
                AArrayListAdd(coroutineList, coroutine);

                // set waiting coroutines execute forward
                for (int j = 0; j < coroutine->waits->size; j++)
                {
                    Coroutine* wait = AArrayListGet(coroutine->waits, j, Coroutine*);

                    ALogA
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
                    coroutine->curWaitValue += deltaTime;
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
