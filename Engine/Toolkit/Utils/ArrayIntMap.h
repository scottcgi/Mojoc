/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-5-27
 * Author : scott.cgi
 * Version: 0.0.0
 */


#ifndef ARRAY_INT_MAP_H
#define ARRAY_INT_MAP_H


#include <stdbool.h>
#include <stdint.h>
#include "Engine/Toolkit/Utils/ArrayList.h"


typedef struct
{
    /**
     * Identity and search ArrayIntMap value
     */
    intptr_t key;

    /**
     * ArrayIntMap value pointer
     * value data copy into ArrayIntMapElement memory space
     */
    void*    valuePtr;
}
ArrayIntMapElement;


typedef struct
{
    /**
     * sizeof ArrayIntMap value type
     */
    int                            valueTypeSize;

    /**
     * Hold all ArrayIntMapElement
     */
    ArrayList(ArrayIntMapElement*) elementList[1];
}
ArrayIntMap;


struct AArrayIntMap
{
    ArrayIntMap* (*Create)            (int valueTypeSize);
    void         (*Init)              (int valueTypeSize, ArrayIntMap* outArrayIntMap);

    ArrayIntMap* (*CreateWithCapacity)(int valueTypeSize, int capacity);
    void         (*InitWithCapacity)  (int valueTypeSize, int capacity, ArrayIntMap* outArrayIntMap);

    void         (*Release)           (ArrayIntMap* arrayIntMap);

    /**
     * Put key and value from valuePtr into ArrayIntMap
     *
     * valuePtr: point to value, value data copy in ArrayIntMapElement
     *
     * if key not exist in ArrayIntMap
     *     return valuePtr in ArrayIntMap
     * else
     *     return NULL
     */
    void*        (*TryPut)            (ArrayIntMap* arrayIntMap, intptr_t key, void* valuePtr);


    /**
     * Get valuePtr by key, if no key found return defaultValutPtr
     */
    void*        (*Get)               (ArrayIntMap* arrayIntMap, intptr_t key, void* defaultValuePtr);


    /**
     * Set new value from valuePtr by key
     *
     * if key exist in ArrayIntMap
     *     return valuePtr in ArrayIntMap
     * else
     *     return NULL
     */
    void*        (*TrySet)            (ArrayIntMap* arrayIntMap, intptr_t key, void* valuePtr);


    /**
     * Remove value by key
     * return true success, false failed
     */
    bool         (*TryRemove)         (ArrayIntMap* arrayIntMap, intptr_t key);
    
    
    /**
     * Clear all value, reset size 0, and keep memory space
     */
    void         (*Clear)             (ArrayIntMap* arrayIntMap);
    
    
    /**
     *  Insert value from valuePtr at index, the possible index may be "-getIndex() - 1"
     *  return valuePtr in ArrayIntMap
     */
    void*        (*InsertAt)          (ArrayIntMap* arrayIntMap, intptr_t key, int index, void* valuePtr);


    /**
     * Get index of key, if negative not found then return "-insertIndex - 1"
     * so insert index is "-getIndex() - 1"
     */
    int          (*GetIndex)          (ArrayIntMap* arrayIntMap, intptr_t key);


    /**
     * Get Key at index
     */
    intptr_t     (*GetKey)            (ArrayIntMap* arrayIntMap, int index);


    /**
     * Get valuePtr at index
     */
    void*        (*GetAt)             (ArrayIntMap* arrayIntMap, int index);


    /**
     * Set value from valuePtr at index
     * return valuePtr in ArrayIntMap
     */
    void*        (*SetAt)             (ArrayIntMap* arrayIntMap, int index, void* valuePtr);


    /**
     * Remove value at index
     */
    void         (*RemoveAt)          (ArrayIntMap* arrayIntMap, int index);
};


extern struct AArrayIntMap AArrayIntMap[1];


/**
 * Marked ArrayIntMap key and value
 */
#define ArrayIntMap(keyName, valueType) ArrayIntMap


/**
 * Initialize constant ArrayIntMap
 * use like ArrayIntMap map[1] = AArrayIntMap_Init(valueType, increase)
 */
#define AArrayIntMap_Init(valueType, increase)              \
    {                                                       \
        {                                                   \
            sizeof(valueType),                              \
            AArrayList_Init(ArrayIntMapElement*, increase), \
        }                                                   \
    }


/**
 * Shortcut of AArrayIntMap->TryPut
 */
#define AArrayIntMap_TryPut(arrayIntMap, key, value) \
    AArrayIntMap->TryPut(arrayIntMap, (intptr_t) key, &(value))


/**
 * Shortcut of AArrayIntMap->Get
 * return value
 */
#define AArrayIntMap_Get(arrayIntMap, key, valueType) \
    (*(valueType*) AArrayIntMap->Get(arrayIntMap, (intptr_t) key, NULL_PTR))


/**
 * Shortcut of AArrayIntMap->Get
 * return valuePtr
 */
#define AArrayIntMap_GetPtr(arrayIntMap, key, valueType) \
    ((valueType*) AArrayIntMap->Get(arrayIntMap, (intptr_t) key, NULL))


/**
 * Shortcut of AArrayIntMap->TrySet
 */
#define AArrayIntMap_TrySet(arrayIntMap, key, value) \
    AArrayIntMap->TrySet(arrayIntMap, (intptr_t) key, &(value))


/**
 * Shortcut of AArrayIntMap->InsertAt
 */
#define AArrayIntMap_InsertAt(arrayIntMap, key, index, value) \
    AArrayIntMap->InsertAt(arrayIntMap, (intptr_t) key, index, &(value))


/**
 * Shortcut of AArrayIntMap->GetAt
 * return value
 */
#define AArrayIntMap_GetAt(arrayIntMap, index, valueType) \
   (*(valueType*) AArrayIntMap->GetAt(arrayIntMap, index))


/**
 * Shortcut of AArrayIntMap->GetAt
 * return valuePtr
 */
#define AArrayIntMap_GetPtrAt(arrayIntMap, index, valueType) \
   ((valueType*) AArrayIntMap->GetAt(arrayIntMap, index))


/**
 * Shortcut of AArrayIntMap->SetAt
 */
#define AArrayIntMap_SetAt(arrayIntMap, index, value) \
    AArrayIntMap->SetAt(arrayIntMap, index, &(value))


#endif
