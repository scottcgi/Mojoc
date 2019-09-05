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
 * Since    : 2019-1-3
 * Update   : 2019-11
 * Author   : scott.cgi
 */

 
#ifndef ARRAY_STR_SET_H
#define ARRAY_STR_SET_H


#include <stdbool.h>
#include <stdint.h>
#include "Engine/Toolkit/Utils/ArrayList.h"


/**
 * The actual element store in ArrayStrSet.
 */
typedef struct
{
    /**
     * The str data will copy into ArrayStrSetElement malloc space.
     */
    const char* str;
    
    /**
     * The length of str, include '\0'.
     */
    int         strLength;
}
ArrayStrSetElement;


/**
 * A set of elements that not repeated and ordered by element's str.
 */
typedef struct
{
    ArrayList(ArrayStrSetElement*) elementList[1];
}
ArrayStrSet;


/**
 * Control ArrayStrSet.
 */
struct AArrayStrSet
{
    ArrayStrSet* (*Create)            (void);
    void         (*Init)              (ArrayStrSet* outArrayStrSet);

    ArrayStrSet* (*CreateWithCapacity)(int capacity);
    void         (*InitWithCapacity)  (int capacity, ArrayStrSet* outArrayStrSet);

    void         (*Release)           (ArrayStrSet* arrayStrSet);

    /**
     * Get str in ArrayStrSet, if str not found will add and return.
     */
    const char*  (*Get)               (ArrayStrSet* arrayStrSet, const char* str);

    /**
     * If remove success
     *     return true
     * else
     *     not found str in ArrayStrSet
     */
    bool         (*TryRemove)         (ArrayStrSet* arrayStrSet, const char* str);

    /**
     * If contains str
     *     return true
     * else
     *     not contains
     */
    bool         (*IsContains)        (ArrayStrSet* arrayStrSet, const char* str);

    /**
     * Clear all elements, reset size to 0, and keep memory space.
     */
    void         (*Clear)             (ArrayStrSet* arrayStrSet);
};


extern struct AArrayStrSet AArrayStrSet[1];


/**
 * Marked ArrayStrSet element name.
 */
#define ArrayStrSet(elementName) ArrayStrSet


/**
 * Init constant ArrayStrSet.
 * example: ArrayStrSet set[1] = ArrayStrSet_Init(elementName, increase)
 */
#define ArrayStrSet_Init(elementName, increase)        \
    {                                                  \
        AArrayList_Init(ArrayStrSetElement*, increase),\
    }


#endif
