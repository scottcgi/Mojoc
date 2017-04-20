/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2016-11-13
 * Author : scott.cgi
 */

#ifndef coroutine_h
#define coroutine_h


#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Toolkit/Head/MacroDefine.h"


typedef enum
{
    /**
     * Coroutine wait for frame count to waitValue
     */
    coroutine_wait_frame,

    /**
     * Coroutine wait for second count to waitValue
     */
    coroutine_wait_second,

    /**
     * Coroutine wait for other Coroutine to finish
     */
    coroutine_wait_coroutine,

    /**
     * Coroutine just run forward
     */
    coroutine_wait_null,
}
CoroutineWaitType;


typedef enum
{
    /**
     * Coroutine enter queue ready to running
     */
    coroutine_state_ready,

    /**
     * Coroutine has started to execute
     */
    coroutine_state_running,

    /**
     * Coroutine already finished and waiting for reuse
     */
    coroutine_state_finish,
}
CoroutineState;


typedef struct Coroutine Coroutine;
typedef void   (*CoroutineRun)(Coroutine* coroutine);


struct Coroutine
{
    /**
     * Record coroutine run step
     */
    void*                 step;

    /**
     * Coroutine implement function
     */
    CoroutineRun          Run;

    /**
     * Coroutine current state
     */
    CoroutineState        state;

    /**
     * Coroutine wait value to execute
     */
    float                 waitValue;

    /**
     * Record wait progress
     */
    float                 curWaitValue;

    /**
     * Coroutine wait type
     */
    CoroutineWaitType     waitType;

    /**
     * Hold params for CoroutineRun to get
     * when coroutine finish clear but the param create memory control yourself
     */
    ArrayList(void*)      params[1];

    /**
     * Hold Coroutines wait for this Coroutine to finish
     */
    ArrayList(Coroutine*) waits [1];
};


struct ACoroutine
{
    /**
     * Bind CoroutineRun with Coroutine and enter queue ready to run
     */
    Coroutine* (*StartCoroutine)(CoroutineRun Run);

    /**
     * Update on every frame
     */
    void       (*Update)        (float deltaSeconds);
};


extern struct ACoroutine ACoroutine[1];


#define ACoroutineAddParam(coroutine, value) \
    AArrayListAdd(coroutine->params, value)


/**
 * Get param value
 */
#define ACoroutineGetParam(coroutine, index, type)       \
    AArrayListGet(coroutine->params, index, type)


/**
 * Get param valuePtr
 */
#define ACoroutineGetPtrParam(coroutine, index, type)    \
    AArrayListGetPtr(coroutine->params, index, type)


/**
 * Construct goto label with line number
 */
#define _ACoroutineLabel(line) label##line
#define  ACoroutineLabel(line) _ACoroutineLabel(line)


#define ACoroutineBegin()                      \
    if (coroutine->step != NULL)               \
    {                                          \
        goto *coroutine->step;                 \
    }                                          \
    coroutine->state = coroutine_state_running \


#define ACoroutineEnd() \
    coroutine->state = coroutine_state_finish


/**
 * Called between ACoroutineBegin and ACoroutineEnd
 *
 * waitFrameCount: CoroutineRun wait frames and running again
 */
#define ACoroutineYieldFrame(waitFrames)                   \
    coroutine->waitValue    = waitFrames;                  \
    coroutine->curWaitValue = 0.0f;                        \
    coroutine->waitType     = coroutine_wait_frame;        \
    coroutine->step         = &&ACoroutineLabel(__LINE__); \
    return;                                                \
    ACoroutineLabel(__LINE__):


/**
 * Called between ACoroutineBegin and ACoroutineEnd
 *
 * waitSecond: CoroutineRun wait seconds and running again
 */
#define ACoroutineYieldSecond(waitSeconds)                 \
    coroutine->waitValue    = waitSeconds;                 \
    coroutine->curWaitValue = 0.0f;                        \
    coroutine->waitType     = coroutine_wait_second;       \
    coroutine->step         = &&ACoroutineLabel(__LINE__); \
    return;                                                \
    ACoroutineLabel(__LINE__):


/**
 * Called between ACoroutineBegin and ACoroutineEnd
 *
 * waitCoroutine: CoroutineRun wait other Coroutine finished and running again
 */
#define ACoroutineYieldCoroutine(waitCoroutine)            \
    coroutine->waitValue    = 0.0f;                        \
    coroutine->curWaitValue = 0.0f;                        \
    coroutine->waitType     = coroutine_wait_coroutine;    \
    AArrayListAdd((waitCoroutine)->waits, coroutine);      \
    coroutine->step         = &&ACoroutineLabel(__LINE__); \
    return;                                                \
    ACoroutineLabel(__LINE__):


/**
 * Called between ACoroutineBegin and ACoroutineEnd
 * sotp coroutine running
 */
#define ACoroutineYieldBreak()                 \
    coroutine->state = coroutine_state_finish; \
    return


#endif
