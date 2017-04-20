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
     * Holde all elements
     */
	ArrayList arrayList[1];

	/**
	 * ArrayQueue top element index
	 */
	int       topIndex;
}
ArrayQueue;


struct AArrayQueue
{
	ArrayQueue* (*Create)            (int typeSize);
	void        (*Init)              (int typeSize, ArrayQueue* outArrayQueue);

	ArrayQueue* (*CreateWithCapacity)(int typeSize, int capacity);
	void        (*InitWithCapacity)  (int typeSize, int capacity, ArrayQueue* outArrayQueue);

	void        (*Release)           (ArrayQueue*   arrayQueue);

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


static inline void AArrayQueueSetIncrease(ArrayQueue* arrayQueue, int increase)
{
	arrayQueue->arrayList->increase = increase;
}


/**
 * Marked ArrayQueue element type
 *
 * type: element data type
 */
#define ArrayQueue(type) ArrayQueue


/**
 * Initialize constant ArrayQueue
 * use like ArrayQueue queue[1] = AArrayQueueInit(type, increase)
 */
#define AArrayQueueInit(type, increase)      \
	{                                        \
		{                                    \
			AArrayListInit(type, increase),  \
			0,                               \
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
#define AArrayQueuePop(arrayQueue, type) \
	(*(type*) AArrayQueue->Pop(arrayQueue, null_ptr))


/**
 * Shortcut of AArrayQueue->Pop
 * return element
 */
#define AArrayQueuePopWithDefault(arrayQueue, type, defaultValue) \
	(*(type*) AArrayQueue->Pop(arrayQueue, &(defaultValue)))


/**
 * Shortcut of AArrayQueue->Pop
 * return elementPtr
 */
#define AArrayQueuePopPtr(arrayQueue, type) \
	((type*) AArrayQueue->Pop(arrayQueue, NULL))


/**
 * Shortcut of AArrayQueue->Pop
 * return elementPtr
 */
#define AArrayQueuePopPtrWithDefault(arrayQueue, type, defaultValue) \
	((type*) AArrayQueue->Pop(arrayQueue, &(defaultValue)))


#endif
