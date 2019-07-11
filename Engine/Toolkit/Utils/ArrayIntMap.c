/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2013-5-20
 * Update   : 2019-1-17
 * Author   : scott.cgi
 */


#include <string.h>
#include <stdlib.h>
#include "Engine/Toolkit/Utils/ArrayIntMap.h"
#include "Engine/Toolkit/Platform/Log.h"


#define CheckIndex(tag)                                        \
    ALog_A                                                     \
    (                                                          \
        index >= 0 && index <  arrayIntMap->elementList->size, \
        "AArrayIntMap" tag "index = %d, size = %d, invalid",   \
        index,                                                 \
        arrayIntMap->elementList->size                         \
    )


#define CheckInsertIndex(tag)                                  \
    ALog_A                                                     \
    (                                                          \
        index >= 0 && index <= arrayIntMap->elementList->size, \
        "AArrayIntMap" tag "index = %d, size = %d, invalid",   \
        index,                                                 \
        arrayIntMap->elementList->size                         \
    )



/**
 * Search index of key, if negative not found then return "-insertIndex - 1",
 * so insert index is "-BinarySearch() - 1".
 */
static inline int BinarySearch(ArrayList* elementList, intptr_t key)
{
    int high  = elementList->size;
    int low   = -1;
    int guess = -1;

    while (high - low > 1) // prevent infinite loops
    {
        // (high + low) always positive, so convert to unsigned
        // then the '>>' is unsigned move right
        // so the overflow will be handled correctly
        // because sign bit shift to right and 0 will be added
        guess               = (unsigned int) (high + low) >> 1;
        intptr_t elementKey = AArrayList_Get(elementList, guess, ArrayIntMapElement*)->key;

        if (elementKey < key)
        {
            low  = guess;
        }
        else if (elementKey > key)
        {
            high = guess;
        }
        else if (elementKey == key)
        {
            // find the key, the guess is positive value
            return guess;
        }
    }

    // if guess == high
    // the guess is bigger than key index and insert value at guess

    if (guess == low)
    {
        // the guess is smaller than key index and insert value behind,
        // or if list empty then guess is -1, so do this make guess at 0
        ++guess;
    }

    // when list empty guess is 0, so we -1 make sure return negative value
    return -guess - 1;
}


static void* TryPut(ArrayIntMap* arrayIntMap, intptr_t key, void* valuePtr)
{
    int guess = BinarySearch(arrayIntMap->elementList, key);

    if (guess < 0)
    {
        ArrayIntMapElement* element = malloc(sizeof(ArrayIntMapElement) + arrayIntMap->valueTypeSize);
        element->key                = key;
        element->valuePtr           = (char*) element + sizeof(ArrayIntMapElement);

        AArrayList_Insert(arrayIntMap->elementList, -guess - 1, element);

        return memcpy(element->valuePtr, valuePtr, (size_t) arrayIntMap->valueTypeSize);
    }
    else
    {
        return NULL;
    }
}


static void* Get(ArrayIntMap* arrayIntMap, intptr_t key, void* defaultValuePtr)
{
    int guess = BinarySearch(arrayIntMap->elementList, key);

    return guess >= 0 ?
           AArrayList_Get(arrayIntMap->elementList, guess, ArrayIntMapElement*)->valuePtr : defaultValuePtr;
}


static void* TrySet(ArrayIntMap* arrayIntMap, intptr_t key, void* valuePtr)
{
    int guess = BinarySearch(arrayIntMap->elementList, key);

    if (guess >= 0)
    {
        return memcpy
               (
                   AArrayList_Get(arrayIntMap->elementList, guess, ArrayIntMapElement*)->valuePtr,
                   valuePtr,
                   (size_t) arrayIntMap->valueTypeSize
               );
    }
    else
    {
        return NULL;
    }
}


static bool TryRemove(ArrayIntMap* arrayIntMap, intptr_t key)
{
    int guess = BinarySearch(arrayIntMap->elementList, key);

    if (guess >= 0)
    {
        free
        (
            AArrayList_Get(arrayIntMap->elementList, guess, ArrayIntMapElement*)
        );

        AArrayList->Remove(arrayIntMap->elementList, guess);

        return true;
    }
    
    return false;
}


static void Clear(ArrayIntMap* arrayIntMap)
{
    for (int i = 0; i < arrayIntMap->elementList->size; ++i)
    {
        free
        (
            AArrayList_Get(arrayIntMap->elementList, i, ArrayIntMapElement*)
        );
    }

    AArrayList->Clear(arrayIntMap->elementList);
}


static void* InsertAt(ArrayIntMap* arrayIntMap, intptr_t key, int index, void* valuePtr)
{
    CheckInsertIndex("InsertAt");

    ArrayIntMapElement* element = malloc(sizeof(ArrayIntMapElement) + arrayIntMap->valueTypeSize);
    element->key                = key;
    element->valuePtr           = (char*) element + sizeof(ArrayIntMapElement);
    AArrayList_Insert(arrayIntMap->elementList, index, element);

    return memcpy(element->valuePtr, valuePtr, (size_t) arrayIntMap->valueTypeSize);
}


static int GetIndex(ArrayIntMap* arrayIntMap, intptr_t key)
{
    return BinarySearch(arrayIntMap->elementList, key);
}


static intptr_t GetKey(ArrayIntMap* arrayIntMap, int index)
{
    CheckIndex("GetKey");
    return AArrayList_Get(arrayIntMap->elementList, index, ArrayIntMapElement*)->key;
}


static void* GetAt(ArrayIntMap* arrayIntMap, int index)
{
    CheckIndex("GetAt");
    return AArrayList_Get(arrayIntMap->elementList, index, ArrayIntMapElement*)->valuePtr;
}


static void* SetAt(ArrayIntMap* arrayIntMap, int index, void* valuePtr)
{
    CheckIndex("SetAt");

    return memcpy
           (
               AArrayList_Get(arrayIntMap->elementList, index, ArrayIntMapElement*)->valuePtr,
               valuePtr,
               (size_t) arrayIntMap->valueTypeSize
           );
}


static void RemoveAt(ArrayIntMap* arrayIntMap, int index)
{
    CheckIndex("RemoveAt");

    free
    (
        AArrayList_Get(arrayIntMap->elementList, index, ArrayIntMapElement*)
    );

    AArrayList->Remove(arrayIntMap->elementList, index);
}


static void Release(ArrayIntMap* arrayIntMap)
{
    for (int i = 0; i < arrayIntMap->elementList->size; ++i)
    {
        free
        (
            AArrayList_Get(arrayIntMap->elementList, i, ArrayIntMapElement*)
        );
    }

    AArrayList->Release(arrayIntMap->elementList);
}


static void InitWithCapacity(int valueTypeSize, int capacity, ArrayIntMap* outArrayIntMap)
{
    if (capacity == 0)
    {
        AArrayList->Init(sizeof(ArrayIntMapElement*), outArrayIntMap->elementList);
    }
    else
    {
        AArrayList->InitWithCapacity(sizeof(ArrayIntMapElement*), capacity, outArrayIntMap->elementList);
    }

    outArrayIntMap->valueTypeSize = valueTypeSize;
}


static ArrayIntMap* CreateWithCapacity(int valueTypeSize, int capacity)
{
    ArrayIntMap* arrayIntMap = malloc(sizeof(ArrayIntMap));
    InitWithCapacity(valueTypeSize, capacity, arrayIntMap);

    return arrayIntMap;
}


static void Init(int valueTypeSize, ArrayIntMap* outArrayIntMap)
{
    InitWithCapacity(valueTypeSize, 0, outArrayIntMap);
}


static ArrayIntMap* Create(int valueTypeSize)
{
    return CreateWithCapacity(valueTypeSize, 0);
}


struct AArrayIntMap AArrayIntMap[1] =
{{
    Create,
    Init,
    CreateWithCapacity,
    InitWithCapacity,

    Release,

    TryPut,
    Get,
    TrySet,
    TryRemove,
    Clear,
    InsertAt,
    GetIndex,
    GetKey,
    GetAt,
    SetAt,
    RemoveAt,
}};


#undef CheckIndex
#undef CheckInsertIndex



