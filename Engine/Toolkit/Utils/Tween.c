/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2016-6-8
 * Update: 2019-1-8
 * Author: scott.cgi
 */

 
#include "Engine/Toolkit/Utils/Tween.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/ArrayIntMap.h"
#include "Engine/Toolkit/Utils/ArrayQueue.h"


typedef struct
{
    /**
     * Target's actions queue, run each after over.
     */
    ArrayQueue(TweenAction*) queue  [1];

    /**
     * Target's current running action.
     */
    ArrayList(TweenAction*)  current[1];

    /**
     * Current running action of queue.
     */
    TweenAction*             currentAction;
}
Tween;


static ArrayIntMap(tweenId, Tween*) tweenMap  [1] = AArrayIntMap_Init(Tween*,       25);
static ArrayList  (Tween*)          tweenList [1] = AArrayList_Init  (Tween*,       25);
static ArrayList  (TweenAction*)    actionList[1] = AArrayList_Init  (TweenAction*, 25);


static inline Tween* GetTween()
{
    Tween* tween = AArrayList_Pop(tweenList, Tween*);

    if (tween == NULL)
    {
        tween = (Tween*) malloc(sizeof(Tween));

        AArrayQueue->Init(sizeof(TweenAction*), tween->queue);
        tween->queue->elementList->increase = 6;

        AArrayList->Init(sizeof(TweenAction*),  tween->current);
        tween->current->increase            = 6;
    }
    else
    {
        AArrayQueue->Clear(tween->queue);
        AArrayList ->Clear(tween->current);
    }

    tween->currentAction = NULL;

    return tween;
}


static TweenAction* GetAction()
{
    TweenAction* action = AArrayList_Pop(actionList, TweenAction*);

    if (action == NULL)
    {
        action = (TweenAction*) malloc(sizeof(TweenAction));
        AArrayList->InitWithCapacity(sizeof(TweenActionValue), 6, action->actionValueList);
        action->actionValueList->increase = 6;
    }
    else
    {
        AArrayList->Clear(action->actionValueList);
    }

    AUserData_Init(action->userData);
    action->curTime    = 0.0f;
    action->duration   = 0.0f;
    action->OnComplete = NULL;
    action->isQueue    = true;
    action->target     = NULL;

    return action;
}


static TweenActionValue* AddTweenActionValue(TweenAction* action)
{
    TweenActionValue* actionValue = AArrayList_GetPtrAdd(action->actionValueList, TweenActionValue);
    actionValue->value            = 0.0f;
    actionValue->fromValue        = 0.0f;
    actionValue->toValue          = 0.0f;
    actionValue->OnGet            = NULL;
    actionValue->OnSet            = NULL;
    actionValue->isRelative       = true;
    actionValue->easeType         = TweenEaseType_Linear;

    return actionValue;
}


static inline void SetActionValue(TweenAction* action)
{
    for (int i = 0; i < action->actionValueList->size; ++i)
    {
        TweenActionValue* actionValue = AArrayList_GetPtr(action->actionValueList, i, TweenActionValue);

        ALog_A
        (
            actionValue->OnGet != NULL && actionValue->OnSet != NULL,
            "ATween SetActionValue action OnSet and OnGet must not NULL."
        );

        actionValue->fromValue = actionValue->OnGet(action->target);

        if (actionValue->isRelative)
        {
            actionValue->toValue = actionValue->value + actionValue->fromValue;
        }
        else
        {
            actionValue->toValue = actionValue->value;
        }
    }
}


static void* RunActions(Array(TweenAction*)* actions, void* tweenId)
{
    Tween* tween;

    if (tweenId == NULL)
    {
         // not give tweenId, we use Tween ptr for it
        tween   = GetTween();
        tweenId = tween;

        AArrayIntMap_TryPut(tweenMap, tweenId, tween);
    }
    else
    {
        int index = AArrayIntMap->GetIndex(tweenMap, (intptr_t) tweenId);

        if (index < 0)
        {
            tween = GetTween();
            AArrayIntMap_InsertAt(tweenMap, tweenId, -index - 1, tween);
        }
        else
        {
            tween = AArrayIntMap_GetAt(tweenMap, index, Tween*);
        }
    }

//----------------------------------------------------------------------------------------------------------------------

    for (int i = 0; i < actions->length; ++i)
    {
        TweenAction* action = AArray_Get(actions, i, TweenAction*);

        if (action->isQueue)
        {
            AArrayQueue_Push(tween->queue, action);
        }
        else
        {
            AArrayList_Add(tween->current, action);
            SetActionValue(action);
        }
    }

    return tweenId;
}


static bool TryRemoveAction(void* tweenId, TweenAction* action)
{
    Tween* tween = AArrayIntMap_Get(tweenMap, tweenId, Tween*);

    if (tween != NULL)
    {
        for (int i = 0; i < tween->current->size; ++i)
        {
            TweenAction* tweenAction = AArrayList_Get(tween->current, i, TweenAction*);

            if (action == tweenAction)
            {
                if (action == tween->currentAction)
                {
                    tween->currentAction = NULL;
                }

                AArrayList->RemoveByLast(tween->current, i);
                AArrayList_Add(actionList, action);

                return true;
            }
        }

        for (int i = tween->queue->topIndex; i < tween->queue->elementList->size; ++i)
        {
            TweenAction* tweenAction = AArrayList_Get(tween->queue->elementList, i, TweenAction*);

            if (action == tweenAction)
            {
                AArrayQueue->RemoveAt(tween->queue, i);
                AArrayList_Add(actionList, action);

                return true;
            }
        }
    }

    return false;
}


static bool TryRemoveAllActions(void* tweenId)
{
    int index = AArrayIntMap->GetIndex(tweenMap, (intptr_t) tweenId);

    if (index >= 0)
    {
        Tween* tween = AArrayIntMap_GetAt(tweenMap, index, Tween*);

        for (int i = 0; i < tween->current->size; ++i)
        {
            AArrayList_Add(actionList, AArrayList_Get(tween->current, i, TweenAction*));
        }
        AArrayList->Clear(tween->current);

        TweenAction* action;
        while ((action = AArrayQueue_Pop(tween->queue, TweenAction*)))
        {
            AArrayList_Add(actionList, action);
        }

        // if currentAction not NULL it must be in tweenData->current
        // so just set NULL
        tween->currentAction = NULL;

        return true;
    }

    return false;
}


static inline void SetActionComplete(TweenAction* action, bool isFireOnComplete)
{
    for (int k = 0; k < action->actionValueList->size; ++k)
    {
        TweenActionValue* actionValue = AArrayList_GetPtr(action->actionValueList, k, TweenActionValue);

        actionValue->OnSet
        (
            action->target,
            actionValue->toValue
        );
    }

    if (isFireOnComplete && action->OnComplete != NULL)
    {
        action->OnComplete(action);
    }
}


static bool TryCompleteAllActions(void* tweenId, bool isFireOnComplete)
{
    int index = AArrayIntMap->GetIndex(tweenMap, (intptr_t) tweenId);

    if (index >= 0)
    {
        Tween* tween = AArrayIntMap_GetAt(tweenMap, index, Tween*);

        for (int i = 0; i < tween->current->size; ++i)
        {
            TweenAction* action = AArrayList_Get(tween->current, i, TweenAction*);
            SetActionComplete(action, isFireOnComplete);
            AArrayList_Add(actionList, action);
        }
        AArrayList->Clear(tween->current);

        TweenAction* action;
        while ((action = AArrayQueue_Pop(tween->queue, TweenAction*)))
        {
            SetActionComplete(action, isFireOnComplete);
            AArrayList_Add(actionList, action);
        }

        // if currentAction not NULL it must be in tweenData->current
        // so just set NULL
        tween->currentAction = NULL;

        return true;
    }

    return false;
}


static bool HasAction(void* tweenId)
{
    int index = AArrayIntMap->GetIndex(tweenMap, (intptr_t) tweenId);

    if (index >= 0)
    {
        Tween* tween = AArrayIntMap_GetAt(tweenMap, index, Tween*);

        if (tween->current->size > 0 || tween->queue->elementList->size > 0)
        {
            return true;
        }

        return false;
    }

    return false;
}


static void Update(float deltaSeconds)
{
    for (int i = tweenMap->elementList->size - 1; i > -1; --i)
    {
        Tween* tween = AArrayIntMap_GetAt(tweenMap, i, Tween*);

        // get current action of queue actions
        if (tween->currentAction == NULL)
        {
            tween->currentAction = AArrayQueue_Pop(tween->queue, TweenAction*);

            if (tween->currentAction != NULL)
            {
                // add current action into current array
                AArrayList_Add (tween->current, tween->currentAction);
                SetActionValue(tween->currentAction);
            }
        }

        if (tween->current->size == 0)
        {
            // all actions complete so remove tweenData and push to cache
            AArrayList_Add        (tweenList, tween);
            AArrayIntMap->RemoveAt(tweenMap,  i);
            continue;
        }

        for (int j = tween->current->size - 1; j > -1; --j)
        {
            TweenAction* action = AArrayList_Get(tween->current, j, TweenAction*);

            if (action->curTime < action->duration)
            {
                for (int k = 0; k < action->actionValueList->size; ++k)
                {
                    TweenActionValue* actionValue = AArrayList_GetPtr(action->actionValueList, k, TweenActionValue);

                    actionValue->OnSet
                    (
                        action->target,
                        ATweenEase->Interpolates[actionValue->easeType]
                        (
                            actionValue->fromValue,
                            actionValue->toValue,
                            action->curTime / action->duration
                        )
                    );
                }

                action->curTime += deltaSeconds;
            }
            else
            {
                for (int k = 0; k < action->actionValueList->size; ++k)
                {
                    TweenActionValue* actionValue = AArrayList_GetPtr(action->actionValueList, k, TweenActionValue);
                    actionValue->OnSet(action->target, actionValue->toValue);
                }

                // action complete
                if (action->OnComplete != NULL)
                {
                    action->OnComplete(action);
                }

                if (tween->currentAction == action)
                {
                    tween->currentAction = NULL;
                }

                AArrayList->RemoveByLast(tween->current, j);
                AArrayList_Add(actionList, action);
            }
        }
    }
}


struct ATween ATween[1] =
{
    GetAction,
    AddTweenActionValue,
    RunActions,
    TryRemoveAllActions,
    TryCompleteAllActions,
    TryRemoveAction,
    HasAction,
    Update,
};
