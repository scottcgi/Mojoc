/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License:
 * https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * Since : 2016-11-13
 * UPdate: 2019-18
 * Author: scott.cgi
 */

 
#ifndef COROUTINE_H
#define  COROUTINE_H


#include <Engine/Toolkit/HeaderUtils/UserData.h>
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


typedef struct Coroutine Coroutine;
typedef void   (*CoroutineRun)(Coroutine* coroutine);


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
     * Coroutine wait value to execute.
     */
    float                  waitValue;

    /**
     * Current wait progress.
     */
    float                  curWaitValue;

    /**
     * Coroutine wait type.
     */
    CoroutineWaitType     waitType;

    /**
     * Hold Coroutines wait for this Coroutine to finish.
     */
    ArrayList(Coroutine*) waits [1];
};


struct ACoroutine
{
    /**
     * Bind CoroutineRun to Coroutine and enter running queue.
     */
    Coroutine* (*StartCoroutine)(CoroutineRun Run);

    /**
     * Update on every frame by game loop.
     */
    void       (*Update)        (float deltaSeconds);
};


extern struct ACoroutine ACoroutine[1];


#define ACoroutine_Begin()                               \
    switch (coroutine->step)                            \
    {                                                   \
        case 0:                                         \
            coroutine->state = CoroutineState_Running


#define ACoroutine_End()                                 \
    }                                                   \
    coroutine->state = CoroutineState_Finish            \


/**
 * Called between ACoroutine_Begin and ACoroutine_End.
 * waitFrames: CoroutineRun wait frames and running again
 */
#define ACoroutine_YieldFrames(waitFrames)               \
    coroutine->waitValue    = waitFrames;               \
    coroutine->curWaitValue = 0.0f;                     \
    coroutine->waitType     = CoroutineWaitType_Frames; \
    coroutine->step         = __LINE__;                 \
    return;                                             \
    case __LINE__:


/**
 * Called between ACoroutine_Begin and ACoroutine_End.
 * waitSeconds: CoroutineRun wait seconds and running again
 */
#define ACoroutine_YieldSeconds(waitSeconds)             \
    coroutine->waitValue    = waitSeconds;              \
    coroutine->curWaitValue = 0.0f;                     \
    coroutine->waitType     = CoroutineWaitType_Seconds;\
    coroutine->step         = __LINE__;                 \
    return;                                             \
    case __LINE__:


/**
 * Called between ACoroutine_Begin and ACoroutine_End.
 * waitCoroutine: CoroutineRun wait other Coroutine finished and running again
 */
#define ACoroutine_YieldCoroutine(waitCoroutine)             \
    coroutine->waitValue    = 0.0f;                         \
    coroutine->curWaitValue = 0.0f;                         \
    coroutine->waitType     = CoroutineWaitType_Coroutines; \
    AArrayList_Add((waitCoroutine)->waits, coroutine);      \
    coroutine->step         = __LINE__;                     \
    return;                                                 \
    case __LINE__:


/**
 * Called between ACoroutine_Begin and ACoroutine_End.
 * Stop coroutine running and jump out ACoroutine_Begin and ACoroutine_End.
 */
#define ACoroutine_YieldBreak()                \
    coroutine->state = CoroutineState_Finish; \
    break


#endif
