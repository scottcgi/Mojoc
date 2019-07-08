/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2013-2-4
 * Update : 2019-1-17
 * Author : scott.cgi
 */


#include <string.h>
#include <stdlib.h>
#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Toolkit/Platform/Log.h"


#define CheckIndex(tag, index)                                          \
    ALog_A                                                              \
    (                                                                   \
        ((index) < arrayList->size && (index) >= 0),                    \
        "AArrayList " tag " failed index error, index = %d, size = %d", \
        index,                                                          \
        arrayList->size                                                 \
    )                                                                    


#define CheckInsertIndex(tag)                                           \
    ALog_A                                                              \
    (                                                                   \
        ((index) <= arrayList->size && (index) >= 0),                   \
        "AArrayList " tag " failed index error, index = %d, size = %d", \
        index,                                                          \
        arrayList->size                                                 \
    )


static inline void AddCapacity(ArrayList* arrayList, int increase)
{
    ALog_A(increase > 0, "AArrayList AddCapacity failed, increase = %d cannot <= 0", increase);

    void* data = realloc
                 (
                     arrayList->elementArr->data,
                     (size_t) (increase + arrayList->elementArr->length) * arrayList->elementTypeSize
                 );

    ALog_A
    (
        data != NULL,
        "AArrayList AddCapacity failed, unable to realloc memory, size = %d, length = %d, increase = %d",
        arrayList->size, arrayList->elementArr->length, increase
    );

    arrayList->elementArr->data    = data;
    arrayList->elementArr->length += increase;
}


static void* GetAdd(ArrayList* arrayList)
{
    if (arrayList->size == arrayList->elementArr->length)
    {
        AddCapacity(arrayList, arrayList->increase);
    }

    return (char*) arrayList->elementArr->data + arrayList->elementTypeSize * (arrayList->size++);
}


static void* GetInsert(ArrayList* arrayList, int index)
{
    // insert index can equal size
    CheckInsertIndex("GetInsert");

    if (arrayList->size == arrayList->elementArr->length)
    {
        AddCapacity(arrayList, arrayList->increase);
    }

    void* from = (char*) arrayList->elementArr->data + arrayList->elementTypeSize * index;
    void* to   = (char*) from                        + arrayList->elementTypeSize;

    // from and to overlap so cannot use memcpy
    memmove(to, from, (size_t) arrayList->elementTypeSize * ((arrayList->size++) - index));

    return from;
}


static void* Add(ArrayList* arrayList, void* elementPtr)
{
    return memcpy(GetAdd(arrayList), elementPtr, (size_t) arrayList->elementTypeSize);
}


static void* Insert(ArrayList* arrayList, int index, void* elementPtr)
{
    return memcpy(GetInsert(arrayList, index), elementPtr, (size_t) arrayList->elementTypeSize);
}


/**
 * Use macro instead.
 * Redundancy is better so don't delete.
 */
static void* Get(ArrayList* arrayList, int index)
{
    CheckIndex("Get", index);
    return (char*) arrayList->elementArr->data + arrayList->elementTypeSize * index;
}


/**
 * Use macro instead.
 * Redundancy is better so don't delete.
 */
static void Set(ArrayList* arrayList, int index, void* elementPtr)
{
    CheckIndex("Set", index);
    memcpy
    (
        (char*)  arrayList->elementArr->data + arrayList->elementTypeSize * index,
        elementPtr,
        (size_t) arrayList->elementTypeSize
    );
}



static void* Pop(ArrayList* arrayList, void* defaultElementPtr)
{
    if (arrayList->size > 0)
    {
        // now size equal last index
        return (char*) arrayList->elementArr->data + arrayList->elementTypeSize * (--arrayList->size);
    }
    else
    {
        return defaultElementPtr;
    }
}


static void AddArray(ArrayList* arrayList, void* data, int length, int elementTypeSize)
{
    ALog_A
    (
        arrayList->elementTypeSize == elementTypeSize,
        "AArrayList AddArray arrayList elementTypeSize = %d must equal Array elementTypeSize = %d",
        arrayList->elementTypeSize,
        elementTypeSize
    );

    int n = arrayList->elementArr->length - arrayList->size - length;

    if (n < 0)
    {
        AddCapacity(arrayList, -n + arrayList->increase);
    }

    memcpy
    (
        (char*)  arrayList->elementArr->data + arrayList->elementTypeSize * arrayList->size,
        data,
        (size_t) arrayList->elementTypeSize * length
    );

    arrayList->size += length;
}


static void Remove(ArrayList* arrayList, int index)
{
    CheckIndex("Remove", index);

    // now size equal last index
    --arrayList->size;

    if (index != arrayList->size)
    {
        void* to   = (char*) arrayList->elementArr->data + arrayList->elementTypeSize * index;
        void* from = (char*) to                          + arrayList->elementTypeSize;

        // move between index and last index element
        memcpy(to, from,  (size_t) arrayList->elementTypeSize * (arrayList->size - index));
    }
}


static void RemoveRange(ArrayList* arrayList, int fromIndex, int toIndex)
{
    CheckIndex("RemoveRange", fromIndex);
    CheckIndex("RemoveRange", toIndex);
    ALog_A    (toIndex >=     fromIndex, "AArrayList RemoveRange toIndex must more than fromIndex");

    int num          = toIndex         - fromIndex + 1;
    int lastIndex    = arrayList->size - 1;

    arrayList->size -= num;

    if (toIndex != lastIndex)
    {
        void* to    = (char*) arrayList->elementArr->data + arrayList->elementTypeSize * fromIndex;
        void* from  = (char*) to                          + arrayList->elementTypeSize * num;

        // move between lastIndex and toIndex element
        memcpy(to, from, (size_t) arrayList->elementTypeSize * (lastIndex - toIndex));
    }
}


static void RemoveByLast(ArrayList* arrayList, int index)
{
    CheckIndex("RemoveByLast", index);

    // now size equal last index
    --arrayList->size;

    if (index != arrayList->size)
    {
        memcpy
        (
            (char*)  arrayList->elementArr->data + arrayList->elementTypeSize * index,
            (char*)  arrayList->elementArr->data + arrayList->elementTypeSize * arrayList->size,
            (size_t) arrayList->elementTypeSize
        );
    }
}


static void Clear(ArrayList* arrayList)
{
    arrayList->size = 0;
}


static void Shrink(ArrayList* arrayList)
{
    if (arrayList->size == 0)
    {
        free(arrayList->elementArr->data);
        arrayList->elementArr->data   = NULL;
        arrayList->elementArr->length = 0;
    }
    else
    {
        void* data = realloc(arrayList->elementArr->data, (size_t) arrayList->size * arrayList->elementTypeSize);
        ALog_A(data, "AArrayList Shrink error, size = %d ", arrayList->size);

        arrayList->elementArr->data   = data;
        arrayList->elementArr->length = arrayList->size;
    }
}


static void Release(ArrayList* arrayList)
{
    free(arrayList->elementArr->data);
    arrayList->elementArr->data   = NULL;
    arrayList->elementArr->length = 0;
    arrayList->size                 = 0;
}


static void SetSize(ArrayList* arrayList, int size)
{
    arrayList->size = size;

    if (arrayList->elementArr->length < size)
    {
        AddCapacity(arrayList, size - arrayList->elementArr->length);
    }
}


static void SetCapacity(ArrayList* arrayList, int capacity)
{
    if (arrayList->elementArr->length < capacity)
    {
        AddCapacity(arrayList, capacity - arrayList->elementArr->length);
    }
}


static inline void InitArrayList(int elementTypeSize, ArrayList* arrayList)
{
    arrayList->elementArr->data   = NULL;
    arrayList->elementArr->length = 0;
    arrayList->elementTypeSize    = elementTypeSize;
    arrayList->size               = 0;
    arrayList->increase           = 20;
}


static ArrayList* Create(int elementTypeSize)
{
    ArrayList* arrayList = malloc(sizeof(ArrayList));
    InitArrayList(elementTypeSize, arrayList);
    
    return arrayList;
}


static void init(int elementTypeSize, ArrayList* outArrayList)
{
    InitArrayList(elementTypeSize, outArrayList);
}


static ArrayList* CreateWithSize(int elementTypeSize, int size)
{
    ArrayList* arrayList = Create(elementTypeSize);
    SetSize(arrayList, size);

    return arrayList;
}


static void InitWithSize(int elementTypeSize, int size, ArrayList* outArrayList)
{
    InitArrayList(elementTypeSize, outArrayList);
    SetSize(outArrayList, size);
}


static ArrayList* CreateWithCapacity(int elementTypeSize, int capacity)
{
    ArrayList* arrayList = Create(elementTypeSize);
    SetCapacity(arrayList, capacity);
    
    return arrayList;
}


static void InitWithCapacity(int elementTypeSize, int capacity, ArrayList* outArrayList)
{
    InitArrayList(elementTypeSize, outArrayList);
    SetCapacity(outArrayList, capacity);
}


struct AArrayList AArrayList[1] =
{
    Create,
    init,
    CreateWithSize,
    InitWithSize,
    CreateWithCapacity,
    InitWithCapacity,

    Release,

    GetAdd,
    GetInsert,
    Add,
    Insert,
    Pop,
    AddArray,
    Remove,
    RemoveRange,
    RemoveByLast,
    Clear,
    Shrink,
    SetSize,
    SetCapacity,
};


#undef CheckIndex
#undef CheckInsertIndex
