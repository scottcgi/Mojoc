/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2019-1-3
 * Author: scott.cgi
 */


#include <stdlib.h>
#include <string.h>

#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/ArrayStrSet.h"
#include "ArrayStrSet.h"


/**
 * Search index of key, if negative not found then return "-insertIndex - 1",
 * so insert index is "-BinarySearch() - 1".
 */
static inline int BinarySearch(ArrayList* elementList, char* str, int strLength)
{
    int high  = elementList->size;
    int low   = -1;
    int guess = -1;

    while (high - low > 1)
    {
        // not consider int overflow
        guess                       = (high + low) >> 1;
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
            int cmp = memcmp(element->str, str, strLength);

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
                // find the key
                return guess;
            }
        }
    }

    // if guess == high
    // the guess is bigger than key and insert value at guess

    if (guess == low)
    {
        // the guess is smaller than key and insert value behind
        // or if empty then guess is -1, also do this make guess at 0
        guess++;
    }

    // when empty guess is 0, so we -1 make sure return negative value
    return -guess - 1;
}


//----------------------------------------------------------------------------------------------------------------------


static void Release(ArrayStrSet* arrayStrSet)
{
    for (int i = 0; i < arrayStrSet->elementList->size; i++)
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
    ArrayStrSet* arrayStrSet = (ArrayStrSet*) malloc(sizeof(ArrayStrSet));
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


static char* Get(ArrayStrSet* arrayStrSet, char* str)
{
    int                 strLength = (int) strlen(str) + 1;
    int                 guess     = BinarySearch(arrayStrSet->elementList, str, strLength);
    ArrayStrSetElement* element;

    if (guess < 0)
    {
        element            = (ArrayStrSetElement*) malloc(sizeof(ArrayStrSetElement) + strLength);
        element->strLength = strLength;
        element->str       = (char*) element + sizeof(ArrayStrSetElement);

        memcpy((void*) element->str, str, strLength);
        AArrayList_Insert(arrayStrSet->elementList, -guess - 1, element);
    }
    else
    {
        element = AArrayList_Get(arrayStrSet->elementList, guess, ArrayStrSetElement*);
    }

    return element->str;
}


static bool TryRemove(ArrayStrSet* arrayStrSet, char* str)
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


static bool IsContains(ArrayStrSet* arrayStrSet, char* str)
{
    return BinarySearch(arrayStrSet->elementList, str, (int) strlen(str) + 1) >= 0;
}


static void Clear(ArrayStrSet* arrayStrSet)
{
    for (int i = 0; i < arrayStrSet->elementList->size; i++)
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