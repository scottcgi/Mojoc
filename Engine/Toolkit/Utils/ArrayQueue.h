/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2015-6-5
 * Update: 2019-1-17
 * Author: scott.cgi
 */

#ifndef ARRAY_QUEUE_H
#define  ARRAY_QUEUE_H


#include "Engine/Toolkit/Utils/ArrayList.h"


/**
 * The queue implementation by array.
 */
typedef struct
{
    /**
     * ArrayQueue top element index.
     */
    int       topIndex;

    /**
     * Stores all elements.
     */
    ArrayList elementList[1];
}
ArrayQueue;


/**
 * Control ArrayQueue.
 */
struct AArrayQueue
{
    ArrayQueue* (*Create)            (int elementTypeSize);
    void        (*Init)              (int elementTypeSize, ArrayQueue* outArrayQueue);

    ArrayQueue* (*CreateWithCapacity)(int elementTypeSize, int capacity);
    void        (*InitWithCapacity)  (int elementTypeSize, int capacity, ArrayQueue* outArrayQueue);

    void        (*Release)           (ArrayQueue* arrayQueue);

    /**
     * Push element from elementPtr into ArrayQueue.
     * elementPtr: point to element
     *
     * return elementPtr in ArrayQueue.
     */
    void*       (*Push)              (ArrayQueue* arrayQueue, void* elementPtr);

    /**
     * Pop element from ArrayQueue.
     * return top elementPtr in ArrayQueue, if no element return defaultElementPtr.
     */
    void*       (*Pop)               (ArrayQueue* arrayQueue, void* defaultElementPtr);

    /**
     * Remove element at index that range in [topIndex, ArrayQueue size - 1].
     */
    void        (*RemoveAt)          (ArrayQueue* arrayQueue, int   index);

    /**
     * Clear all elements, and reset topIndex to 0.
     */
    void        (*Clear)             (ArrayQueue* arrayQueue);
};


extern struct AArrayQueue AArrayQueue[1];


/**
 * Marked ArrayQueue element type.
 * elementType: element data type
 */
#define ArrayQueue(elementType) ArrayQueue


/**
 * Init constant ArrayQueue.
 * example: ArrayQueue queue[1] = AArrayQueue_Init(elementType, increase)
 */
#define AArrayQueue_Init(elementType, increase)   \
    {                                            \
        0,                                       \
        AArrayList_Init(elementType, increase),  \
    }


/**
 * Shortcut of AArrayQueue->Push.
 */
#define AArrayQueue_Push(arrayQueue, element) \
    AArrayQueue->Push(arrayQueue, &(element))


/**
 * Shortcut of AArrayQueue->Pop.
 * return element.
 */
#define AArrayQueue_Pop(arrayQueue, elementType) \
    (*(elementType*) AArrayQueue->Pop(arrayQueue, NULL_PTR))


/**
 * Shortcut of AArrayQueue->Pop.
 * return element.
 */
#define AArrayQueue_PopWithDefault(arrayQueue, elementType, defaultValue) \
    (*(elementType*) AArrayQueue->Pop(arrayQueue, &(defaultValue)))


/**
 * Shortcut of AArrayQueue->Pop.
 * return elementPtr.
 */
#define AArrayQueue_PopPtr(arrayQueue, elementType) \
    ((elementType*) AArrayQueue->Pop(arrayQueue, NULL))


/**
 * Shortcut of AArrayQueue->Pop.
 * return elementPtr.
 */
#define AArrayQueue_PopPtrWithDefault(arrayQueue, elementType, defaultValue) \
    ((elementType*) AArrayQueue->Pop(arrayQueue, &(defaultValue)))


#endif
