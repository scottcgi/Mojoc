/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2015-6-5
 * Author : scott.cgi
 */

#include <stdlib.h>
#include "Engine/Toolkit/Utils/ArrayQueue.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "ArrayList.h"


static void* Push(ArrayQueue* arrayQueue, void* elementPtr)
{
	if (arrayQueue->topIndex > 0 && arrayQueue->topIndex == arrayQueue->elementList->elementArray->length)
	{
        AArrayList->RemoveRange(arrayQueue->elementList, 0, arrayQueue->topIndex - 1);
        arrayQueue->topIndex = 0;
	}

	return AArrayList->Add(arrayQueue->elementList, elementPtr);
}


static void* Pop(ArrayQueue* arrayQueue, void* defaultElementPtr)
{
	if (arrayQueue->topIndex == arrayQueue->elementList->size)
	{
		return defaultElementPtr;
	}

	return (char*) arrayQueue->elementList->elementArray->data + arrayQueue->elementList->typeSize * (arrayQueue->topIndex++);
}


static void RemoveAt(ArrayQueue* arrayQueue, int index)
{
	ALogA
	(
		index >= arrayQueue->topIndex && index < arrayQueue->elementList->size,
		"popIndex index = %d, out of range [%d, %d]",
		index, arrayQueue->topIndex, arrayQueue->elementList->size - 1
	);

	AArrayList->Remove(arrayQueue->elementList, index);
}


static void Release(ArrayQueue* arrayQueue)
{
	arrayQueue->topIndex = 0;
	AArrayList->Release(arrayQueue->elementList);
}


static void InitWithCapacity(int typeSize, int capacity, ArrayQueue* outArrayQueue)
{
	if (capacity == 0)
	{
		AArrayList->Init(typeSize, outArrayQueue->elementList);
	}
	else
	{
		AArrayList->InitWithCapacity(typeSize, capacity, outArrayQueue->elementList);
	}

	outArrayQueue->topIndex = 0;
}


static ArrayQueue* CreateWithCapacity(int typeSize, int capacity)
{
	ArrayQueue* arrayQueue = (ArrayQueue*) malloc(sizeof(ArrayQueue));
	InitWithCapacity(typeSize, capacity, arrayQueue);

	return arrayQueue;
}


static void Init(int typeSize, ArrayQueue* outArrayQueue)
{
	InitWithCapacity(typeSize, 0, outArrayQueue);
}


static ArrayQueue* Create(int typeSize)
{
	return CreateWithCapacity(typeSize, 0);
}


static void Clear(ArrayQueue* arrayQueue)
{
	arrayQueue->topIndex = 0;
    AArrayList->Clear(arrayQueue->elementList);
}


struct AArrayQueue AArrayQueue[1] =
{
	Create,
	Init,
	CreateWithCapacity,
	InitWithCapacity,
	Release,

	Push,
	Pop,
	RemoveAt,
    Clear,
};
