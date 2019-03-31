/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The project Mojoc is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2019-1-3
 * Update: 2019-1-14
 * Author: scott.cgi
 */


#include <stdlib.h>
#include <string.h>
#include "Engine/Toolkit/Utils/ArrayStrSet.h"


/**
 * Search index of key, if negative not found then return "-insertIndex - 1",
 * so insert index is "-BinarySearch() - 1".
 */
static inline int BinarySearch(ArrayList* elementList, const char* str, int strLength)
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
        guess                       = (unsigned int) (high + low) >> 1; // NOLINT(hicpp-signed-bitwise)
        ArrayStrSetElement* element = AArrayList_Get(elementList, guess, ArrayStrSetElement*);

        if (element->strLength < strLength)
        {
            low  = guess;
        }
        else if (element->strLength > strLength)
        {
            high = guess;
        }
        else if (element->strLength == strLength)
        {
            int cmp = memcmp(element->str, str, (size_t) strLength);

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
                // find the key, the guess is positive value
                return guess;
            }
        }
    }

    // if guess == high
    // the guess is bigger than key index and insert value at guess

    if (guess == low)
    {
        // the guess is smaller than key index and insert value behind
        // or if list empty then guess is -1, also do this make guess at 0
        ++guess;
    }

    // when list empty guess is 0, so we -1 make sure return negative value
    return -guess - 1;
}


static void Release(ArrayStrSet* arrayStrSet)
{
    for (int i = 0; i < arrayStrSet->elementList->size; ++i)
    {
        free(AArrayList_Get(arrayStrSet->elementList, i, ArrayStrSetElement*));
    }

    AArrayList->Release(arrayStrSet->elementList);
}


static void InitWithCapacity(int capacity, ArrayStrSet* arrayStrSet)
{
    if (capacity == 0)
    {
        AArrayList->Init(sizeof(intptr_t), arrayStrSet->elementList);
    }
    else
    {
        AArrayList->InitWithCapacity(sizeof(intptr_t), capacity, arrayStrSet->elementList);
    }
}


static ArrayStrSet* CreateWithCapacity(int capacity)
{
    ArrayStrSet* arrayStrSet = malloc(sizeof(ArrayStrSet));
    InitWithCapacity(capacity, arrayStrSet);
    return arrayStrSet;
}


static void Init(ArrayStrSet* arrayStrSet)
{
    InitWithCapacity(0, arrayStrSet);
}


static ArrayStrSet* Create()
{
    return CreateWithCapacity(0);
}


static const char* Get(ArrayStrSet* arrayStrSet, const char* str)
{
    int                 strLength = (int) strlen(str) + 1;
    int                 guess     = BinarySearch(arrayStrSet->elementList, str, strLength);
    ArrayStrSetElement* element;

    if (guess < 0)
    {
        element            = malloc(sizeof(ArrayStrSetElement) + strLength);
        element->strLength = strLength;
        element->str       = (char*) element + sizeof(ArrayStrSetElement);

        memcpy((void*) element->str, str, (size_t) strLength);
        AArrayList_Insert(arrayStrSet->elementList, -guess - 1, element);
    }
    else
    {
        element = AArrayList_Get(arrayStrSet->elementList, guess, ArrayStrSetElement*);
    }

    return element->str;
}


static bool TryRemove(ArrayStrSet* arrayStrSet, const char* str)
{
    int guess = BinarySearch(arrayStrSet->elementList, str, (int) strlen(str) + 1);

    if (guess >= 0)
    {
        free(AArrayList_Get(arrayStrSet->elementList, guess, ArrayStrSetElement*));
        AArrayList->Remove(arrayStrSet->elementList, guess);
        
        return true;
    }

    return false;
}


static bool IsContains(ArrayStrSet* arrayStrSet, const char* str)
{
    return BinarySearch(arrayStrSet->elementList, str, (int) strlen(str) + 1) >= 0;
}


static void Clear(ArrayStrSet* arrayStrSet)
{
    for (int i = 0; i < arrayStrSet->elementList->size; ++i)
    {
        free(AArrayList_Get(arrayStrSet->elementList, i, ArrayStrSetElement*));
    }

    AArrayList->Clear(arrayStrSet->elementList);
}


struct AArrayStrSet AArrayStrSet[1] =
{
    Create,
    Init,
    CreateWithCapacity,
    InitWithCapacity,
    Release,
    Get,
    TryRemove,
    IsContains,
    Clear,
};