/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-5-20
 * Update: 2019-1-14
 * Author: scott.cgi
 */

#ifndef ARRAY_STR_MAP_H
#define  ARRAY_STR_MAP_H


#include <stdbool.h>
#include "Engine/Toolkit/Utils/ArrayList.h"


/**
 * The actual element store in ArrayStrMap.
 */
typedef struct
{
    /**
     * ArrayStrMap value's key.
     * the key data will copy into ArrayStrMapElement malloc space.
     */
    const char* key;

    /**
     * The length of key, include '\0'.
     */
    int         keyLength;

    /**
     * ArrayStrMap value pointer.
     * the value data copy into ArrayStrMapElement malloc space.
     */
    void*       valuePtr;
}
ArrayStrMapElement;


/**
 * A list of elements each of which is a k-v pair.
 */
typedef struct
{
    /**
     * The sizeof ArrayStrMap value type.
     */
    int                            valueTypeSize;

    /**
     * Stores all ArrayStrMapElements.
     */
    ArrayList(ArrayStrMapElement*) elementList[1];
}
ArrayStrMap;


/**
 * Control ArrayStrMap.
 */
struct AArrayStrMap
{
    ArrayStrMap* (*Create)             (int valueTypeSize);
    void         (*Init)               (int valueTypeSize, ArrayStrMap* outArrayStrMap);

    ArrayStrMap* (*CreateWithCapacity) (int valueTypeSize, int capacity);
    void         (*InitWithCapacity)   (int valueTypeSize, int capacity, ArrayStrMap* outArrayStrMap);

    void         (*Release)            (ArrayStrMap* arrayStrMap);

    /**
     * Try put key and value of valuePtr into ArrayStrMap.
     *
     * valuePtr: point to value.
     *
     * if key not exist in ArrayStrMap
     *     return valuePtr in ArrayStrMap
     * else
     *     return NULL
     */
    void*        (*TryPut)             (ArrayStrMap* arrayStrMap, const char* key, void* valuePtr);

    /**
     * Get valuePtr by key, if no key found return defaultValuePtr.
     */
    void*        (*Get)                (ArrayStrMap* arrayStrMap, const char* key, void* defaultValuePtr);

    /**
     * Try set new value of valuePtr to element by key.
     *
     * if key exist in ArrayStrMap
     *     return valuePtr in ArrayStrMap
     * else
     *     return NULL
     */
    void*        (*TrySet)             (ArrayStrMap* arrayStrMap, const char* key, void* valuePtr);

    /**
     * Remove element by key.
     * return true success, false failed.
     */
    bool         (*TryRemove)          (ArrayStrMap* arrayStrMap, const char* key);

    /**
     * Clear all values, reset size to 0, and keep memory space.
     */
    void         (*Clear)              (ArrayStrMap* arrayStrMap);

    /**
     * Insert value of valuePtr at index, the possible index is (-getIndex() - 1).
     * return valuePtr in ArrayStrMap.
     */
    void*        (*InsertAt)           (ArrayStrMap* arrayStrMap, const char* key, int index, void* valuePtr);

    /**
     * Get index of key.
     * if negative means not found the element by key, and return (-insertIndex - 1),
     * so the insert index is (-getIndex() - 1).
     */
    int          (*GetIndex)           (ArrayStrMap* arrayStrMap, const char* key);

    /**
     * Get key at index.
     */
    const char*  (*GetKey)             (ArrayStrMap* arrayStrMap, int index);

    /**
     * Get valuePtr at index.
     */
    void*        (*GetAt)              (ArrayStrMap* arrayStrMap, int index);

    /**
     * Set value of valuePtr at index.
     * return valuePtr in ArrayStrMap.
     */
    void*        (*SetAt)              (ArrayStrMap* arrayStrMap, int index, void* valuePtr);

    /**
     * Remove element at index.
     */
    void         (*RemoveAt)           (ArrayStrMap* arrayStrMap, int index);
};


extern struct AArrayStrMap AArrayStrMap[1];


/**
 * Get key in ArrayStrMapElement by valuePtr and valueTypeSize.
 */
static inline const char* AArrayStrMap_GetKey(void* valuePtr, int valueTypeSize)
{
    return (const char*) valuePtr + valueTypeSize;
}


/**
 * Marked ArrayStrMap key and value.
 */
#define ArrayStrMap(keyName, valueType) ArrayStrMap


/**
 * Init constant ArrayStrMap.
 * example: ArrayStrMap map[1] = AArrayStrMap_Init(valueType, increase)
 */
#define AArrayStrMap_Init(valueType, increase)             \
    {{                                                    \
          sizeof(valueType),                              \
          AArrayList_Init(ArrayStrMapElement*, increase), \
    }}


/**
 * Shortcut of ArrayStrMap->TryPut.
 */
#define AArrayStrMap_TryPut(arrayStrMap, key, value) \
    AArrayStrMap->TryPut(arrayStrMap, key, &(value))


/**
 * Shortcut of AArrayStrMap->Get.
 * return value.
 */
#define AArrayStrMap_Get(arrayStrMap, key, valueType) \
    (*(valueType*) AArrayStrMap->Get(arrayStrMap, key, NULL_PTR))


/**
 * Shortcut of AArrayStrMap->Get.
 * return valuePtr.
 */
#define AArrayStrMap_GetPtr(arrayStrMap, key, valueType) \
    ((valueType*) AArrayStrMap->Get(arrayStrMap, key, NULL))


/**
 * Shortcut of AArrayStrMap->TrySet.
 */
#define AArrayStrMap_TrySet(arrayStrMap, key, value) \
    AArrayStrMap->TrySet(arrayStrMap, key, &(value));


/**
 * Shortcut of AArrayStrMap->InsertAt.
 */
#define AArrayStrMap_InsertAt(arrayStrMap, key, index, value) \
    AArrayStrMap->InsertAt(arrayStrMap, key, index, &(value))


/**
 * Shortcut of AArrayStrMap->GetAt.
 * return value.
 */
#define AArrayStrMap_GetAt(arrayStrMap, index, valueType) \
    (*(valueType*) AArrayStrMap->GetAt(arrayStrMap, index))


/**
 * Shortcut of AArrayStrMap->GetAt.
 * return valuePtr.
 */
#define AArrayStrMap_GetPtrAt(arrayStrMap, index, valueType) \
    ((valueType*) AArrayStrMap->GetAt(arrayStrMap, index))


/**
 * Shortcut of AArrayStrMap->SetAt.
 */
#define AArrayStrMap_SetAt(arrayStrMap, index, value) \
    AArrayStrMap->SetAt(arrayStrMap, index, &(value))


#endif
