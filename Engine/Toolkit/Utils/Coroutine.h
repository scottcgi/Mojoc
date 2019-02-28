/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2016-11-13
 * UPdate: 2019-18
 * Author: scott.cgi
 */

 
#ifndef COROUTINE_H
#define COROUTINE_H


#include "Engine/Toolkit/HeaderUtils/UserData.h"
#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Toolkit/HeaderUtils/Define.h"


typedef enum
{
    /**
     * Coroutine wait for frame count to waitValue.
     */
     CoroutineWaitType_Frames,

    /**
     * Coroutine wait for second count to waitValue.
     */
     CoroutineWaitType_Seconds,

    /**
     * Coroutine wait for other Coroutine to finish.
     */
     CoroutineWaitType_Coroutines,

    /**
     * Coroutine just run forward.
     */
     CoroutineWaitType_Null,
}
CoroutineWaitType;


typedef enum
{
    /**
     * Coroutine enter queue ready to running.
     */
     CoroutineState_Ready,

    /**
     * Coroutine already running.
     */
     CoroutineState_Running,

    /**
     * Coroutine already finished and waiting for reuse.
     */
     CoroutineState_Finish,
}
CoroutineState;


/**
 *  A Coroutine can suspend execution and wait for execution again.
 */
typedef struct Coroutine Coroutine;

/**
 * A Coroutine running function.
 */
typedef void (*CoroutineRun)(Coroutine* coroutine);


struct Coroutine
{
    UserData              userData[1];

    /**
     * Record coroutine run step.
     */
    int                   step;

    /**
     * Coroutine implement this function.
     */
    CoroutineRun          Run;

    /**
     * Coroutine current state.
     */
    CoroutineState        state;

    /**
     * Coroutine wait value to continue execute.
     */
    float                 waitValue;

    /**
     * Current wait progress.
     */
    float                 curWaitValue;

    /**
     * Coroutine wait type.
     */
    CoroutineWaitType     waitType;

    /**
     * Store Coroutines that wait for this Coroutine to finish.
     */
    ArrayList(Coroutine*) waits [1];
};


/**
 * Control all Coroutines.
 */
struct ACoroutine
{
    /**
     * Bind CoroutineRun to Coroutine and enter running queue.
     */
    Coroutine* (*StartCoroutine)(CoroutineRun Run);

    /**
     * Update on every frame by loop.
     */
    void       (*Update)        (float deltaSeconds);
};


extern struct ACoroutine ACoroutine[1];


/**
 * Mark the Coroutine area begin.
 */
#define ACoroutine_Begin()                              \
    switch (coroutine->step)                            \
    {                                                   \
        default:                                        \
            break;                                      \
        case 0:                                         \
            coroutine->state = CoroutineState_Running


/**
* Mark the Coroutine area end.
*/
#define ACoroutine_End()                                \
    }                                                   \
    coroutine->state = CoroutineState_Finish            \


/**
 * Yield execution and wait number of frames then execution again.
 * called between ACoroutine_Begin and ACoroutine_End.
 *
 * waitFrames: CoroutineRun wait frames and running again
 */
#define ACoroutine_YieldFrames(waitFrames)              \
    coroutine->waitValue    = waitFrames;               \
    coroutine->curWaitValue = 0.0f;                     \
    coroutine->waitType     = CoroutineWaitType_Frames; \
    coroutine->step         = __LINE__;                 \
    return;                                             \
    case __LINE__:


/**
 * Yield execution and wait number of seconds then execution again.
 * called between ACoroutine_Begin and ACoroutine_End.
 * 
 * waitSeconds: CoroutineRun wait seconds and running again
 */
#define ACoroutine_YieldSeconds(waitSeconds)            \
    coroutine->waitValue    = waitSeconds;              \
    coroutine->curWaitValue = 0.0f;                     \
    coroutine->waitType     = CoroutineWaitType_Seconds;\
    coroutine->step         = __LINE__;                 \
    return;                                             \
    case __LINE__:


/**
 * Yield execution and wait other Coroutine finished then execution again.
 * called between ACoroutine_Begin and ACoroutine_End.
 * 
 * waitCoroutine: CoroutineRun wait other Coroutine finished and running again
 */
#define ACoroutine_YieldCoroutine(waitCoroutine)            \
    coroutine->waitValue    = 0.0f;                         \
    coroutine->curWaitValue = 0.0f;                         \
    coroutine->waitType     = CoroutineWaitType_Coroutines; \
    AArrayList_Add((waitCoroutine)->waits, coroutine);      \
    coroutine->step         = __LINE__;                     \
    return;                                                 \
    case __LINE__:


/**
 * Just stop Coroutine running and jump out ACoroutine_Begin and ACoroutine_End.
 * called between ACoroutine_Begin and ACoroutine_End.
 */
#define ACoroutine_YieldBreak()               \
    coroutine->state = CoroutineState_Finish; \
    break


#endif
