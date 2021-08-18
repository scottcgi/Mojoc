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


#include <stdlib.h>
#include "Engine/Toolkit/Utils/ArrayQueue.h"
#include "Engine/Toolkit/Platform/Log.h"


static void* Enqueue(ArrayQueue* arrayQueue, void* elementPtr)
{
    if (arrayQueue->headIndex > 0 && arrayQueue->headIndex == arrayQueue->elementList->elementArr->length)
    {
        AArrayList->RemoveRange(arrayQueue->elementList, 0, arrayQueue->headIndex - 1);
        arrayQueue->headIndex = 0;
    }

    return AArrayList->Add(arrayQueue->elementList, elementPtr);
}


static void* Dequeue(ArrayQueue* arrayQueue, void* defaultElementPtr)
{
    if (arrayQueue->headIndex == arrayQueue->elementList->size)
    {
        return defaultElementPtr;
    }

    return (char*) arrayQueue->elementList->elementArr->data +
                   arrayQueue->elementList->elementTypeSize  *
                   (arrayQueue->headIndex++);
}


static void RemoveAt(ArrayQueue* arrayQueue, int index)
{
    ALog_A
    (
        index >= arrayQueue->headIndex && index < arrayQueue->elementList->size,
        "AArrayQueue RemoveAt index = %d, out of range [%d, %d]",
        index, arrayQueue->headIndex, arrayQueue->elementList->size - 1
    );

    AArrayList->Remove(arrayQueue->elementList, index);
}


static void Release(ArrayQueue* arrayQueue)
{
    arrayQueue->headIndex = 0;
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

    outArrayQueue->headIndex = 0;
}


static ArrayQueue* CreateWithCapacity(int elementTypeSize, int capacity)
{
    ArrayQueue* arrayQueue = malloc(sizeof(ArrayQueue));
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
    arrayQueue->headIndex = 0;
    AArrayList->Clear(arrayQueue->elementList);
}


struct AArrayQueue AArrayQueue[1] =
{{
    Create,
    Init,
    CreateWithCapacity,
    InitWithCapacity,
    Release,

    Enqueue,
    Dequeue,
    RemoveAt,
    Clear,
}};
