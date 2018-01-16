/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2015-6-5
 * Author: scott.cgi
 */

#include <stdlib.h>
#include "Engine/Toolkit/Utils/ArrayQueue.h"
#include "Engine/Toolkit/Platform/Log.h"


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

    return (char*) arrayQueue->elementList->elementArray->data +
                   arrayQueue->elementList->elementTypeSize    *
                   (arrayQueue->topIndex++);
}


static void RemoveAt(ArrayQueue* arrayQueue, int index)
{
    ALog_A
    (
        index >= arrayQueue->topIndex && index < arrayQueue->elementList->size,
        "AArrayQueue RemoveAt index = %d, out of range [%d, %d]",
        index, arrayQueue->topIndex, arrayQueue->elementList->size - 1
    );

    AArrayList->Remove(arrayQueue->elementList, index);
}


static void Release(ArrayQueue* arrayQueue)
{
    arrayQueue->topIndex = 0;
    AArrayList->Release(arrayQueue->elementList);
}


static void InitWithCapacity(int elementTypeSize, int capacity, ArrayQueue* outArrayQueue)
{
    if (capacity == 0)
    {
        AArrayList->Init(elementTypeSize, outArrayQueue->elementList);
    }
    else
    {
        AArrayList->InitWithCapacity(elementTypeSize, capacity, outArrayQueue->elementList);
    }

    outArrayQueue->topIndex = 0;
}


static ArrayQueue* CreateWithCapacity(int elementTypeSize, int capacity)
{
    ArrayQueue* arrayQueue = (ArrayQueue*) malloc(sizeof(ArrayQueue));
    InitWithCapacity(elementTypeSize, capacity, arrayQueue);

    return arrayQueue;
}


static void Init(int elementTypeSize, ArrayQueue* outArrayQueue)
{
    InitWithCapacity(elementTypeSize, 0, outArrayQueue);
}


static ArrayQueue* Create(int elementTypeSize)
{
    return CreateWithCapacity(elementTypeSize, 0);
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
