/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2015-8-19
 * Author: scott.cgi
 */

#ifndef component_h
#define component_h


#include "Engine/Toolkit/Utils/ArrayIntMap.h"
#include "Engine/Toolkit/Head/UserData.h"


typedef struct Component      Component;
typedef struct ComponentState ComponentState;


struct Component
{
    UserData                                   userData[1];

	/**
	 * If isActive false Component will not change state, update state, handle and send message
	 */
	bool                                       isActive;

	/**
	 * Order in parent children list when add in
	 * changed it and reorderAllChildren will sort by order
	 */
	int                                        order;

	/**
	 * When append child order auto increase by increaseOrder add last child order
	 * default 50
	 */
	int                                        increaseOrder;

	/**
	 * Component's parent in a tree structure
	 */
	Component*                                 parent;

	/**
	 * Current active state, default empty state with stateId 0
	 */
	ComponentState*                            curState;

	/**
	 * Previous active state, default empty state with stateId 0
	 */
	ComponentState*                            preState;

	/**
	 * Default state when Component created with stateId 0
	 */
	ComponentState*                            defaultState;

	/**
	 * Children mapped by Component order
	 */
	ArrayIntMap(order,        Component*)      childMap   [1];

	/**
	 * Component notification observers
	 */
	ArrayIntMap(componentPtr, Component*)      observerMap[1];

	/**
	 * All ComponentStates mapped by ComponentState's id
	 */
	ArrayIntMap(stateId,      ComponentState*) stateMap  [1];
};


enum
{
	/**
	 * ComponentState default state id
	 * any other state id must be different
	 */
	component_state_default = 0
};


/**
 * Component's any ComponentState may receive this type of subject message
 * then 'OnMessage' called for handle
 * and the sender is AComponent
 */
enum
{
	/**
	 * When ComponentState enter
	 */
	component_msg_on_enter,

	/**
	 * When ComponentState exit
	 */
	component_msg_on_exit,
};


struct ComponentState
{
    UserData  userData[1];

    /**
     * Key in Component's stateMap
     */
	int       id;

	/**
	 * Active ComponentState called per frame
	 */
	void (*Update)      (Component* component, float deltaSeconds);

	/**
	 * After Component's children updated called per frame
	 */
	void (*UpdateAfter) (Component* component, float deltaSeconds);

	/**
	 * When message received called
	 * if return true means consumed event then will stop event pass
	 */
	bool (*OnMessage)   (Component* component, void* sender, int subject, void* extraData);
};


typedef void (*ComponentStateUpdate)   (Component* component, float deltaSeconds);
typedef bool (*ComponentStateOnMessage)(Component* component, void* sender, int subject, void* extraData);


struct AComponent
{
	/**
	 * Create new Component that allocate memory and need to free it manually
	 */
	Component*      (*Create)               ();

	/**
	 * Initialize Component that memory has already allocated
	 */
	void            (*Init)                 (Component* outComponent);

	/**
	 * Free members memory that allocate by malloc and reset some property values
	 */
	void            (*Release)              (Component* component);

	/**
	 * Add Component into parent children list
	 */
	void            (*AddChild)             (Component* parent, Component* child, int order);

	/**
	 * Add child and auto child order by appendZOrder add last child order
	 */
	void            (*AppendChild)          (Component* parent, Component* child);

    /**
     *  Remove Component from children list reindex other children
	 */
	void            (*RemoveChild)          (Component* parent, Component* child);

	/**
	 * Remove all children from parent
	 */
	void            (*RemoveAllChildren)    (Component* parent);

	/**
	 * Reorder all children after changed child's order
	 */
	void            (*ReorderAllChildren)   (Component* parent);

	/**
	 * Add observer to sender, will receive notification by sender
	 */
	void            (*AddObserver)          (Component* sender, Component* observer);

	/**
	 * Remove observer from sender
	 */
	void            (*RemoveObserver)       (Component* sender, Component* observer);

	/**
	 * Call self and children's ComponentState update
	 */
	void            (*Update)               (Component* component, float deltaSeconds);

	/**
	 * Call self and children's ComponentState onMessage
	 * if return true means consumed event then will stop event pass
	 */
	bool            (*SendMessage)          (Component* component, void* sender, int subject, void* extraData);

	/**
	 * Call children's ComponentState onMessage
	 * if return true means consumed event then will stop event pass
	 */
	bool            (*SendMessageToChildren)(Component* component, void* sender, int subject, void* extraData);

	/**
	 * Notify sender all observer's ComponentState OnMessage
	 */
	void            (*Notify)               (Component* sender, int subject, void* extraData);

	/**
	 * Change Component current active State
	 */
	void            (*SetState)             (Component* component, int stateId);

	/**
	 * Add ComponentState in Component which create with state id
	 * free by Componet's Release
	 */
	ComponentState* (*AddState)             (Component* component, int stateId, ComponentStateOnMessage onMessage, ComponentStateUpdate update);

	/**
	 * Set Component active state
	 */
	void            (*SetActive)            (Component* component, bool isActive);
};


extern struct AComponent AComponent[1];


static inline int AComponentGetChildCount(Component* component)
{
	return component->childMap->elementList->size;
}


static inline int AComponentGetCurStateId(Component* component)
{
	return component->curState->id;
}


/**
 * How many capacity increase when stateMap full
 */
static inline void AComponentSetStateIncrease(Component *component, int increase)
{
	component->stateMap->elementList->increase = increase;
}


#endif
