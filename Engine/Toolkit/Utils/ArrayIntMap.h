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
 * Since    : 2013-5-27
 * Update   : 2019-1-17
 * Author   : scott.cgi
 */


#ifndef ARRAY_INT_MAP_H
#define ARRAY_INT_MAP_H


#include <stdbool.h>
#include <stdint.h>
#include "Engine/Toolkit/Utils/ArrayList.h"


/**
 * The actual element store in ArrayIntMap.
 */
typedef struct
{
    /**
     * Identity and search element value.
     */
    intptr_t key;

    /**
     * ArrayIntMap value pointer.
     * the value data copy into ArrayIntMapElement malloc memory space.
     */
    void*    valuePtr;
}
ArrayIntMapElement;


/**
 * A list of elements each of which is a k-v pair.
 */
typedef struct
{
    /**
     * The sizeof ArrayIntMap value type.
     */
    int                            valueTypeSize;

    /**
     * Store all ArrayIntMapElements.
     */
    ArrayList(ArrayIntMapElement*) elementList[1];
}
ArrayIntMap;


/**
 * Control ArrayIntMap.
 */
struct AArrayIntMap
{
    ArrayIntMap* (*Create)            (int valueTypeSize);
    void         (*Init)              (int valueTypeSize, ArrayIntMap* outArrayIntMap);

    ArrayIntMap* (*CreateWithCapacity)(int valueTypeSize, int capacity);
    void         (*InitWithCapacity)  (int valueTypeSize, int capacity, ArrayIntMap* outArrayIntMap);

    void         (*Release)           (ArrayIntMap* arrayIntMap);

    /**
     * Put key and value of valuePtr into ArrayIntMap.
     *
     * valuePtr: point to value that will copy in ArrayIntMapElement
     *
     * if key not exist in ArrayIntMap
     *     return valuePtr in ArrayIntMap
     * else
     *     return NULL
     */
    void*        (*TryPut)            (ArrayIntMap* arrayIntMap, intptr_t key, void* valuePtr);


    /**
     * Get valuePtr by key, if no key found return defaultValuePtr.
     */
    void*        (*Get)               (ArrayIntMap* arrayIntMap, intptr_t key, void* defaultValuePtr);


    /**
     * Try set new value of valuePtr to element by key.
     *
     * if key exist in ArrayIntMap
     *     return valuePtr in ArrayIntMap
     * else
     *     return NULL
     */
    void*        (*TrySet)            (ArrayIntMap* arrayIntMap, intptr_t key, void* valuePtr);


    /**
     * Remove element by key.
     * return true success, false failed.
     */
    bool         (*TryRemove)         (ArrayIntMap* arrayIntMap, intptr_t key);
    
    
    /**
     * Clear all elements, reset size 0, and keep memory space.
     */
    void         (*Clear)             (ArrayIntMap* arrayIntMap);
    
    
    /**
     *  Insert value of valuePtr at index, the possible index is (-getIndex() - 1).
     *  return valuePtr in ArrayIntMap
     */
    void*        (*InsertAt)          (ArrayIntMap* arrayIntMap, intptr_t key, int index, void* valuePtr);


    /**
     * Get index of key.
     * if negative means not found the element by key, and return (-insertIndex - 1),
     * so the insert index is (-getIndex() - 1).
     */
    int          (*GetIndex)          (ArrayIntMap* arrayIntMap, intptr_t key);


    /**
     * Get Key at index.
     */
    intptr_t     (*GetKey)            (ArrayIntMap* arrayIntMap, int index);


    /**
     * Get valuePtr at index.
     */
    void*        (*GetAt)             (ArrayIntMap* arrayIntMap, int index);


    /**
     * Set value of valuePtr at index.
     * return valuePtr in ArrayIntMap.
     */
    void*        (*SetAt)             (ArrayIntMap* arrayIntMap, int index, void* valuePtr);


    /**
     * Remove value at index
     */
    void         (*RemoveAt)          (ArrayIntMap* arrayIntMap, int index);
};


extern struct AArrayIntMap AArrayIntMap[1];


/**
 * Marked ArrayIntMap key and value.
 */
#define ArrayIntMap(keyName, ValueType) ArrayIntMap


/**
 * Init constant ArrayIntMap.
 * example: ArrayIntMap map[1] = AArrayIntMap_Init(ValueType, increase)
 */
#define AArrayIntMap_Init(ValueType, increase)          \
    {                                                   \
        sizeof(ValueType),                              \
        AArrayList_Init(ArrayIntMapElement*, increase), \
    }                                                   \


/**
 * Shortcut of AArrayIntMap->TryPut.
 */
#define AArrayIntMap_TryPut(arrayIntMap, key, value) \
    AArrayIntMap->TryPut(arrayIntMap, (intptr_t) (key), &(value))


/**
 * Shortcut of AArrayIntMap->Get with type.
 * return value.
 */
#define AArrayIntMap_Get(arrayIntMap, key, ValueType) \
    (*(ValueType*) AArrayIntMap->Get(arrayIntMap, (intptr_t) (key), NULL_PTR))


/**
 * Shortcut of AArrayIntMap->Get with type.
 * return valuePtr.
 */
#define AArrayIntMap_GetPtr(arrayIntMap, key, ValueType) \
    ((ValueType*) AArrayIntMap->Get(arrayIntMap, (intptr_t) (key), NULL))


/**
 * Shortcut of AArrayIntMap->TrySet.
 */
#define AArrayIntMap_TrySet(arrayIntMap, key, value) \
    AArrayIntMap->TrySet(arrayIntMap, (intptr_t) (key), &(value))


/**
 * Shortcut of AArrayIntMap->InsertAt.
 */
#define AArrayIntMap_InsertAt(arrayIntMap, key, index, value) \
    AArrayIntMap->InsertAt(arrayIntMap, (intptr_t) (key), index, &(value))


/**
 * Shortcut of AArrayIntMap->GetAt.
 * return value.
 */
#define AArrayIntMap_GetAt(arrayIntMap, index, ValueType) \
   (*(ValueType*) AArrayIntMap->GetAt(arrayIntMap, index))


/**
 * Shortcut of AArrayIntMap->GetAt.
 * return valuePtr.
 */
#define AArrayIntMap_GetPtrAt(arrayIntMap, index, ValueType) \
   ((ValueType*) AArrayIntMap->GetAt(arrayIntMap, index))


/**
 * Shortcut of AArrayIntMap->SetAt.
 */
#define AArrayIntMap_SetAt(arrayIntMap, index, value) \
    AArrayIntMap->SetAt(arrayIntMap, index, &(value))


#endif
