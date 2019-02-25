/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2019-1-3
 * Update: 2019-11
 * Author: scott.cgi
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
 * Init constant ArrayStrSet.
 * example: ArrayStrSet set[1] = ArrayStrSet_Init(increase)
 */
#define ArrayStrSet_Init(increase)                     \
    {                                                  \
        AArrayList_Init(ArrayStrSetElement*, increase),\
    }


#endif
