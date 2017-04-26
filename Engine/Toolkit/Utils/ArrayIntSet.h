/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2017-4-23
 * Author: scott.cgi
 */

#ifndef array_int_set_h
#define array_int_set_h


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
     */
    void         (*Add)               (ArrayIntSet* arrayIntSet, intptr_t element);

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
 * use like ArrayIntSet set[1] = AArrayIntSetInit(elementType, increase)
 */
#define AArrayIntSetInit(elementType, increase) \
	{                                           \
		{                                       \
			AArrayListInit(intptr_t, increase), \
		}                                       \
	}


#endif
