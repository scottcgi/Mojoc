/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2015-6-5
 * Author : scott.cgi
 */

#ifndef array_queue_h
#define array_queue_h


#include "Engine/Toolkit/Utils/ArrayList.h"


typedef struct
{
    /**
     * ArrayQueue top element index
     */
    int       topIndex;

    /**
     * Holde all elements
     */
	ArrayList elementList[1];
}
ArrayQueue;


struct AArrayQueue
{
	ArrayQueue* (*Create)            (int elementTypeSize);
	void        (*Init)              (int elementTypeSize, ArrayQueue* outArrayQueue);

	ArrayQueue* (*CreateWithCapacity)(int elementTypeSize, int capacity);
	void        (*InitWithCapacity)  (int elementTypeSize, int capacity, ArrayQueue* outArrayQueue);

	void        (*Release)           (ArrayQueue* arrayQueue);

	/**
	 * Push element from elementPtr into ArrayQueue
	 *
	 * elementPtr: point to element
	 *
	 * return elementPtr in ArrayQueue
	 */
	void*       (*Push)              (ArrayQueue* arrayQueue, void* elementPtr);

	/**
	 * Pop element from ArrayQueue
	 * return top elementPtr in ArrayQueue, if no element return defaultElementPtr
	 */
	void*       (*Pop)               (ArrayQueue* arrayQueue, void* defaultElementPtr);

	/**
	 * Remove element at index range in [topIndex, ArrayQueue size - 1]
	 */
	void        (*RemoveAt)          (ArrayQueue* arrayQueue, int   index);

	/**
	 * Clear arrayList and reset topIndex to 0
	 */
	void        (*Clear)             (ArrayQueue* arrayQueue);
};


extern struct AArrayQueue AArrayQueue[1];


/**
 * Marked ArrayQueue element type
 *
 * elementType: element data type
 */
#define ArrayQueue(elementType) ArrayQueue


/**
 * Initialize constant ArrayQueue
 * use like ArrayQueue queue[1] = AArrayQueueInit(elementType, increase)
 */
#define AArrayQueueInit(elementType, increase)      \
	{                                        \
		{                                    \
			0,                               \
			AArrayListInit(elementType, increase),  \
		}                                    \
	}


/**
 * Shortcut of AArrayQueue->Push
 */
#define AArrayQueuePush(arrayQueue, element) \
	AArrayQueue->Push(arrayQueue, &(element))


/**
 * Shortcut of AArrayQueue->Pop
 * return element
 */
#define AArrayQueuePop(arrayQueue, elementType) \
	(*(elementType*) AArrayQueue->Pop(arrayQueue, null_ptr))


/**
 * Shortcut of AArrayQueue->Pop
 * return element
 */
#define AArrayQueuePopWithDefault(arrayQueue, elementType, defaultValue) \
	(*(elementType*) AArrayQueue->Pop(arrayQueue, &(defaultValue)))


/**
 * Shortcut of AArrayQueue->Pop
 * return elementPtr
 */
#define AArrayQueuePopPtr(arrayQueue, elementType) \
	((elementType*) AArrayQueue->Pop(arrayQueue, NULL))


/**
 * Shortcut of AArrayQueue->Pop
 * return elementPtr
 */
#define AArrayQueuePopPtrWithDefault(arrayQueue, elementType, defaultValue) \
	((elementType*) AArrayQueue->Pop(arrayQueue, &(defaultValue)))


#endif
