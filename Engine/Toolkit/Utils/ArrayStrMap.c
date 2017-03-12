/*
 *
 *
 *  Created on: 2013-5-20
 *      Author: scott.cgi
 */

#include <string.h>
#include <stdlib.h>

#include "Engine/Toolkit/Utils/ArrayStrMap.h"
#include "Engine/Toolkit/Platform/Log.h"


#define CheckIndex(tag) \
	ALogA(index >= 0 && index < arrayStrMap->arrayList->size, "ArrayStrMap "  tag " index = %d, size = %d, invalid", index, arrayStrMap->arrayList->size)


#define CheckInsertIndex(tag) \
	ALogA(index >= 0 && index <= arrayStrMap->arrayList->size, "ArrayStrMap " tag " index = %d, size = %d, invalid", index, arrayStrMap->arrayList->size)


/**
 * Search index of key, if negative not found then return "-insertIndex - 1"
 * so insert index is "-BinarySearch() - 1"
 */
static inline int BinarySearch(ArrayList* elements, const char* key, int keyLength)
{
	int high  = elements->size;
	int low   = -1;
	int guess = -1;

	while (high - low > 1)
    {
		// not consider int overflow
		guess                       = (high + low) >> 1;
		ArrayStrMapElement* element = AArrayListGet(elements, guess, ArrayStrMapElement*);

		if (element->keyLength < keyLength)
        {
			low  = guess;
		}
        else if (element->keyLength > keyLength)
        {
			high = guess;
		}
        else if (element->keyLength == keyLength)
        {
			int cmp = memcmp(element->key, key, keyLength);

			if (cmp < 0)
            {
				low  = guess;
			}
			else if (cmp > 0)
			{
				high = guess;
			}
			else if (cmp == 0)
			{
				// cmp 0 means find the key
				return guess;
			}
		}
	 }

	// if guess == high the guess is bigger than key in ArrayStrMap and insert value at guess

	if (guess == low)
	{
		// the guess is smaller than key in ArrayStrMap and insert value behind
		// or if ArrayStrMap empty then guess is -1, also do this make guess at 0
		guess++;
	}

	// when ArrayStrMap empty guess is 0, so we -1 make sure return negative value
	return -guess - 1;
}


//--------------------------------------------------------------------------------------------------


static void* Put(ArrayStrMap* arrayStrMap, const char* key, void* valuePtr)
{
	int keyLength = (int) strlen(key) + 1;
	int guess     = BinarySearch(arrayStrMap->arrayList, key, keyLength);

	ALogA(guess < 0, "ArrayStrMap put key = %s, has already exist", key);

	int                 typeSize = arrayStrMap->typeSize;
	ArrayStrMapElement* element  = (ArrayStrMapElement*) malloc(sizeof(ArrayStrMapElement) + typeSize + keyLength);

	element->keyLength           = keyLength;
	element->valuePtr            = (char*) element + sizeof(ArrayStrMapElement);
	element->key                 = (char*) element->valuePtr + typeSize;
	memcpy((void*) element->key, key, keyLength);

	AArrayListInsert(arrayStrMap->arrayList, -guess - 1, element);

    return memcpy(element->valuePtr, valuePtr, typeSize);
}


static void* Get(ArrayStrMap* arrayStrMap, const char* key, void* defaultValuePtr)
{
	int guess = BinarySearch(arrayStrMap->arrayList, key, strlen(key) + 1);
	return guess >= 0 ? AArrayListGet(arrayStrMap->arrayList, guess, ArrayStrMapElement*)->valuePtr : defaultValuePtr;
}


static void* Set(ArrayStrMap* arrayStrMap, const char* key, void* valuePtr)
{
	int guess = BinarySearch(arrayStrMap->arrayList, key, strlen(key) + 1);

	ALogA(guess >= 0, "ArrayStrMap set key = %s, has not exist", key);

	return memcpy
		   (
			   AArrayListGet(arrayStrMap->arrayList, guess, ArrayStrMapElement*)->valuePtr,
			   valuePtr,
			   arrayStrMap->typeSize
		   );
}


static bool TryRemove(ArrayStrMap* arrayStrMap, const char* key)
{
	int guess = BinarySearch(arrayStrMap->arrayList, key, strlen(key) + 1);

	if (guess >= 0)
	{
		free
		(
			AArrayListGet(arrayStrMap->arrayList, guess, ArrayStrMapElement*)
		);

		AArrayList->Remove(arrayStrMap->arrayList, guess);

		return true;
	}

	return false;
}


static void Clear(ArrayStrMap* arrayStrMap)
{
	for (int i = 0; i < arrayStrMap->arrayList->size; i++)
	{
		free
		(
			AArrayListGet(arrayStrMap->arrayList, i, ArrayStrMapElement*)
		);
	}

	AArrayList->Clear(arrayStrMap->arrayList);
}


static void* InsertAt(ArrayStrMap* arrayStrMap, const char* key, int index, void* valuePtr)
{
	CheckInsertIndex("InsertAt");

	int keyLength = (int) strlen(key) + 1;
	int typeSize  = arrayStrMap->typeSize;

	ArrayStrMapElement* element = (ArrayStrMapElement*) malloc(sizeof(ArrayStrMapElement) + typeSize + keyLength);
	element->keyLength          = keyLength;
	element->valuePtr           = (char*) element + sizeof(ArrayStrMapElement);
	element->key                = (char*) element->valuePtr + typeSize;
	memcpy((void*) element->key, key, keyLength);

	AArrayListInsert( arrayStrMap->arrayList, index, element);

	return memcpy(element->valuePtr, valuePtr, typeSize);
}


static int GetIndex(ArrayStrMap* arrayStrMap, const char* key)
{
	return BinarySearch(arrayStrMap->arrayList, key, strlen(key) + 1);
}


static const char* GetKey(ArrayStrMap* arrayStrMap, int index)
{
	CheckIndex("GetKey");
	return AArrayListGet(arrayStrMap->arrayList, index, ArrayStrMapElement*)->key;
}


static void* GetAt(ArrayStrMap* arrayStrMap, int index)
{
	CheckIndex("GetAt");
	return AArrayListGet(arrayStrMap->arrayList, index, ArrayStrMapElement*)->valuePtr;
}


static void* SetAt(ArrayStrMap* arrayStrMap, int index, void* valuePtr)
{
	CheckIndex("SetAt");

	return memcpy
		   (
			   AArrayListGet(arrayStrMap->arrayList, index, ArrayStrMapElement*)->valuePtr,
			   valuePtr,
			   arrayStrMap->typeSize
		   );
}


static void RemoveAt(ArrayStrMap* arrayStrMap, int index)
{
	CheckIndex("RemoveAt");

	free
	(
		AArrayListGet(arrayStrMap->arrayList, index, ArrayStrMapElement*)
	);

	AArrayList->Remove(arrayStrMap->arrayList, index);
}


static void Release(ArrayStrMap*  arrayStrMap)
{
	for (int i = 0; i < arrayStrMap->arrayList->size; i++)
	{
		free
		(
			AArrayListGet(arrayStrMap->arrayList, i, ArrayStrMapElement*)
		);
	}

	AArrayList->Release(arrayStrMap->arrayList);
}


static void InitWithCapacity(int typeSize, int capacity, ArrayStrMap* outArrayStrMap)
{
	if (capacity == 0)
	{
		AArrayList->Init(sizeof(ArrayStrMapElement*), outArrayStrMap->arrayList);
	}
	else
	{
		AArrayList->InitWithCapacity(sizeof(ArrayStrMapElement*), capacity, outArrayStrMap->arrayList);
	}

	outArrayStrMap->typeSize = typeSize;
}


static ArrayStrMap* CreateWithCapacity(int typeSize, int capacity)
{
	ArrayStrMap* arrayStrMap = (ArrayStrMap*) malloc(sizeof(ArrayStrMap));
	InitWithCapacity(typeSize, capacity, arrayStrMap);

	return arrayStrMap;
}


static void Init(int typeSize, ArrayStrMap* outArrayStrMap)
{
	InitWithCapacity(typeSize, 0, outArrayStrMap);
}


static ArrayStrMap* Create(int typeSize)
{
	return CreateWithCapacity(typeSize, 0);
}


struct AArrayStrMap AArrayStrMap[1] =
{
	Create,
	Init,
	CreateWithCapacity,
	InitWithCapacity,

	Release,

	Put,
	Get,
	Set,
	TryRemove,
	Clear,
	InsertAt,
	GetIndex,
	GetKey,
	GetAt,
	SetAt,
	RemoveAt,
};

#undef CheckIndex
#undef CheckInsertIndex
