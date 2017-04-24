/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2013-5-27
 * Author: scott.cgi
 */

#ifndef array_int_map_h
#define array_int_map_h


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
	 * return valuePtr in ArrayIntMap
	 */
	void*        (*Put)               (ArrayIntMap* arrayIntMap, intptr_t key, void* valuePtr);


	/**
	 * Get valuePtr by key, if no key found return defaultValutPtr
	 */
	void*        (*Get)               (ArrayIntMap* arrayIntMap, intptr_t key, void* defaultValuePtr);


	/**
	 * Set new value from valuePtr by key, return valuePtr in ArrayIntMap
	 */
	void*        (*Set)               (ArrayIntMap* arrayIntMap, intptr_t key, void* valuePtr);


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
 * use like ArrayIntMap map[1] = AArrayIntMapInit(valueType, increase)
 */
#define AArrayIntMapInit(valueType, increase)              \
	{                                                      \
		{                                                  \
			sizeof(valueType),                             \
			AArrayListInit(ArrayIntMapElement*, increase), \
		}                                                  \
	}


/**
 * Shortcut of AArrayIntMap->Put
 */
#define AArrayIntMapPut(arrayIntMap, key, value) \
	AArrayIntMap->Put(arrayIntMap, (intptr_t) key, &(value))


/**
 * Shortcut of AArrayIntMap->Get
 * return value
 */
#define AArrayIntMapGet(arrayIntMap, key, valueType) \
	(*(valueType*) AArrayIntMap->Get(arrayIntMap, (intptr_t) key, null_ptr))


/**
 * Shortcut of AArrayIntMap->Get
 * return valuePtr
 */
#define AArrayIntMapGetPtr(arrayIntMap, key, valueType) \
	((valueType*) AArrayIntMap->Get(arrayIntMap, (intptr_t) key, NULL))


/**
 * Shortcut of AArrayIntMap->Set
 */
#define AArrayIntMapSet(arrayIntMap, key, value) \
	AArrayIntMap->Set(arrayIntMap, (intptr_t) key, &(value))


/**
 * Shortcut of AArrayIntMap->InsertAt
 */
#define AArrayIntMapInsertAt(arrayIntMap, key, index, value) \
	AArrayIntMap->InsertAt(arrayIntMap, (intptr_t) key, index, &(value))


/**
 * Shortcut of AArrayIntMap->GetAt
 * return value
 */
#define AArrayIntMapGetAt(arrayIntMap, index, valueType) \
   (*(valueType*) AArrayIntMap->GetAt(arrayIntMap, index))


/**
 * Shortcut of AArrayIntMap->GetAt
 * return valuePtr
 */
#define AArrayIntMapGetPtrAt(arrayIntMap, index, valueType) \
   ((valueType*) AArrayIntMap->GetAt(arrayIntMap, index))


/**
 * Shortcut of AArrayIntMap->SetAt
 */
#define AArrayIntMapSetAt(arrayIntMap, index, value) \
	AArrayIntMap->SetAt(arrayIntMap, index, &(value))


#endif
