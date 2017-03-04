//
// Created by scott.cgi on 2016-11-13.
//

#ifndef coroutine_h
#define coroutine_h

#include "Mojoc/Toolkit/Utils/ArrayList.h"

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
    int                   step;

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
     * Coroutine wait types
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
    void       (*Update)        (float deltaTime);
};

extern struct ACoroutine ACoroutine[1];


#define ACoroutineAddParam(coroutine, value)             \
    AArrayListAdd(coroutine->params, value)


/**
 * return value
 */
#define ACoroutineGetParam(coroutine, index, type)       \
    AArrayListGet(coroutine->params, index, type)


/**
 * return valuePtr
 */
#define ACoroutineGetPtrParam(coroutine, index, type)    \
    AArrayListGetPtr(coroutine->params, index, type)



#define ACoroutineBegin()                                \
    switch (coroutine->step)                             \
    {                                                    \
         case 0:                                         \
             coroutine->state = coroutine_state_running


#define ACoroutineEnd()                                  \
    }                                                    \
    coroutine->state = coroutine_state_finish            \



#define ACoroutineYieldFrame(waitFrameCount)             \
    coroutine->waitValue    = waitFrameCount;            \
    coroutine->curWaitValue = 0.0f;                      \
    coroutine->waitType     = coroutine_wait_frame;      \
    coroutine->step         = __LINE__;                  \
    return;                                              \
    case __LINE__:                                       \


#define ACoroutineYieldSecond(waitSecond)                \
    coroutine->waitValue    = waitSecond;                \
    coroutine->curWaitValue = 0.0f;                      \
    coroutine->waitType     = coroutine_wait_second;     \
    coroutine->step         = __LINE__;                  \
    return;                                              \
    case __LINE__:                                       \


#define ACoroutineYieldCoroutine(waitCoroutine)          \
    coroutine->waitValue    = 0.0f;                      \
    coroutine->curWaitValue = 0.0f;                      \
    coroutine->waitType     = coroutine_wait_coroutine;  \
    AArrayListAdd((waitCoroutine)->waits, coroutine);    \
    coroutine->step         = __LINE__;                  \
    return;                                              \
    case __LINE__:                                       \


#define ACoroutineYieldBreak()                           \
    coroutine->state = coroutine_state_finish;           \
    return                                               \

#endif
