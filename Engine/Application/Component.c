/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The project Mojoc is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2015-8-19
 * Update: 2019-1-27
 * Author: scott.cgi
 */


#include <string.h>
#include <stdlib.h>
#include "Engine/Toolkit/Utils/ArrayIntSet.h"
#include "Engine/Application/Component.h"
#include "Engine/Toolkit/Platform/Log.h"


static void Init(Component* outComponent)
{
    AUserData_Init(outComponent->userData);

    outComponent->order        = 0;
    outComponent->deltaOrder   = 100;
    outComponent->parent       = NULL;
    outComponent->isActive     = true;

    AArrayIntMap->Init(sizeof(Component*), outComponent->childMap);
    AArrayIntSet->Init(outComponent->observerSet);
    AArrayIntMap->InitWithCapacity(sizeof(ComponentState*), 1, outComponent->stateMap);

    outComponent->stateMap->elementList->increase = 5;
    outComponent->defaultState = AComponent->AddState(outComponent, ComponentState_Default, NULL, NULL);
    outComponent->curState     = outComponent->defaultState;
    outComponent->preState     = outComponent->defaultState;
}



static Component* Create()
{
    Component* component = malloc(sizeof(Component));
    Init(component);

    return component;
}


static void Release(Component* component)
{
    AArrayIntMap->Release(component->childMap);
    AArrayIntSet->Release(component->observerSet);

    for (int i = 0; i < component->stateMap->elementList->size; ++i)
    {
        free(AArrayIntMap_GetAt(component->stateMap, i, ComponentState*));
    }

    AArrayIntMap->Release(component->stateMap);

    component->defaultState = NULL;
    component->curState     = NULL;
    component->preState     = NULL;
}


//----------------------------------------------------------------------------------------------------------------------


static void AddChild(Component* parent, Component* child, int order)
{
    ALog_A(parent != NULL && child != NULL, "AComponent AddChild failed, parent and child cannot NULL");
    ALog_A(child->parent == NULL,           "AComponent AddChild failed, child already has parent");

    if (AArrayIntMap_TryPut(parent->childMap, order, child) != NULL)
    {
        child->order  = order;
        child->parent = parent;
    }
    else
    {
        ALog_A(false, "AComponent AddChild failed, order = %d already exist", order);
    }
}


static void AppendChild(Component* parent, Component* child)
{
    ALog_A(parent != NULL && child != NULL, "AComponent AppendChild failed, parent and child cannot NULL");

    int order;

    if (parent->childMap->elementList->size == 0)
    {
        order = parent->deltaOrder;
    }
    else
    {
        order = AArrayIntMap_GetAt
                (
                    parent->childMap,
                    parent->childMap->elementList->size - 1,
                    Component*
                )
                ->order + parent->deltaOrder;
    }

    AddChild(parent, child, order);
}


static void RemoveChild(Component* parent, Component* child)
{
    ALog_A(parent!= NULL && child != NULL, "AComponent RemoveChild failed, parent and child cannot NULL");

    bool isRemoved = AArrayIntMap->TryRemove(parent->childMap, child->order);

    ALog_A
    (
        isRemoved,
        "AComponent cannot found child by order = %d, "
        "may forget called ReorderAllChildren after changed order",
        child->order
    );

    child->parent = NULL;
}


static void RemoveAllChildren(Component* parent)
{
    ALog_A(parent != NULL, "AComponent RemoveAllChildren failed, parent cannot NULL");

    for (int i = 0; i < parent->childMap->elementList->size; ++i)
    {
        Component* child = AArrayIntMap_GetAt(parent->childMap, i, Component*);
        child->parent    = NULL;
    }

    AArrayIntMap->Clear(parent->childMap);
}


static int Compare(const void* a, const void* b)
{
    int keyA = (int) (*(ArrayIntMapElement**) a)->key;
    int keyB = (int) (*(ArrayIntMapElement**) b)->key;
    ALog_A(keyA != keyB, "AComponent ReorderAllChildren failed, two child has same order = %d", keyA);

    return keyA - keyB;
}


static void ReorderAllChildren(Component* parent)
{
    // renew all children key
    for (int i = 0; i < parent->childMap->elementList->size; ++i)
    {
        ArrayIntMapElement* element = AArrayList_Get(parent->childMap->elementList, i, ArrayIntMapElement*);
        element->key                = (*(Component**) element->valuePtr)->order;
    }

    qsort
    (
        parent->childMap->elementList->elementArr->data,
        (size_t) parent->childMap->elementList->size,
        sizeof(ArrayIntMapElement*),
        Compare
    );
}


//----------------------------------------------------------------------------------------------------------------------


static void AddObserver(Component* sender, Component* observer)
{
    ALog_A(sender != NULL && observer != NULL, "AComponent AddObserver failed, sender and observer cannot NULL");

    if (AArrayIntSet->TryAdd(sender->observerSet, (intptr_t) observer) == false)
    {
        ALog_A(false, "AComponent AddObserver failed, observer %p already exist in sender", observer);
    }
}


static void RemoveObserver(Component* sender, Component* observer)
{
    ALog_A(sender != NULL && observer != NULL, "AComponent RemoveObserver failed, sender and observer cannot NULL");

    bool isRemoved = AArrayIntSet->TryRemove(sender->observerSet, (intptr_t) observer);

    ALog_A
    (
        isRemoved,
        "AComponent RemoveObserver sender cannot found observer = %p",
        observer
    );
}


static bool SendMessage(Component* component, void* sender, int subject, void* extraData)
{
    if (component->isActive)
    {
        if(component->curState->OnMessage != NULL)
        {
            if (component->curState->OnMessage(component, sender, subject, extraData))
            {
                // stop message passing
                return true;
            }
        }

        // the order bigger the response first
        for (int i = component->childMap->elementList->size - 1; i > -1 ; --i)
        {
            if
            (
                // if in OnMessage removed parent child, the i index may overflow
                AComponent->SendMessage
                (
                    AArrayIntMap_GetAt(component->childMap, i, Component*),
                    sender,
                    subject,
                    extraData
                )
            )
            {
                // stop message passing
                return true;
            }
        }
    }

    return false;
}


static void Notify(Component* sender, int subject, void* extraData)
{
    ALog_A(sender != NULL, "AComponent Notify failed, sender cannot NULL");

    if (sender->isActive)
    {
        for (int i = 0; i < sender->observerSet->elementList->size; ++i)
        {
            Component* observer = AArrayList_Get(sender->observerSet->elementList, i, Component*);

            if (observer->isActive && observer->curState->OnMessage != NULL)
            {
                observer->curState->OnMessage(observer, sender, subject, extraData);
            }
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------


static void SetState(Component* component, int stateId)
{
    if (component->isActive)
    {
        ComponentState* state = AArrayIntMap_Get(component->stateMap, stateId, ComponentState*);
        ALog_A(state != NULL, "AComponent SetState not found ComponentState by stateId = %d", stateId);

        if (component->curState->OnMessage != NULL)
        {
            component->curState->OnMessage(component, AComponent, ComponentMsg_OnExit, NULL);
        }

        component->preState = component->curState;
        component->curState = state;

        if (state->OnMessage != NULL)
        {
            state->OnMessage(component, AComponent, ComponentMsg_OnEnter, NULL);
        }
    }
}


static ComponentState* AddState
(
    Component*              component,
    int                     stateId,
    ComponentStateOnMessage OnMessage,
    ComponentStateUpdate    Update
)
{
    ALog_A(component != NULL, "AComponent AddState failed, component cannot NULL");

    int index = AArrayIntMap->GetIndex(component->stateMap, stateId);
    ALog_A(index < 0, "AComponent AddState failed, stateId = %d already exist", stateId);

    ComponentState* state = malloc(sizeof(ComponentState));
    state->id             = stateId;
    state->Update         = Update;
    state->UpdateAfter    = NULL;
    state->OnMessage      = OnMessage;
    AUserData_Init(state->userData);

    AArrayIntMap_InsertAt(component->stateMap, stateId, -index - 1, state);

    return state;
}


//----------------------------------------------------------------------------------------------------------------------


static void SetActive(Component* component, bool isActive)
{
    component->isActive = isActive;

    for (int i = 0; i < component->childMap->elementList->size; ++i)
    {
        AArrayIntMap_GetAt(component->childMap, i, Component*)->isActive = isActive;
    }
}


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
                AComponent->Update(AArrayIntMap_GetAt(component->childMap, i,     Component*), deltaSeconds);
                AComponent->Update(AArrayIntMap_GetAt(component->childMap, i + 1, Component*), deltaSeconds);
                AComponent->Update(AArrayIntMap_GetAt(component->childMap, i + 2, Component*), deltaSeconds);
                AComponent->Update(AArrayIntMap_GetAt(component->childMap, i + 3, Component*), deltaSeconds);

                i += 4;
            }

            while (i < component->childMap->elementList->size)
            {
                AComponent->Update(AArrayIntMap_GetAt(component->childMap, i++, Component*), deltaSeconds);
            }

            break;
        }

        if (component->curState->UpdateAfter != NULL)
        {
            component->curState->UpdateAfter(component, deltaSeconds);
        }
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
    SendMessage,
    Notify,

    SetState,
    AddState,
    SetActive,
    Update,
};
