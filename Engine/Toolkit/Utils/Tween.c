/*
 * Tween.c
 *
 *  Created on: 2016-6-8
 *      Author: scott.cgi
 */

#include "Engine/Toolkit/Utils/Tween.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Define/Def.h"
#include "Engine/Toolkit/Utils/ArrayIntMap.h"
#include "Engine/Toolkit/Utils/ArrayQueue.h"

typedef struct
{
	/**
	 * Target action value queue, run each after over
	 */
	ArrayQueue(TweenAction*) queue  [1];

	/**
	 * Target current running action value
	 */
	ArrayList(TweenAction*)  current[1];

	/**
	 * One running action of queue
	 */
	TweenAction*             currentAction;
}
TweenData;


static ArrayIntMap(tweendId, TweenData*) dataMap   [1] = AArrayIntMapInit(TweenData*,   25);
static ArrayList  (TweenData*)           dataList  [1] = AArrayListInit  (TweenData*,   25);
static ArrayList  (TweenAction*)         actionList[1] = AArrayListInit  (TweenAction*, 25);

static inline TweenData* GetTweenData()
{
    TweenData* tweenData = AArrayListPop(dataList, TweenData*);

    if (tweenData == NULL)
    {
        tweenData = (TweenData*) malloc(sizeof(TweenData));

        AArrayQueue->Init(sizeof(TweenAction*), tweenData->queue);
        AArrayQueueSetIncrease(tweenData->queue, 6);

        AArrayList->Init(sizeof(TweenAction*),  tweenData->current);
        tweenData->current->increase = 6;
    }
    else
    {
		AArrayQueue->Clear(tweenData->queue);
		AArrayList ->Clear(tweenData->current);
    }

    tweenData->currentAction = NULL;

    return tweenData;
}


static inline void Release(TweenAction* action)
{
	AArrayList->Release(action->actionValueList);
}


static TweenAction* GetAction()
{
	TweenAction* action = AArrayListPop(actionList, TweenAction*);

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

	action->curTime    = 0.0f;
	action->duration   = 0.0f;
	action->OnComplete = NULL;
	action->userData   = NULL;
	action->isQueue    = true;
	action->target     = NULL;

	return action;
}

static TweenActionValue* AddTweenActionValue(TweenAction* action)
{
	TweenActionValue* actionValue = AArrayListGetPtrAdd(action->actionValueList, TweenActionValue);

	actionValue->userData   = NULL;
	actionValue->value      = 0.0f;
	actionValue->fromValue  = 0.0f;
	actionValue->toValue    = 0.0f;
	actionValue->OnGet      = NULL;
	actionValue->OnSet      = NULL;
	actionValue->isRelative = true;
	actionValue->easeType   = tween_ease_linear;

	return actionValue;
}

static inline void SetActionValue(TweenAction* action)
{
	for (int i = 0; i < action->actionValueList->size; i++)
	{
		TweenActionValue* actionValue = AArrayListGetPtr(action->actionValueList, i, TweenActionValue);

		ALogA(actionValue->OnGet != NULL && actionValue->OnSet != NULL, "TweenActionValue OnSet OnGet must not NULL");

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
	TweenData* tweenData;

	if (tweenId == NULL)
	{
		 // not give tweenId, we use TweenData ptr for it
        tweenData = GetTweenData();
        tweenId   = tweenData;

        AArrayIntMapInsertAt
        (
            dataMap,
            tweenId,
            -AArrayIntMap->GetIndex(dataMap, (intptr_t) tweenId) - 1,
            tweenData
        );
	}
    else
    {
        int index = AArrayIntMap->GetIndex(dataMap, (intptr_t) tweenId);

        if (index < 0)
        {
            tweenData = GetTweenData();
            AArrayIntMapInsertAt(dataMap, tweenId, -index - 1, tweenData);
        }
        else
        {
            tweenData = AArrayIntMapGetAt(dataMap, index, TweenData*);
        }
    }

//--------------------------------------------------------------------------------------------------

	for (int i = 0; i < actions->length; i++)
	{
		TweenAction* action = AArrayGet(actions, i, TweenAction*);

		if (action->isQueue)
		{
			AArrayQueuePush(tweenData->queue, action);
		}
		else
		{
			AArrayListAdd(tweenData->current, action);
			SetActionValue(action);
		}
	}

	return tweenId;
}


static bool TryRemoveAction(void* tweenId, TweenAction* action)
{
	TweenData* tweenData = AArrayIntMapGet(dataMap, tweenId, TweenData*);

	if (tweenData != NULL)
	{
		for (int i = 0; i < tweenData->current->size; i++)
		{
			TweenAction* tweenAction = AArrayListGet(tweenData->current, i, TweenAction*);

			if (action == tweenAction)
			{
				if (action == tweenData->currentAction)
				{
					tweenData->currentAction = NULL;
				}

				AArrayList->RemoveByLast(tweenData->current, i);
				AArrayListAdd(actionList, action);

				return true;
			}
		}

//--------------------------------------------------------------------------------------------------

		for (int i = tweenData->queue->topIndex; i < tweenData->queue->arrayList->size; i++)
		{
			TweenAction* tweenAction = AArrayListGet(tweenData->queue->arrayList, i, TweenAction*);

			if (action == tweenAction)
			{
				AArrayQueue->RemoveAt(tweenData->queue, i);
				AArrayListAdd(actionList, action);

				return true;
			}
		}
	}

	return false;
}


static bool TryRemoveAllActions(void* tweenId)
{
	int index = AArrayIntMap->GetIndex(dataMap, (intptr_t) tweenId);

	if (index >= 0)
	{
		TweenData* tweenData = AArrayIntMapGetAt(dataMap, index, TweenData*);

		for (int i = 0; i < tweenData->current->size; i++)
		{
			AArrayListAdd
			(
				actionList,
				AArrayListGet(tweenData->current, i, TweenAction*)
			);
		}
		AArrayList->Clear(tweenData->current);

//--------------------------------------------------------------------------------------------------

		TweenAction* action;
		while ((action = AArrayQueuePop(tweenData->queue, TweenAction*)))
		{
			AArrayListAdd(actionList, action);
		}

        // if currentAction not NULL it must be in tweenData->current
        // so just set NULL
        tweenData->currentAction = NULL;

		return true;
	}

	return false;
}

static inline void SetActionComplete(TweenAction* action, bool isFireOnComplete)
{
    for (int k = 0; k < action->actionValueList->size; k++)
    {
        TweenActionValue* actionValue = AArrayListGetPtr(action->actionValueList, k, TweenActionValue);

        actionValue->OnSet
        (
            action->target,
            actionValue->toValue
        );
    }

    if (isFireOnComplete && action->OnComplete != NULL)
    {
        action->OnComplete(action, action->userData);
    }
}

static bool TryCompleteAllActions(void* tweenId, bool isFireOnComplete)
{
    int index = AArrayIntMap->GetIndex(dataMap, (intptr_t) tweenId);

    if (index >= 0)
    {
        TweenData* tweenData = AArrayIntMapGetAt(dataMap, index, TweenData*);

        for (int i = 0; i < tweenData->current->size; i++)
        {
            TweenAction* action = AArrayListGet(tweenData->current, i, TweenAction*);

            SetActionComplete(action, isFireOnComplete);
            AArrayListAdd(actionList, action);
        }
        AArrayList->Clear(tweenData->current);

//--------------------------------------------------------------------------------------------------

        TweenAction* action;
        while ((action = AArrayQueuePop(tweenData->queue, TweenAction*)))
        {
            SetActionComplete(action, isFireOnComplete);
            AArrayListAdd(actionList, action);
        }

        // if currentAction not NULL it must be in tweenData->current
        // so just set NULL
        tweenData->currentAction = NULL;

        return true;
    }

    return false;
}


static bool HasAction(void* tweenId)
{
    int index = AArrayIntMap->GetIndex(dataMap, (intptr_t) tweenId);

    if (index >= 0)
    {
        TweenData* tweenData = AArrayIntMapGetAt(dataMap, index, TweenData*);

        if (tweenData->current->size > 0 || tweenData->queue->arrayList->size > 0)
        {
            return true;
        }

        return false;
    }

    return false;
}


static void Update(float deltaTime)
{
	for (int i = dataMap->arrayList->size - 1; i > -1; i--)
	{
		TweenData* tweenData = AArrayIntMapGetAt(dataMap, i, TweenData*);

		// get current action of queue actions
		if (tweenData->currentAction == NULL)
		{
			tweenData->currentAction = AArrayQueuePop(tweenData->queue, TweenAction*);

			if (tweenData->currentAction != NULL)
			{
				// add current action into current array
				AArrayListAdd (tweenData->current, tweenData->currentAction);
				SetActionValue(tweenData->currentAction);
			}
		}

		if (tweenData->current->size == 0)
		{
			// all actions complete so remove tweenData and push to cache
            AArrayListAdd         (dataList, tweenData);
            AArrayIntMap->RemoveAt(dataMap,  i);
			continue;
		}

//--------------------------------------------------------------------------------------------------

		for (int j = tweenData->current->size - 1; j > -1; j--)
		{
			TweenAction* action = AArrayListGet(tweenData->current, j, TweenAction*);

			if (action->curTime < action->duration)
			{
                for (int k = 0; k < action->actionValueList->size; k++)
                {
                    TweenActionValue* actionValue = AArrayListGetPtr(action->actionValueList, k, TweenActionValue);

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

                action->curTime += deltaTime;
			}
			else
			{
                for (int k = 0; k < action->actionValueList->size; k++)
                {
                    TweenActionValue* actionValue = AArrayListGetPtr(action->actionValueList, k, TweenActionValue);

                    actionValue->OnSet
                    (
                        action->target,
                        actionValue->toValue
                    );
                }

//--------------------------------------------------------------------------------------------------

                // action complete
                if (action->OnComplete != NULL)
                {
                    action->OnComplete(action, action->userData);
                }

                if (tweenData->currentAction == action)
                {
                    tweenData->currentAction = NULL;
                }

                AArrayList->RemoveByLast(tweenData->current, j);
                AArrayListAdd(actionList, action);
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
