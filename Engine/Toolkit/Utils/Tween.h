/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License.
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub.
 * This Mojoc game engine is a serious project with extreme code pursuit, and will continue to iterate.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2016-6-8
 * Update : 2019-1-8
 * Author : scott.cgi
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
     * Bind TweenActions to tweenId and running.
     * if tweenId is NULL will generate tweenId value.
     *
     * return tweenId.
     */
    void*             (*RunActions)           (Array(TweenAction*)* actions, void* tweenId);

    /**
     * Remove tweenId's all actions immediately, return false when tweenId not in use.
     * we can or not cleanup tweenId bound data for actions,
     * but when tweenId not in use then must cleanup for reuse memory
     */
    bool              (*TryRemoveAllActions)  (void* tweenId);

    /**
     * Complete tweenId's all actions immediately, return false when tweenId not in use.
     * if isFireOnComplete true will fire callback.
     */
    bool              (*TryCompleteAllActions)(void* tweenId, bool isFireOnComplete);

    /**
     * Find TweenAction in current or queue, and remove it.
     * if tweenId not in use return false.
     * if not found TweenAction return false.
     */
    bool              (*TryRemoveAction)      (void* tweenId, TweenAction* action);

    /**
     * Whether the tweenId has action in current or queue.
     */
    bool              (*HasAction)            (void* tweenId);

    /**
     * Called every frame by loop.
     */
    void              (*Update)               (float deltaSeconds);
};


extern struct ATween ATween[1];


#endif
