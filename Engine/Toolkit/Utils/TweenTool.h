/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2016-6-24
 * Update: 2019-1-8
 * Author: scott.cgi
 */

 
#ifndef TWEEN_TOOL_H
#define TWEEN_TOOL_H


#include <stdbool.h>
#include "Engine/Toolkit/Utils/Tween.h"


/**
 * Get and Set methods for get and set TweenActionValue.
 */
typedef struct
{
    TweenActionValueOnGet OnGet;
    TweenActionValueOnSet OnSet;
}
TweenActionValueGetSet;


/**
 * Various TweenActionValueGetSets need to be implemented.
 */
struct ATweenActionValueGetSetImpl
{
    TweenActionValueGetSet moveX  [1];
    TweenActionValueGetSet moveY  [1];

    TweenActionValueGetSet scaleX [1];
    TweenActionValueGetSet scaleY [1];

    TweenActionValueGetSet rotateZ[1];
    TweenActionValueGetSet fadeTo [1];
};


/**
 * Application must implement TweenActionValue get and set method.
 */
extern struct ATweenActionValueGetSetImpl ATweenActionValueGetSetImpl[1];


/**
 * Easy-to-use for tween, and can use chain calls.
 * cannot use typedef ATweenTool to avoid conflict with struct ATweenTool ATweenTool.
 */
struct ATweenTool
{
    /**
     * Add one TweenAction in context for chain setting,
     * the TweenAction can add several TweenActionValues.
     */
    struct ATweenTool* (*AddAction)  (void);

    /**
     * Add action with no actionValue just through duration time then callback.
     */
    struct ATweenTool* (*AddInterval)(float duration);

//----------------------------------------------------------------------------------------------------------------------

    /* Add one TweenAction with TweenActionValues. */

    struct ATweenTool* (*AddMoveX)     (float moveX,   float duration);
    struct ATweenTool* (*AddMoveY)     (float moveY,   float duration);

    struct ATweenTool* (*AddMove2)     (
                                           float          moveX,
                                           float          moveY,
                                           float          duration,
                                           bool          isRelative,
                                           TweenEaseType easeType
                                       );

    struct ATweenTool* (*AddMoveSame2) (float move,    float duration, bool isRelative, TweenEaseType easeType);

    struct ATweenTool* (*AddScaleX)    (float scaleX,  float duration);
    struct ATweenTool* (*AddScaleY)    (float scaleY,  float duration);

    struct ATweenTool* (*AddScale2)    (
                                           float            scaleX,  
                                           float            scaleY,
                                           float            duration,
                                           bool            isRelative,
                                           TweenEaseType   easeType
                                       );
    struct ATweenTool* (*AddScaleSame2)(float scale,   float duration, bool isRelative, TweenEaseType  easeType);

    struct ATweenTool* (*AddRotateZ)   (float rotateZ, float duration);
    struct ATweenTool* (*AddFadeTo)    (float fadeTo,  float duration);

//----------------------------------------------------------------------------------------------------------------------

    /* Set TweenAction property for current context. */

    struct ATweenTool* (*SetDuration)      (float                 duration);
    struct ATweenTool* (*SetQueue)         (bool                  isQueue);
    struct ATweenTool* (*SetOnComplete)    (TweenActionOnComplete OnComplete);
    struct ATweenTool* (*SetTarget)        (void*                 target);

    struct ATweenTool* (*SetUserDataInt)   (int                   slotIndex, int   value);
    struct ATweenTool* (*SetUserDataFloat) (int                   slotIndex, float value);
    struct ATweenTool* (*SetUserDataPtr)   (int                   slotIndex, void* value);
    struct ATweenTool* (*SetUserDataString)(int                   slotIndex, char* value);

    /**
     * Get TweenAction in current context.
     */
    struct ATweenTool* (*GetAction)        (TweenAction**         outActionPtr);

//----------------------------------------------------------------------------------------------------------------------

    /* Set new TweenActionValue into context TweenAction. */

    struct ATweenTool* (*SetMoveX)     (float moveX);
    struct ATweenTool* (*SetMoveY)     (float moveY);

    struct ATweenTool* (*SetMove2)     (float moveX, float moveY,     bool isRelative, TweenEaseType easeType);
    struct ATweenTool* (*SetMoveSame2) (float move,  bool isRelative, TweenEaseType    easeType);

    struct ATweenTool* (*SetScaleX)    (float scaleX);
    struct ATweenTool* (*SetScaleY)    (float scaleY);

    struct ATweenTool* (*SetScale2)    (float scaleX,  float scaleY,    bool isRelative, TweenEaseType easeType);
    struct ATweenTool* (*SetScaleSame2)(float scale,   bool isRelative, TweenEaseType    easeType);

    struct ATweenTool* (*SetRotateZ)   (float rotateZ);
    struct ATweenTool* (*SetFadeTo)    (float fadeTo);

//----------------------------------------------------------------------------------------------------------------------

    /* Set TweenActionValue property for current context. */

    struct ATweenTool* (*SetRelative)(bool          isRelative);
    struct ATweenTool* (*SetEaseType)(TweenEaseType easeType);

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Run all actions in current context, and set all actions target if has actionValue.
     * use target be tweenId.
     */
    void               (*RunActions)(void* target);

    /**
     * Run all actions in current context, and all actions must set owner's target if has actionValue.
     * all different targets will in same queue, run step by step.
     * return tweenId.
     */
    void*              (*RunTargets)(void);
};


extern struct ATweenTool ATweenTool[1];


#endif
