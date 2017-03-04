/*
 *
 *  Created on: 2015-6-5
 *      Author: scott.cgi
 */

#include <malloc.h>

#include "Mojoc/Toolkit/Utils/ArrayQueue.h"
#include "Mojoc/Toolkit/Platform/Log.h"


static void* Push(ArrayQueue* arrayQueue, void* elementPtr)
{
	if (arrayQueue->topIndex > 0 && arrayQueue->topIndex == arrayQueue->arrayList->array->length)
	{
        AArrayList->RemoveRange(arrayQueue->arrayList, 0, arrayQueue->topIndex - 1);
        arrayQueue->topIndex = 0;
	}

	return AArrayList->Add(arrayQueue->arrayList, elementPtr);
}

static void* Pop(ArrayQueue* arrayQueue, void* defaultValuePtr)
{
	if (arrayQueue->topIndex == arrayQueue->arrayList->size)
	{
		return defaultValuePtr;
	}

	return (char*) arrayQueue->arrayList->array->data + arrayQueue->arrayList->typeSize * (arrayQueue->topIndex++);
}

static void RemoveAt(ArrayQueue* arrayQueue, int index)
{
	ALogA
	(
		index >= arrayQueue->topIndex && index < arrayQueue->arrayList->size,
		"popIndex index = %d, out of range [%d, %d]",
		index, arrayQueue->topIndex, arrayQueue->arrayList->size - 1
	);

	AArrayList->Remove(arrayQueue->arrayList, index);
}

static void Release(ArrayQueue* arrayQueue)
{
	arrayQueue->topIndex = 0;
	AArrayList->Release(arrayQueue->arrayList);
}

static void InitWithCapacity(int typeSize, int capacity, ArrayQueue* outArrayQueue)
{
	if (capacity == 0)
	{
		AArrayList->Init(typeSize, outArrayQueue->arrayList);
	}
	else
	{
		AArrayList->InitWithCapacity(typeSize, capacity, outArrayQueue->arrayList);
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
    AArrayList->Clear(arrayQueue->arrayList);
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
