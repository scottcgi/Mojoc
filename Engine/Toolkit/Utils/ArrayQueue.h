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
 * Since    : 2015-6-5
 * Update   : 2019-1-17
 * Author   : scott.cgi
 */


#ifndef ARRAY_QUEUE_H
#define ARRAY_QUEUE_H


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
     * Store all elements.
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
 * ElementType: element data type
 */
#define ArrayQueue(ElementType) ArrayQueue


/**
 * Init constant ArrayQueue.
 * example: ArrayQueue queue[1] = AArrayQueue_Init(ElementType, increase)
 */
#define AArrayQueue_Init(ElementType, increase)  \
    {                                            \
        0,                                       \
        AArrayList_Init(ElementType, increase),  \
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
#define AArrayQueue_Pop(arrayQueue, ElementType) \
    (*(ElementType*) AArrayQueue->Pop(arrayQueue, NULL_PTR))


/**
 * Shortcut of AArrayQueue->Pop.
 * return element.
 */
#define AArrayQueue_PopWithDefault(arrayQueue, ElementType, defaultValue) \
    (*(ElementType*) AArrayQueue->Pop(arrayQueue, &(defaultValue)))


/**
 * Shortcut of AArrayQueue->Pop.
 * return elementPtr.
 */
#define AArrayQueue_PopPtr(arrayQueue, ElementType) \
    ((ElementType*) AArrayQueue->Pop(arrayQueue, NULL))


/**
 * Shortcut of AArrayQueue->Pop.
 * return elementPtr.
 */
#define AArrayQueue_PopPtrWithDefault(arrayQueue, ElementType, defaultValue) \
    ((ElementType*) AArrayQueue->Pop(arrayQueue, &(defaultValue)))


#endif
