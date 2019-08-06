/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2015-8-19
 * Update   : 2019-1-26
 * Author   : scott.cgi
 */


#ifndef COMPONENT_H
#define COMPONENT_H


#include "Engine/Toolkit/Utils/ArrayIntMap.h"
#include "Engine/Toolkit/Utils/ArrayIntSet.h"
#include "Engine/Toolkit/HeaderUtils/UserData.h"


typedef struct Component      Component;
typedef struct ComponentState ComponentState;


/**
 * Component has a State-Machine, message subscription list, and component child list.
 * State-Machine make component can switch self state.
 * message subscription make component can send message to subscriber.
 * components and sub components form a tree structure that make message can delivery to any child.
 *
 * also component can sort children by order, but not draw or render anything.
 * The function of draw or render need to sub component implementation,
 * such as SpriteComponent that is component with sprite,
 * and then the component can draw sprite,
 * the sprite has component's State-Machine, message subscription so on.
 */
struct Component
{
    UserData                              userData[1];

    /**
     * If isActive false Component will not change state, update state, handle and send message.
     */
    bool                                  isActive;

    /**
     * Order in parent children list when add in auto setting.
     * if changed it then ReorderAllChildren will resort Component order in parent list.
     */
    int                                   order;

    /**
     * When append child the order increased by deltaOrder, default 100.
     */
    int                                   deltaOrder;

    /**
     * Component's parent in a tree structure.
     */
    Component*                            parent;

    /**
     * Current active state, default is defaultState.
     */
    ComponentState*                       curState;

    /**
     * Previous active state, default is defaultState.
     */
    ComponentState*                       preState;

    /**
     * Default state when Component created with stateID ComponentState_Default.
     */
    ComponentState*                       defaultState;

    /**
     * Children mapped by Component order.
     */
    ArrayIntMap(order, Component*)        childMap   [1];

    /**
     * Component notification observers.
     */
    ArrayIntSet(Component*)               observerSet[1];

    /**
     * All ComponentStates mapped by ComponentState's id.
     */
    ArrayIntMap(stateID, ComponentState*) stateMap   [1];
};



/**
 * ComponentState stateID.
 */
enum
{
    /**
     * ComponentState default stateID, any other stateID must be different.
     */
    ComponentState_Default = 0
};


/**
 * ComponentState subject.
 *
 * Component's ComponentState may receive this type of subject message,
 * then OnMessage called for handle.
 */
enum
{
    /**
     * When ComponentState enter.
     * 
     * sender   : AComponent
     * extraData: NULL
     */
    ComponentMsg_OnEnter,

    /**
     * When ComponentState exit.
     *
     * sender   : AComponent
     * extraData: NULL
     */
    ComponentMsg_OnExit,
};


/**
 * Component's ComponentState for handle different logic in different states.
 */
struct ComponentState
{
    UserData  userData[1];

    /**
     * Key in Component's stateMap
     */
    int       id;

    /**
     * Active ComponentState called every frame.
     */
    void (*Update)      (Component* component, float deltaSeconds);

    /**
     * After Component's children updated called every frame.
     */
    void (*UpdateAfter) (Component* component, float deltaSeconds);

    /**
     * When message received called.
     * if return true means consumed event then will stop event passing.
     */
    bool (*OnMessage)   (Component* component, void* sender, int subject, void* extraData);
};


typedef void (*ComponentStateUpdate)   (Component* component, float deltaSeconds);
typedef bool (*ComponentStateOnMessage)(Component* component, void* sender, int subject, void* extraData);


/**
 * Manage and control Component.
 */
struct AComponent
{
    /**
     * Create new Component that malloc memory and need to free it manually.
     */
    Component*      (*Create)               (void);

    /**
     * Init Component that memory has already malloc.
     */
    void            (*Init)                 (Component* outComponent);

    /**
     * Free members memory and reset some property values.
     */
    void            (*Release)              (Component* component);

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Add child into parent's children list by order.
     */
    void            (*AddChild)             (Component* parent, Component* child, int order);

    /**
     * Append child and auto set child order by deltaOrder.
     */
    void            (*AppendChild)          (Component* parent, Component* child);

    /**
     * Remove child from parent's children list and reindex other children.
     */
    void            (*RemoveChild)          (Component* parent, Component* child);

    /**
     * Remove all children from parent.
     */
    void            (*RemoveAllChildren)    (Component* parent);

    /**
     * Reorder all children by child's order.
     * we should apply this method after Component order changed. (may after all orders changes have been completed)
     */
    void            (*ReorderAllChildren)   (Component* parent);

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Add observer to sender, will receive notification by sender.
     */
    void            (*AddObserver)          (Component* sender, Component* observer);

    /**
     * Remove observer from sender.
     */
    void            (*RemoveObserver)       (Component* sender, Component* observer);

    /**
     * Send message to component self and children (depth-first traversal),
     * and will call self and children's ComponentState OnMessage method,
     * and the component order bigger the response first.
     * 
     * if any OnMessage return true means consumed event then will stop event passing.
     * if in OnMessage removed parent child will cause error.
     * if component isActive false will do nothing.
     */
    bool            (*SendMessage)          (Component* component, void* sender, int subject, void* extraData);

    /**
     * Notify sender all observers,
     * and will call observers ComponentState OnMessage method.
     *
     * if sender isActive false will do nothing.
     * if observers isActive false will not call OnMessage.
     * if in observers OnMessage removed sender observer will cause error.
     */
    void            (*Notify)               (Component* sender, int subject, void* extraData);

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Change Component current active State.
     * if component isActive false will do nothing.
     */
    void            (*SetState)             (Component* component, int stateID);

    /**
     * Create ComponentState with stateID, and add into Component.
     * the add ComponentState free by Component's Release.
     */
    ComponentState* (*AddState)             (
                                                Component*              component,
                                                int                     stateID,
                                                ComponentStateOnMessage OnMessage,
                                                ComponentStateUpdate    Update
                                            );

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Set Component and all children active state.
     */
    void            (*SetActive)            (Component* component, bool isActive);

    /**
     * Update Component self and children's state that will call ComponentState update method.
     */
    void            (*Update)               (Component* component, float deltaSeconds);
};


extern struct AComponent AComponent[1];


#endif
