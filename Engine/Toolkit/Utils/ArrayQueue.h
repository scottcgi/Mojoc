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
     * ArrayQueue head element index.
     */
    int       headIndex;

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
     * Add the element from elementPtr to the tail of ArrayQueue.
     * elementPtr: point to element
     *
     * return the elementPtr in ArrayQueue.
     */
    void*       (*Enqueue)           (ArrayQueue* arrayQueue, void* elementPtr);

    /**
     * Remove the element from the head of ArrayQueue.
     * return the head elementPtr of the ArrayQueue, if no element return defaultElementPtr.
     */
    void*       (*Dequeue)           (ArrayQueue* arrayQueue, void* defaultElementPtr);

    /**
     * Remove the element at index that range in [headIndex, ArrayQueue size - 1].
     */
    void        (*RemoveAt)          (ArrayQueue* arrayQueue, int   index);

    /**
     * Clear all elements, and reset headIndex to 0.
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
    {                                           \
        0,                                      \
        AArrayList_Init(ElementType, increase), \
    }


/**
 * Shortcut of AArrayQueue->Enqueue.
 */
#define AArrayQueue_Enqueue(arrayQueue, element) \
    AArrayQueue->Enqueue(arrayQueue, &(element))


/**
 * Shortcut of AArrayQueue->Dequeue.
 * return element.
 */
#define AArrayQueue_Dequeue(arrayQueue, ElementType) \
    (*(ElementType*) AArrayQueue->Dequeue(arrayQueue, NULL_PTR))


/**
 * Shortcut of AArrayQueue->Dequeue.
 * return element.
 */
#define AArrayQueue_DequeueWithDefault(arrayQueue, ElementType, defaultValue) \
    (*(ElementType*) AArrayQueue->Dequeue(arrayQueue, &(defaultValue)))


/**
 * Shortcut of AArrayQueue->Dequeue.
 * return elementPtr.
 */
#define AArrayQueue_DequeuePtr(arrayQueue, ElementType) \
    ((ElementType*) AArrayQueue->Dequeue(arrayQueue, NULL))


/**
 * Shortcut of AArrayQueue->Dequeue.
 * return elementPtr.
 */
#define AArrayQueue_DequeuePtrWithDefault(arrayQueue, ElementType, defaultValue) \
    ((ElementType*) AArrayQueue->Dequeue(arrayQueue, &(defaultValue)))


#endif
