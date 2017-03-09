/*
 *
 *  Created on: 2015-8-19
 *      Author: scott.cgi
 */

#include <string.h>
#include <stdlib.h>

#include "Engine/Application/Component.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Head/Def.h"


static void Init(Component* outComponent)
{
	outComponent->userData      = NULL;
	outComponent->order         = 0;
	outComponent->increaseOrder = 50;
	outComponent->parent        = NULL;
	outComponent->isActive      = true;

	AArrayIntMap->Init(sizeof(Component*), outComponent->childMap);
	AArrayIntMap->Init(sizeof(Component*), outComponent->observerMap);

	AArrayIntMap->InitWithCapacity(sizeof(ComponentState*), 1, outComponent->stateMap);
	AArrayIntMapSetIncrease(outComponent->stateMap, 5);

	outComponent->defualtState = AComponent->AddState(outComponent, component_state_default, NULL, NULL);
	outComponent->curState     = outComponent->defualtState;
	outComponent->preState     = outComponent->defualtState;
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
	AArrayIntMap->Release(component->observerMap);

	for (int i = 0; i < component->stateMap->arrayList->size; i++)
	{
		free(AArrayIntMapGetAt(component->stateMap, i, ComponentState*));
	}

	AArrayIntMap->Release(component->stateMap);
}


//--------------------------------------------------------------------------------------------------

static void AddChild(Component* parent, Component* child, int order)
{
	ALogA(parent != NULL && child != NULL, "Component addChild failed, parent and child can not NULL");
	ALogA(child->parent == NULL,           "Component addChild failed, child already has parent");

	int index = AArrayIntMap->GetIndex(parent->childMap, order);
	ALogA(index < 0, "Component addChild failed, order = %d already exist", order);

	child->order          = order;
	child->parent         = parent;
	AArrayIntMapInsertAt(parent->childMap, order, -index - 1, child);
}


static void AppendChild(Component* parent, Component* child)
{
	ALogA(parent != NULL && child != NULL, "Component appendChild failed, parent and child can not NULL");

	int order;

	if (parent->childMap->arrayList->size == 0)
	{
		order = parent->increaseOrder;
	}
	else
	{
		order = AArrayIntMapGetAt
                (
                    parent->childMap,
                    parent->childMap->arrayList->size - 1,
                    Component*
                )
                ->order + parent->increaseOrder;
	}

	AddChild(parent, child, order);
}


static void	RemoveChild(Component* parent, Component* child)
{
	ALogA(parent!= NULL && child != NULL, "Component removeChild failed, parent and child can not NULL");

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
	ALogA(parent != NULL, "Component removeAllChildren failed, parent can not NULL");

	for (int i = 0; i < parent->childMap->arrayList->size; i++)
	{
		Component* child = AArrayIntMapGetAt(parent->childMap, i, Component*);
		child->parent    = NULL;
	}

	AArrayIntMap->Clear(parent->childMap);
}


static int Compare(const void* a, const void* b)
{
	int keyA = (*(ArrayIntMapElement**) a)->key;
	int keyB = (*(ArrayIntMapElement**) b)->key;
	ALogA(keyA != keyB, "Component ReorderAllChildren failed ! Two child has same order = %d", keyA);

	return keyA - keyB;
}



static void ReorderAllChildren(Component* parent)
{
	// renew all children key
	for (int i = 0; i < parent->childMap->arrayList->size; i++)
	{
		ArrayIntMapElement* element = AArrayListGet(parent->childMap->arrayList, i, ArrayIntMapElement*);
		element->key                = (*(Component**) element->valuePtr)->order;
	}

	qsort
	(
        parent->childMap->arrayList->array->data,
        parent->childMap->arrayList->size,
		sizeof(void**),
		Compare
	);
}



static void AddObserver(Component* sender, Component* observer)
{
	ALogA(sender != NULL && observer != NULL, "Component addObserver failed, sender and observer can not NULL");

	int index = AArrayIntMap->GetIndex(sender->observerMap, (intptr_t) observer);
	ALogA(index < 0, "Component addObserver failed, observer = %p already exist", observer);

	AArrayIntMapInsertAt(sender->observerMap, observer, -index - 1, observer);
}


static void RemoveObserver(Component* sender, Component* observer)
{
	ALogA(sender != NULL && observer != NULL, "Component removeObserver failed, sender and observer can not NULL");

	bool isRemoved = AArrayIntMap->TryRemove(sender->observerMap,  (intptr_t) observer);

	ALogA
	(
		isRemoved,
		"sender can't found observer = %p",
		observer
	);
}

//--------------------------------------------------------------------------------------------------


static void Update(Component* component, float deltaTime)
{
	if (component->isActive)
	{
		if (component->curState->Update != NULL)
		{
			component->curState->Update(component, deltaTime);
		}

        for (int i = 0;;)
        {
            while (i + 3 < component->childMap->arrayList->size)
            {
                AComponent->Update(AArrayIntMapGetAt(component->childMap, i,     Component*), deltaTime);
                AComponent->Update(AArrayIntMapGetAt(component->childMap, i + 1, Component*), deltaTime);
                AComponent->Update(AArrayIntMapGetAt(component->childMap, i + 2, Component*), deltaTime);
                AComponent->Update(AArrayIntMapGetAt(component->childMap, i + 3, Component*), deltaTime);

                i += 4;
            }

            while (i < component->childMap->arrayList->size)
            {
                AComponent->Update(AArrayIntMapGetAt(component->childMap, i++, Component*), deltaTime);
            }

            break;
        }

		if (component->curState->UpdateAfter != NULL)
		{
			component->curState->UpdateAfter(component, deltaTime);
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
		for (int i = component->childMap->arrayList->size - 1; i > -1 ; i--)
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
		for (int i = component->childMap->arrayList->size - 1; i > -1; i--)
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
	ALogA(sender != NULL, "Component notify failed, sender can not NULL");

	if (sender->isActive)
	{
		for (int i = 0; i < sender->observerMap->arrayList->size; i++)
		{
			Component* observer = AArrayIntMapGetAt(sender->observerMap, i, Component*);

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
	ALogA(component != NULL, "Component addState failed, component can not NULL");

	int index = AArrayIntMap->GetIndex(component->stateMap, stateId);
	ALogA(index < 0, "Component addState fail, stateId = %d already exist", stateId);

	ComponentState* state = (ComponentState*) malloc(sizeof(ComponentState));
    memset(state, 0, sizeof(ComponentState));
    state->id             = stateId;
    state->OnMessage      = onMessage;
    state->Update         = update;

	AArrayIntMapInsertAt(component->stateMap, stateId, -index - 1, state);

	return state;
}


static void SetActive(Component* component, bool isActive)
{
	component->isActive = isActive;

	for (int i = 0; i < component->childMap->arrayList->size; i++)
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
	ReorderAllChildren,
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