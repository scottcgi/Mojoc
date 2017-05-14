/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2017-4-23
 * Author: scott.cgi
 */

#include <stdlib.h>
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/ArrayIntSet.h"


/**
 * Search index of key, if negative not found then return "-insertIndex - 1"
 * so insert index is "-BinarySearch() - 1"
 */
static inline int BinarySearch(ArrayList* elementList, intptr_t key)
{
    int high  = elementList->size;
    int low   = -1;
    int guess = -1;

    while (high - low > 1)
    {
        // not consider int overflow
        guess            = (high + low) >> 1;
        intptr_t element = AArrayListGet(elementList, guess, intptr_t);

        if (element < key)
        {
            low  = guess;
        }
        else if (element > key)
        {
            high = guess;
        }
        else if (element == key)
        {
            // find the key
            return guess;
        }
    }

    // if guess == high the guess is bigger than key and insert at guess

    if (guess == low)
    {
        // the guess is smaller than key and insert behind
        // or if empty then guess is -1, also do this make guess at 0
        guess++;
    }

    // when empty guess is 0, so we -1 make sure return negative value
    return -guess - 1;
}


//--------------------------------------------------------------------------------------------------


static void Release(ArrayIntSet* arrayIntSet)
{
    AArrayList->Release(arrayIntSet->elementList);
}


static void InitWithCapacity(int capacity, ArrayIntSet* arrayIntSet)
{
    if (capacity == 0)
    {
        AArrayList->Init(sizeof(intptr_t), arrayIntSet->elementList);
    }
    else
    {
        AArrayList->InitWithCapacity(sizeof(intptr_t), capacity, arrayIntSet->elementList);
    }
}


static ArrayIntSet* CreateWithCapacity(int capacity)
{
    ArrayIntSet* arrayIntSet = (ArrayIntSet*) malloc(sizeof(ArrayIntSet));
    InitWithCapacity(capacity, arrayIntSet);

    return arrayIntSet;
}


static void Init(ArrayIntSet* arrayIntSet)
{
    InitWithCapacity(0, arrayIntSet);
}


static ArrayIntSet* Create()
{
    return CreateWithCapacity(0);
}


static void Add(ArrayIntSet* arrayIntSet, intptr_t element)
{
    int guess = BinarySearch(arrayIntSet->elementList, element);

    ALog_A(guess < 0, "ArrayIntSet put element = %zd, has already exist", element);

    AArrayListInsert(arrayIntSet->elementList, -guess - 1, element);
}


static bool TryRemove(ArrayIntSet* arrayIntSet, intptr_t element)
{
    int guess = BinarySearch(arrayIntSet->elementList, element);

    if (guess >= 0)
    {
        AArrayList->Remove(arrayIntSet->elementList, guess);
        return true;
    }

    return false;
}


static bool IsContains(ArrayIntSet* arrayIntSet, intptr_t element)
{
    return  BinarySearch(arrayIntSet->elementList, element) >= 0;
}


static void Clear(ArrayIntSet* arrayIntSet)
{
    AArrayList->Clear(arrayIntSet->elementList);
}


struct AArrayIntSet AArrayIntSet[1] =
{
    Create,
    Init,
    CreateWithCapacity,
    InitWithCapacity,
    Release,
    Add,
    TryRemove,
    IsContains,
    Clear,
};