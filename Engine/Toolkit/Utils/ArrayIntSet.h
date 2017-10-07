/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2017-4-23
 * Author : scott.cgi
 * Version: 0.0.0
 */

#ifndef ARRAY_INT_SET_H
#define ARRAY_INT_SET_H


#include <stdbool.h>
#include <stdint.h>
#include "Engine/Toolkit/Utils/ArrayList.h"


typedef struct
{
    /**
     * Hold all Elements, element type must compatible with integer
     */
    ArrayList(intptr_t) elementList[1];
}
ArrayIntSet;


struct AArrayIntSet
{
    ArrayIntSet* (*Create)            ();
    void         (*Init)              (ArrayIntSet* outArrayIntSet);

    ArrayIntSet* (*CreateWithCapacity)(int capacity);
    void         (*InitWithCapacity)  (int capacity, ArrayIntSet* outArrayIntSet);

    void         (*Release)           (ArrayIntSet* arrayIntSet);

    /**
     * Add no repeat element to ArrayIntSet
     *
     * if element not exist in ArrayIntSet
     *    return true
     * else
     *    return false
     */
    bool         (*TryAdd)            (ArrayIntSet* arrayIntSet, intptr_t element);

    /**
     * If return true remove success else failed
     */
    bool         (*TryRemove)         (ArrayIntSet* arrayIntSet, intptr_t element);

    /**
     * If return true element already in ArrayIntSet else not contains
     */
    bool         (*IsContains)        (ArrayIntSet* arrayIntSet, intptr_t element);

    /**
     * Clear all value, reset size 0, and keep memory space
     */
    void         (*Clear)             (ArrayIntSet* arrayIntSet);
};


extern struct AArrayIntSet AArrayIntSet[1];


/**
 * Marked ArrayIntSet element type
 * element type must compatible with integer
 */
#define ArrayIntSet(elementType) ArrayIntSet


/**
 * Initialize constant ArrayIntSet, element type must compatible with integer
 * use like ArrayIntSet set[1] = AArrayIntSet_Init(elementType, increase)
 */
#define AArrayIntSet_Init(elementType, increase) \
    {                                            \
        {                                        \
            AArrayList_Init(intptr_t, increase), \
        }                                        \
    }


#endif
