/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2015-8-19
 * Author: scott.cgi
 */

#include <string.h>
#include <stdlib.h>
#include <Engine/Toolkit/Utils/ArrayIntSet.h>

#include "Engine/Application/Component.h"
#include "Engine/Toolkit/Platform/Log.h"


static void Init(Component* outComponent)
{
    AUserDataInit(outComponent->userData);

	outComponent->order         = 0;
	outComponent->increaseOrder = 50;
	outComponent->parent        = NULL;
	outComponent->isActive      = true;

	AArrayIntMap->Init            (sizeof(Component*),         outComponent->childMap);
	AArrayIntSet->Init            (                            outComponent->observerSet);
	AArrayIntMap->InitWithCapacity(sizeof(ComponentState*), 1, outComponent->stateMap);

    outComponent->stateMap->elementList->increase = 5;
	outComponent->defaultState                    = AComponent->AddState(outComponent, component_state_default, NULL, NULL);
	outComponent->curState                        = outComponent->defaultState;
	outComponent->preState                        = outComponent->defaultState;
}



static Component* Create()
{
	Component* component = (Component*) malloc(sizeof(Component));
	Init(component);

	return component;
}


static void Release(Component* component)
{
	AArrayIntMap->Release(component->childMap);
	AArrayIntSet->Release(component->observerSet);

	for (int i = 0; i < component->stateMap->elementList->size; i++)
	{
		free(AArrayIntMapGetAt(component->stateMap, i, ComponentState*));
	}

	AArrayIntMap->Release(component->stateMap);
}


//--------------------------------------------------------------------------------------------------


static void AddChild(Component* parent, Component* child, int order)
{
	ALogA(parent != NULL && child != NULL, "Component addChild error, parent and child can not NULL");
	ALogA(child->parent == NULL,           "Component addChild error, child already has parent");

    if (AArrayIntMapPut(parent->childMap, order, child) != NULL)
    {
        child->order  = order;
        child->parent = parent;
    }
    else
    {
        ALogA(false, "Component addChild error, order = %d already exist", order);
    }
}


static void AppendChild(Component* parent, Component* child)
{
	ALogA(parent != NULL && child != NULL, "Component appendChild error, parent and child can not NULL");

	int order;

	if (parent->childMap->elementList->size == 0)
	{
		order = parent->increaseOrder;
	}
	else
	{
		order = AArrayIntMapGetAt
                (
                    parent->childMap,
                    parent->childMap->elementList->size - 1,
                    Component*
                )
                ->order + parent->increaseOrder;
	}

	AddChild(parent, child, order);
}


static void	RemoveChild(Component* parent, Component* child)
{
	ALogA(parent!= NULL && child != NULL, "Component removeChild error, parent and child can not NULL");

	bool isRemoved = AArrayIntMap->TryRemove(parent->childMap, child->order);

	ALogA
	(
		isRemoved,
		"Component can't found child by order = %d, "
		"may forget called reorderAllChildren when changed order",
		child->order
	);

	child->parent = NULL;
}


static void RemoveAllChildren(Component* parent)
{
	ALogA(parent != NULL, "Component removeAllChildren error, parent can not NULL");

	for (int i = 0; i < parent->childMap->elementList->size; i++)
	{
		Component* child = AArrayIntMapGetAt(parent->childMap, i, Component*);
		child->parent    = NULL;
	}

	AArrayIntMap->Clear(parent->childMap);
}


static int Compare(const void* a, const void* b)
{
	int keyA = (int) (*(ArrayIntMapElement**) a)->key;
	int keyB = (int) (*(ArrayIntMapElement**) b)->key;
	ALogA(keyA != keyB, "Component ReorderAllChildren error, Two child has same order = %d", keyA);

	return keyA - keyB;
}


static void ReorderChildren(Component* parent)
{
	// renew all children key
	for (int i = 0; i < parent->childMap->elementList->size; i++)
	{
		ArrayIntMapElement* element = AArrayListGet(parent->childMap->elementList, i, ArrayIntMapElement*);
		element->key                = (*(Component**) element->valuePtr)->order;
	}

	qsort
	(
        parent->childMap->elementList->elementArray->data,
        parent->childMap->elementList->size,
		sizeof(ArrayIntMapElement*),
		Compare
	);
}


static void AddObserver(Component* sender, Component* observer)
{
	ALogA(sender != NULL && observer != NULL, "Component addObserver error, sender and observer can not NULL");
    AArrayIntSet->Add(sender->observerSet, (intptr_t) observer);
}


static void RemoveObserver(Component* sender, Component* observer)
{
	ALogA(sender != NULL && observer != NULL, "Component removeObserver error, sender and observer can not NULL");

	bool isRemoved = AArrayIntSet->TryRemove(sender->observerSet, (intptr_t) observer);

	ALogA
	(
		isRemoved,
		"sender can't found observer = %p",
		observer
	);
}


//--------------------------------------------------------------------------------------------------


static void Update(Component* component, float deltaSeconds)
{
	if (component->isActive)
	{
		if (component->curState->Update != NULL)
		{
			component->curState->Update(component, deltaSeconds);
		}

        for (int i = 0;;)
        {
            while (i + 3 < component->childMap->elementList->size)
            {
                AComponent->Update(AArrayIntMapGetAt(component->childMap, i,     Component*), deltaSeconds);
                AComponent->Update(AArrayIntMapGetAt(component->childMap, i + 1, Component*), deltaSeconds);
                AComponent->Update(AArrayIntMapGetAt(component->childMap, i + 2, Component*), deltaSeconds);
                AComponent->Update(AArrayIntMapGetAt(component->childMap, i + 3, Component*), deltaSeconds);

                i += 4;
            }

            while (i < component->childMap->elementList->size)
            {
                AComponent->Update(AArrayIntMapGetAt(component->childMap, i++, Component*), deltaSeconds);
            }

            break;
        }

		if (component->curState->UpdateAfter != NULL)
		{
			component->curState->UpdateAfter(component, deltaSeconds);
		}
	}
}


static bool SendMessage(Component* component, void* sender, int subject, void* extraData)
{
	if (component->isActive)
	{
		if (component->curState->OnMessage != NULL && component->curState->OnMessage(component, sender, subject, extraData))
		{
			return true;
		}

		// big order first
		for (int i = component->childMap->elementList->size - 1; i > -1 ; i--)
		{
			// if in OnMessage method remove parent child more than twice
            // the i index will overflow
			if (AComponent->SendMessage(AArrayIntMapGetAt(component->childMap, i, Component*), sender, subject, extraData))
			{
				return true;
			}
		}
	}

	return false;
}


static bool SendMessageToChildren(Component* component, void* sender, int subject, void* extraData)
{
	if (component->isActive)
	{
        // big order first
		for (int i = component->childMap->elementList->size - 1; i > -1; i--)
		{
            // if in OnMessage method remove parent child more than twice
            // the i index will overflow
			if (AComponent->SendMessage(AArrayIntMapGetAt(component->childMap, i, Component*), sender, subject, extraData))
			{
				return true;
			}
		}
	}

	return false;
}


static void Notify(Component* sender, int subject, void* extraData)
{
	ALogA(sender != NULL, "Component notify error, sender can not NULL");

	if (sender->isActive)
	{
		for (int i = 0; i < sender->observerSet->elementList->size; i++)
		{
			Component* observer = AArrayListGet(sender->observerSet->elementList, i, Component*);

			if
			(
               observer->isActive                    &&
               observer->curState->OnMessage != NULL &&
               observer->curState->OnMessage(observer, sender, subject, extraData)
			)
			{
				break;
			}
		}
	}
}


//--------------------------------------------------------------------------------------------------


static void SetState(Component* component, int stateId)
{
	if (component->isActive)
	{
		ComponentState* state = AArrayIntMapGet(component->stateMap, stateId, ComponentState*);
		ALogA(state != NULL, "Component SetState not found ComponentState by stateId = %d", stateId);

		if (component->curState->OnMessage != NULL)
		{
			component->curState->OnMessage(component, AComponent, component_msg_on_exit, NULL);
		}

		component->preState = component->curState;
		component->curState = state;

		if (state->OnMessage != NULL)
		{
			state->OnMessage(component, AComponent, component_msg_on_enter, NULL);
		}
	}
}

static ComponentState* AddState(Component* component, int stateId, ComponentStateOnMessage onMessage, ComponentStateUpdate update)
{
	ALogA(component != NULL, "Component addState error, component can not NULL");

	int index = AArrayIntMap->GetIndex(component->stateMap, stateId);
	ALogA(index < 0, "Component addState error, stateId = %d already exist", stateId);

	ComponentState* state = (ComponentState*) malloc(sizeof(ComponentState));
    state->id             = stateId;
    state->Update         = update;
    state->UpdateAfter    = NULL;
    state->OnMessage      = onMessage;
    AUserDataInit(state->userData);

	AArrayIntMapInsertAt(component->stateMap, stateId, -index - 1, state);

	return state;
}


static void SetActive(Component* component, bool isActive)
{
	component->isActive = isActive;

	for (int i = 0; i < component->childMap->elementList->size; i++)
	{
		AArrayIntMapGetAt(component->childMap, i, Component*)->isActive = isActive;
	}
}


struct AComponent AComponent[1] =
{
	Create,
	Init,
	Release,

	AddChild,
	AppendChild,
	RemoveChild,
	RemoveAllChildren,
	ReorderChildren,
	AddObserver,
	RemoveObserver,

	Update,
	SendMessage,
	SendMessageToChildren,
	Notify,

	SetState,
	AddState,
	SetActive,
};
