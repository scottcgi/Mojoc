/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2013-5-20
 * Author : scott.cgi
 */

#ifndef array_str_map_h
#define array_str_map_h


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
     * Hold all ArrayStrMapElement
     */
	ArrayList arrayList[1];

	/**
	 * ArrayStrMap value type sizeof
	 */
	int       typeSize;
}
ArrayStrMap;


struct AArrayStrMap
{
	ArrayStrMap* (*Create)             (int typeSize);
	void         (*Init)               (int typeSize, ArrayStrMap* outArrayStrMap);

	ArrayStrMap* (*CreateWithCapacity) (int typeSize, int capacity);
	void         (*InitWithCapacity)   (int typeSize, int capacity, ArrayStrMap* outArrayStrMap);

	void         (*Release)            (ArrayStrMap*  arrayStrMap);

	/**
	 * Put key and value from valuePtr into ArrayStrMap
	 *
	 * valuePtr: point to value
	 *
	 * return valuePtr in ArrayStrMap
	 */
	void*        (*Put)                (ArrayStrMap* arrayStrMap, const char* key, void* valuePtr);

	/**
	 * Get valuePtr by key, if no key found return defaultValuePtr
	 */
	void*        (*Get)                (ArrayStrMap* arrayStrMap, const char* key, void* defaultValuePtr);

	/**
	 * Set new value from valuePtr by key, return valuePtr in ArrayStrMap
	 */
	void*        (*Set)                (ArrayStrMap* arrayStrMap, const char* key, void* valuePtr);

	/**
	 * Remove value by key
     * return true success, false failed
	 */
	bool         (*TryRemove)          (ArrayStrMap* arrayStrMap, const char* key);

	/**
	 * Clear all value, reset size 0, and keep memory space
	 */
	void         (*Clear)              (ArrayStrMap* arrayStrMap);

	/**
	 * Insert value from valuePtr at index, the possible index may be -getIndex() - 1
	 * return valuePtr in ArrayStrMap
	 */
	void*        (*InsertAt)           (ArrayStrMap* arrayStrMap, const char* key, int index, void* valuePtr);

	/**
	 * Get index of key, if negative not found then return -insertIndex - 1
	 * so insert index is -getIndex() - 1
	 */
	int          (*GetIndex)           (ArrayStrMap* arrayStrMap, const char* key);

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
 * Get key in ArrayStrMapElement by valuePtr with typeSize
 */
static inline char* AArrayStrMapGetKey(void* valuePtr, int typeSize)
{
	return (char*) valuePtr + typeSize;
}


static inline int AArrayStrMapSize(ArrayStrMap* arrayStrMap)
{
	return arrayStrMap->arrayList->size;
}


static inline void AArrayStrMapSetIncrease(ArrayStrMap* arrayStrMap, int increase)
{
	arrayStrMap->arrayList->increase = increase;
}


/**
 * Marked ArrayIntMap key and value
 */
#define ArrayStrMap(keyName, valueType) ArrayStrMap


/**
 * Initialize constant ArrayStrMap
 * use like ArrayStrMap map[1] = AArrayStrMapInit(type, increase)
 */
#define AArrayStrMapInit(type, increase)                   \
	{                                                      \
		{                                                  \
			AArrayListInit(ArrayStrMapElement*, increase), \
			sizeof(type),                                  \
	 	 }                                                 \
	}


/**
 * Shortcut of ArrayStrMap->Put
 */
#define AArrayStrMapPut(arrayStrMap, key, value) \
	AArrayStrMap->Put(arrayStrMap, key, &(value))


/**
 * Shortcut of AArrayStrMap->Get
 * return value
 */
#define AArrayStrMapGet(arrayStrMap, key, type) \
	(*(type*) AArrayStrMap->Get(arrayStrMap, key, null_ptr))


/**
 * Shortcut of AArrayStrMap->Get
 * return valuePtr
 */
#define AArrayStrMapGetPtr(arrayStrMap, key, type) \
	((type*) AArrayStrMap->Get(arrayStrMap, key, NULL))


/**
 * Shortcut of AArrayStrMap->Set
 */
#define AArrayStrMapSet(arrayStrMap, key, value) \
	AArrayStrMap->Set(arrayStrMap, key, &(value));


/**
 * Shortcut of AArrayStrMap->InsertAt
 */
#define AArrayStrMapInsertAt(arrayStrMap, key, index, value) \
	AArrayStrMap->InsertAt(arrayStrMap, key, index, &(value))


/**
 * Shortcut of AArrayStrMap->GetAt
 * return value
 */
#define AArrayStrMapGetAt(arrayStrMap, index, type) \
	(*(type*) AArrayStrMap->GetAt(arrayStrMap, index))


/**
 * Shortcut of AArrayStrMap->GetAt
 * return valuePtr
 */
#define AArrayStrMapGetPtrAt(arrayStrMap, index, type) \
	((type*) AArrayStrMap->GetAt(arrayStrMap, index))


/**
 * Shortcut of AArrayStrMap->SetAt
 */
#define AArrayStrMapSetAt(arrayStrMap, index, value) \
	AArrayStrMap->SetAt(arrayStrMap, index, &(value))


#endif
