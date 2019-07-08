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


#ifndef ARRAY_INT_SET_H
#define ARRAY_INT_SET_H


#include <stdbool.h>
#include <stdint.h>
#include "Engine/Toolkit/Utils/ArrayList.h"


/**
 * A set of elements that not repeated and ordered by element's int value.
 * element type must compatible with integer.
 */
typedef struct
{
    /**
     * Store all Elements.
     */
    ArrayList(intptr_t) elementList[1];
}
ArrayIntSet;


/**
 * Control ArrayIntSet.
 */
struct AArrayIntSet
{
    ArrayIntSet* (*Create)            (void);
    void         (*Init)              (ArrayIntSet* outArrayIntSet);

    ArrayIntSet* (*CreateWithCapacity)(int capacity);
    void         (*InitWithCapacity)  (int capacity, ArrayIntSet* outArrayIntSet);

    void         (*Release)           (ArrayIntSet* arrayIntSet);

    /**
     * Add no repeat element to ArrayIntSet.
     *
     * if element not exist in ArrayIntSet
     *    return true
     * else
     *    return false
     */
    bool         (*TryAdd)            (ArrayIntSet* arrayIntSet, intptr_t element);

    /**
     * If remove success return true else failed.
     */
    bool         (*TryRemove)         (ArrayIntSet* arrayIntSet, intptr_t element);

    /**
     * If element already in ArrayIntSet return true else not contains.
     */
    bool         (*IsContains)        (ArrayIntSet* arrayIntSet, intptr_t element);

    /**
     * Clear all elements, reset size to 0, and keep memory space.
     */
    void         (*Clear)             (ArrayIntSet* arrayIntSet);
};


extern struct AArrayIntSet AArrayIntSet[1];


/**
 * Marked ArrayIntSet element type.
 * element type must compatible with integer.
 */
#define ArrayIntSet(ElementType) ArrayIntSet


/**
 * Init constant ArrayIntSet.
 * example: ArrayIntSet set[1] = AArrayIntSet_Init(ElementType, increase)
 */
#define AArrayIntSet_Init(ElementType, increase) \
    {                                            \
         AArrayList_Init(intptr_t, increase),    \
    }


#endif
