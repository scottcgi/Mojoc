/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2016-6-8
 * Update   : 2019-1-8
 * Author   : scott.cgi
 */


#ifndef TWEEN_H
#define TWEEN_H


#include <stddef.h>
#include <stdbool.h>
#include "Engine/Toolkit/Math/TweenEase.h"
#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Toolkit/HeaderUtils/UserData.h"


/**
 * Get target value when TweenActionValue need to init.
 */
typedef float (*TweenActionValueOnGet)(void* target);

/**
 * Set target value when TweenActionValue already changed value in one frame.
 */
typedef void (*TweenActionValueOnSet)(void* target, float value);


/**
 * One value in TweenAction that can tween easing.
 */
typedef struct
{
    /**
     * The action will reach to this value that depends on isRelative.
     */
    float                 value;

    /**
     * The action start from this value.
     */
    float                 fromValue;

    /**
     * The final value action will reach to.
     */
    float                 toValue;

    /**
     * When TweenAction's target value need to be get.
     */
    TweenActionValueOnGet OnGet;

    /**
     * When TweenAction's target value need to be set.
     */
    TweenActionValueOnSet OnSet;

    /**
     * The value is relative or absolute, default true.
     */
    bool                  isRelative;

    /**
     * Default TweenEaseType_Linear.
     */
    TweenEaseType         easeType;
}
TweenActionValue;


/**
 * Can tween easing a list of TweenActionValues.
 */
typedef struct TweenAction TweenAction;

/**
 * When TweenAction finished callback.
 */
typedef void (*TweenActionOnComplete)(TweenAction* action);


struct TweenAction
{
    UserData                    userData[1];

    /**
     * Target execute TweenAction.
     */
    void*                       target;

    /**
     * The action running duration time.
     */
    float                       duration;

    /**
     * The action running current position in duration time.
     */
    float                       curTime;

    /**
     * Means action running in queue or immediately, default true.
     */
    bool                        isQueue;

    /**
     * TweenActionValue list changing concurrently.
     */
    ArrayList(TweenActionValue) actionValueList[1];

    /**
     * When action complete callback.
     */
    TweenActionOnComplete       OnComplete;
};


/**
 * Manage and control all TweenActions.
 */
struct ATween
{
    /**
     * Control by ATween when action completed.
     *
     * userData  : NULL
     * target    : NULL
     * isQueue   : true
     * duration  : 0
     * OnComplete: NULL
     */
    TweenAction*      (*GetAction)            (void);

    /**
     * Add TweenActon's TweenActionValue.
     */
    TweenActionValue* (*AddTweenActionValue)  (TweenAction* action);

    /**
     * Bind TweenActions to tweenID and running.
     * if tweenID is NULL will generate tweenID value.
     *
     * return tweenID.
     */
    void*             (*RunActions)           (Array(TweenAction*)* actions, void* tweenID);

    /**
     * Remove tweenID's all actions immediately, return false when tweenID not in use.
     * we can or not cleanup tweenID bound data for actions,
     * but when tweenID not in use then must cleanup for reuse memory
     */
    bool              (*TryRemoveAllActions)  (void* tweenID);

    /**
     * Complete tweenID's all actions immediately, return false when tweenID not in use.
     * if isFireOnComplete true will fire callback.
     */
    bool              (*TryCompleteAllActions)(void* tweenID, bool isFireOnComplete);

    /**
     * Find TweenAction in current or queue, and remove it.
     * if tweenID not in use return false.
     * if not found TweenAction return false.
     */
    bool              (*TryRemoveAction)      (void* tweenID, TweenAction* action);

    /**
     * Whether the tweenID has action in current or queue.
     */
    bool              (*HasAction)            (void* tweenID);

    /**
     * Called every frame by loop.
     */
    void              (*Update)               (float deltaSeconds);
};


extern struct ATween ATween[1];


#endif
