/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-5-20
 * Author: scott.cgi
 */

#include <string.h>
#include <stdlib.h>

#include "Engine/Toolkit/Utils/ArrayStrMap.h"
#include "Engine/Toolkit/Platform/Log.h"


#define CheckIndex(tag)                                         \
    ALog_A                                                     \
    (                                                          \
        index >= 0 && index < arrayStrMap->elementList->size,  \
        "AArrayStrMap " tag " index = %d, size = %d, invalid", \
        index,                                                 \
        arrayStrMap->elementList->size                         \
    )


#define CheckInsertIndex(tag)                                   \
    ALog_A                                                     \
    (                                                          \
        index >= 0 && index <= arrayStrMap->elementList->size, \
        "AArrayStrMap " tag " index = %d, size = %d, invalid", \
        index,                                                 \
        arrayStrMap->elementList->size                         \
    )


/**
 * Search index of key, if negative not found then return "-insertIndex - 1"
 * so insert index is "-BinarySearch() - 1"
 */
static inline int BinarySearch(ArrayList* elementList, char* key, int keyLength)
{
    int high  = elementList->size;
    int low   = -1;
    int guess = -1;

    while (high - low > 1)
    {
        // not consider int overflow
        guess                       = (high + low) >> 1;
        ArrayStrMapElement* element = AArrayList_Get(elementList, guess, ArrayStrMapElement*);

        if (element->keyLength < keyLength)
        {
            low  = guess;
        }
        else if (element->keyLength > keyLength)
        {
            high = guess;
        }
        else if (element->keyLength == keyLength)
        {
            int cmp = memcmp(element->key, key, keyLength);

            if (cmp < 0)
            {
                low  = guess;
            }
            else if (cmp > 0)
            {
                high = guess;
            }
            else if (cmp == 0)
            {
                // cmp 0 means find the key
                return guess;
            }
        }
     }

    // if guess == high the guess is bigger than key in ArrayStrMap and insert value at guess

    if (guess == low)
    {
        // the guess is smaller than key in ArrayStrMap and insert value behind
        // or if ArrayStrMap empty then guess is -1, also do this make guess at 0
        guess++;
    }

    // when ArrayStrMap empty guess is 0, so we -1 make sure return negative value
    return -guess - 1;
}


//----------------------------------------------------------------------------------------------------------------------


static void* TryPut(ArrayStrMap* arrayStrMap, char* key, void* valuePtr)
{
    int keyLength = (int) strlen(key) + 1;
    int guess     = BinarySearch(arrayStrMap->elementList, key, keyLength);

    if (guess < 0)
    {
        int                 valueTypeSize = arrayStrMap->valueTypeSize;
        ArrayStrMapElement* element       = (ArrayStrMapElement*)
                                            malloc(sizeof(ArrayStrMapElement) + valueTypeSize + keyLength);

        element->keyLength                = keyLength;
        element->valuePtr                 = (char*) element + sizeof(ArrayStrMapElement);
        element->key                      = (char*) element->valuePtr + valueTypeSize;
        memcpy((void*) element->key, key, keyLength);

        AArrayList_Insert(arrayStrMap->elementList, -guess - 1, element);

        return memcpy(element->valuePtr, valuePtr, valueTypeSize);
    }
    else
    {
        return NULL;
    }
}


static void* Get(ArrayStrMap* arrayStrMap, char* key, void* defaultValuePtr)
{
    int guess = BinarySearch(arrayStrMap->elementList, key, (int) strlen(key) + 1);

    return guess >= 0 ?
           AArrayList_Get(arrayStrMap->elementList, guess, ArrayStrMapElement*)->valuePtr : defaultValuePtr;
}


static void* TrySet(ArrayStrMap* arrayStrMap, char* key, void* valuePtr)
{
    int guess = BinarySearch(arrayStrMap->elementList, key, (int) strlen(key) + 1);

    if (guess >= 0)
    {
        return memcpy
               (
                   AArrayList_Get(arrayStrMap->elementList, guess, ArrayStrMapElement*)->valuePtr,
                   valuePtr,
                   arrayStrMap->valueTypeSize
               );
    }
    else
    {
        return NULL;
    }
}


static bool TryRemove(ArrayStrMap* arrayStrMap, char* key)
{
    int guess = BinarySearch(arrayStrMap->elementList, key, (int) strlen(key) + 1);

    if (guess >= 0)
    {
        free
        (
            AArrayList_Get(arrayStrMap->elementList, guess, ArrayStrMapElement*)
        );

        AArrayList->Remove(arrayStrMap->elementList, guess);

        return true;
    }

    return false;
}


static void Clear(ArrayStrMap* arrayStrMap)
{
    for (int i = 0; i < arrayStrMap->elementList->size; i++)
    {
        free
        (
            AArrayList_Get(arrayStrMap->elementList, i, ArrayStrMapElement*)
        );
    }

    AArrayList->Clear(arrayStrMap->elementList);
}


static void* InsertAt(ArrayStrMap* arrayStrMap, char* key, int index, void* valuePtr)
{
    CheckInsertIndex("InsertAt");

    int keyLength     = (int) strlen(key) + 1;
    int valueTypeSize = arrayStrMap->valueTypeSize;

    ArrayStrMapElement* element = (ArrayStrMapElement*) malloc(sizeof(ArrayStrMapElement) + valueTypeSize + keyLength);
    element->keyLength          = keyLength;
    element->valuePtr           = (char*) element + sizeof(ArrayStrMapElement);
    element->key                = (char*) element->valuePtr + valueTypeSize;
    memcpy((void*) element->key, key, keyLength);

    AArrayList_Insert( arrayStrMap->elementList, index, element);

    return memcpy(element->valuePtr, valuePtr, valueTypeSize);
}


static int GetIndex(ArrayStrMap* arrayStrMap, char* key)
{
    return BinarySearch(arrayStrMap->elementList, key, (int) strlen(key) + 1);
}


static char* GetKey(ArrayStrMap* arrayStrMap, int index)
{
    CheckIndex("GetKey");
    return AArrayList_Get(arrayStrMap->elementList, index, ArrayStrMapElement*)->key;
}


static void* GetAt(ArrayStrMap* arrayStrMap, int index)
{
    CheckIndex("GetAt");
    return AArrayList_Get(arrayStrMap->elementList, index, ArrayStrMapElement*)->valuePtr;
}


static void* SetAt(ArrayStrMap* arrayStrMap, int index, void* valuePtr)
{
    CheckIndex("SetAt");

    return memcpy
           (
               AArrayList_Get(arrayStrMap->elementList, index, ArrayStrMapElement*)->valuePtr,
               valuePtr,
               arrayStrMap->valueTypeSize
           );
}


static void RemoveAt(ArrayStrMap* arrayStrMap, int index)
{
    CheckIndex("RemoveAt");

    free
    (
        AArrayList_Get(arrayStrMap->elementList, index, ArrayStrMapElement*)
    );

    AArrayList->Remove(arrayStrMap->elementList, index);
}


static void Release(ArrayStrMap* arrayStrMap)
{
    for (int i = 0; i < arrayStrMap->elementList->size; i++)
    {
        free
        (
            AArrayList_Get(arrayStrMap->elementList, i, ArrayStrMapElement*)
        );
    }

    AArrayList->Release(arrayStrMap->elementList);
}


static void InitWithCapacity(int valueTypeSize, int capacity, ArrayStrMap* outArrayStrMap)
{
    if (capacity == 0)
    {
        AArrayList->Init(sizeof(ArrayStrMapElement*), outArrayStrMap->elementList);
    }
    else
    {
        AArrayList->InitWithCapacity(sizeof(ArrayStrMapElement*), capacity, outArrayStrMap->elementList);
    }

    outArrayStrMap->valueTypeSize = valueTypeSize;
}


static ArrayStrMap* CreateWithCapacity(int valueTypeSize, int capacity)
{
    ArrayStrMap* arrayStrMap = (ArrayStrMap*) malloc(sizeof(ArrayStrMap));
    InitWithCapacity(valueTypeSize, capacity, arrayStrMap);

    return arrayStrMap;
}


static void Init(int valueTypeSize, ArrayStrMap* outArrayStrMap)
{
    InitWithCapacity(valueTypeSize, 0, outArrayStrMap);
}


static ArrayStrMap* Create(int valueTypeSize)
{
    return CreateWithCapacity(valueTypeSize, 0);
}


struct AArrayStrMap AArrayStrMap[1] =
{
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
};


#undef CheckIndex
#undef CheckInsertIndex
