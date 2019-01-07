/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2019-1-3
 * Author: scott.cgi
 */

 
#ifndef ARRAY_STR_SET_H
#define  ARRAY_STR_SET_H


#include <stdbool.h>
#include <stdint.h>
#include "Engine/Toolkit/Utils/ArrayList.h"


typedef struct
{
    char* str;
    int   strLength;
}
ArrayStrSetElement;


typedef struct
{
    ArrayList(ArrayStrSetElement*) elementList[1];
}
ArrayStrSet;


struct AArrayStrSet
{
    ArrayStrSet* (*Create)            ();
    void         (*Init)              (ArrayStrSet* outArrayStrSet);

    ArrayStrSet* (*CreateWithCapacity)(int capacity);
    void         (*InitWithCapacity)  (int capacity, ArrayStrSet* outArrayStrSet);

    void         (*Release)           (ArrayStrSet* arrayStrSet);

    /**
     * Get str in ArrayStrSet, if str not found will add first then return.
     */
    char*        (*Get)               (ArrayStrSet* arrayStrSet, char* str);

    /**
     * If remove success
     *     return true
     * else
     *     not found str in ArrayStrSet
     */
    bool         (*TryRemove)         (ArrayStrSet* arrayStrSet, char* str);

    /**
     * If contains str
     *     return true
     * else
     *     not contains
     */
    bool         (*IsContains)        (ArrayStrSet* arrayStrSet, char* str);

    /**
     * Clear all values, reset size to 0, and keep memory space.
     */
    void         (*Clear)             (ArrayStrSet* arrayStrSet);
};


extern struct AArrayStrSet AArrayStrSet[1];


/**
 * Initialize constant ArrayStrSet.
 * Example: ArrayStrSet set[1] = ArrayStrSet_Init(increase)
 */
#define ArrayStrSet_Init(increase)                       \
    {{                                                  \
         AArrayList_Init(ArrayStrSetElement*, increase),\
    }}


#endif
