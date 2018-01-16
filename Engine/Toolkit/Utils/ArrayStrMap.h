/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-5-20
 * Author: scott.cgi
 */

#ifndef ARRAY_STR_MAP_H
#define ARRAY_STR_MAP_H


#include <stdbool.h>
#include "Engine/Toolkit/Utils/ArrayList.h"


typedef struct
{
    char*  key;

    /**
     * length of key include '\0'
     */
    int    keyLength;

    /**
     * ArrayStrMap value pointer
     * value data copy into ArrayStrMapElement
     */
    void*  valuePtr;
}
ArrayStrMapElement;


typedef struct
{
    /**
     * The sizeof ArrayStrMap value type
     */
    int                            valueTypeSize;

    /**
     * Hold all ArrayStrMapElement
     */
    ArrayList(ArrayStrMapElement*) elementList[1];
}
ArrayStrMap;


struct AArrayStrMap
{
    ArrayStrMap* (*Create)             (int valueTypeSize);
    void         (*Init)               (int valueTypeSize, ArrayStrMap* outArrayStrMap);

    ArrayStrMap* (*CreateWithCapacity) (int valueTypeSize, int capacity);
    void         (*InitWithCapacity)   (int valueTypeSize, int capacity, ArrayStrMap* outArrayStrMap);

    void         (*Release)            (ArrayStrMap*  arrayStrMap);

    /**
     * Put key and value from valuePtr into ArrayStrMap
     *
     * valuePtr: point to value
     *
     * if key not exist in ArrayStrMap
     *     return valuePtr in ArrayStrMap
     * else
     *     return NULL
     */
    void*        (*TryPut)             (ArrayStrMap* arrayStrMap, char* key, void* valuePtr);

    /**
     * Get valuePtr by key, if no key found return defaultValuePtr
     */
    void*        (*Get)                (ArrayStrMap* arrayStrMap, char* key, void* defaultValuePtr);

    /**
     * Set new value from valuePtr by key
     *
     * if key exist in ArrayStrMap
     *     return valuePtr in ArrayStrMap
     * else
     *     return NULL
     */
    void*        (*TrySet)             (ArrayStrMap* arrayStrMap, char* key, void* valuePtr);

    /**
     * Remove value by key
     * return true success, false failed
     */
    bool         (*TryRemove)          (ArrayStrMap* arrayStrMap, char* key);

    /**
     * Clear all value, reset size 0, and keep memory space
     */
    void         (*Clear)              (ArrayStrMap* arrayStrMap);

    /**
     * Insert value from valuePtr at index, the possible index may be -getIndex() - 1
     * return valuePtr in ArrayStrMap
     */
    void*        (*InsertAt)           (ArrayStrMap* arrayStrMap, char* key, int index, void* valuePtr);

    /**
     * Get index of key, if negative not found then return -insertIndex - 1
     * so insert index is -getIndex() - 1
     */
    int          (*GetIndex)           (ArrayStrMap* arrayStrMap, char* key);

    /**
     * Get key at index
     */
    char*        (*GetKey)             (ArrayStrMap* arrayStrMap, int index);

    /**
     * Get valuePtr at index
     */
    void*        (*GetAt)              (ArrayStrMap* arrayStrMap, int index);

    /**
     * Set value from valuePtr at index
     * return valuePtr in ArrayStrMap
     */
    void*        (*SetAt)              (ArrayStrMap* arrayStrMap, int index, void* valuePtr);

    /**
     * Remove value at index
     */
    void         (*RemoveAt)           (ArrayStrMap* arrayStrMap, int index);
};


extern struct AArrayStrMap AArrayStrMap[1];


/**
 * Get key in ArrayStrMapElement by valuePtr with valueTypeSize
 */
static inline char* AArrayStrMap_GetKey(void* valuePtr, int valueTypeSize)
{
    return (char*) valuePtr + valueTypeSize;
}


/**
 * Marked ArrayStrMap key and value
 */
#define ArrayStrMap(keyName, valueType) ArrayStrMap


/**
 * Initialize constant ArrayStrMap
 * use like ArrayStrMap map[1] = AArrayStrMap_Init(valueType, increase)
 */
#define AArrayStrMap_Init(valueType, increase)              \
    {                                                       \
        {                                                   \
            sizeof(valueType),                              \
            AArrayList_Init(ArrayStrMapElement*, increase), \
          }                                                 \
    }


/**
 * Shortcut of ArrayStrMap->TryPut
 */
#define AArrayStrMap_TryPut(arrayStrMap, key, value) \
    AArrayStrMap->TryPut(arrayStrMap, key, &(value))


/**
 * Shortcut of AArrayStrMap->Get
 * return value
 */
#define AArrayStrMap_Get(arrayStrMap, key, valueType) \
    (*(valueType*) AArrayStrMap->Get(arrayStrMap, key, NULL_PTR))


/**
 * Shortcut of AArrayStrMap->Get
 * return valuePtr
 */
#define AArrayStrMap_GetPtr(arrayStrMap, key, valueType) \
    ((valueType*) AArrayStrMap->Get(arrayStrMap, key, NULL))


/**
 * Shortcut of AArrayStrMap->TrySet
 */
#define AArrayStrMap_TrySet(arrayStrMap, key, value) \
    AArrayStrMap->TrySet(arrayStrMap, key, &(value));


/**
 * Shortcut of AArrayStrMap->InsertAt
 */
#define AArrayStrMap_InsertAt(arrayStrMap, key, index, value) \
    AArrayStrMap->InsertAt(arrayStrMap, key, index, &(value))


/**
 * Shortcut of AArrayStrMap->GetAt
 * return value
 */
#define AArrayStrMap_GetAt(arrayStrMap, index, valueType) \
    (*(valueType*) AArrayStrMap->GetAt(arrayStrMap, index))


/**
 * Shortcut of AArrayStrMap->GetAt
 * return valuePtr
 */
#define AArrayStrMap_GetPtrAt(arrayStrMap, index, valueType) \
    ((valueType*) AArrayStrMap->GetAt(arrayStrMap, index))


/**
 * Shortcut of AArrayStrMap->SetAt
 */
#define AArrayStrMap_SetAt(arrayStrMap, index, value) \
    AArrayStrMap->SetAt(arrayStrMap, index, &(value))


#endif
