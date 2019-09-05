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
 * Since    : 2017-4-23
 * Update   : 2019-1-17
 * Author   : scott.cgi
 */


#include <stdlib.h>
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/ArrayIntSet.h"


/**
 * Search index of key, if negative not found then return "-insertIndex - 1",
 * so insert index is "-BinarySearch() - 1".
 */
static inline int BinarySearch(ArrayList(intptr_t)* elementList, intptr_t key)
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
        guess            = (unsigned int) (high + low) >> 1;
        intptr_t element = AArrayList_Get(elementList, guess, intptr_t);

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
            // find the key, the guess is positive value
            return guess;
        }
    }

    // if guess == high
    // the guess is bigger than key index and insert at guess

    if (guess == low)
    {
        // the guess is smaller than key index and insert behind,
        // or if list empty then guess is -1, also do this make guess at 0
        ++guess;
    }

    // when list empty guess is 0, so we -1 make sure return negative value
    return -guess - 1;
}


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
    ArrayIntSet* arrayIntSet = malloc(sizeof(ArrayIntSet));
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


static bool TryAdd(ArrayIntSet* arrayIntSet, intptr_t element)
{
    int guess = BinarySearch(arrayIntSet->elementList, element);

    if (guess < 0)
    {
        AArrayList_Insert(arrayIntSet->elementList, -guess - 1, element);
        return true;
    }
    else
    {
        return false;
    }
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
{{
    Create,
    Init,
    CreateWithCapacity,
    InitWithCapacity,
    Release,
    TryAdd,
    TryRemove,
    IsContains,
    Clear,
}};