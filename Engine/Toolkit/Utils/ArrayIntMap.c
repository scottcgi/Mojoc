/*
 *
 *
 *  Created on: 2013-5-20
 *      Author: scott.cgi
 */

#include <string.h>
#include <stdint.h>

#include "Mojoc/Toolkit/Utils/ArrayIntMap.h"
#include "Mojoc/Toolkit/Platform/Log.h"



#define CheckIndex(tag) \
	ALogA(index >= 0 && index <  arrayIntMap->arrayList->size, "ArrayIntMap" tag "index = %d, size = %d, invalid", index, arrayIntMap->arrayList->size);

#define CheckInsertIndex(tag) \
	ALogA(index >= 0 && index <= arrayIntMap->arrayList->size, "ArrayIntMap" tag "index = %d, size = %d, invalid", index, arrayIntMap->arrayList->size);



/**
 * Search index of key, if negative not found then return "-insertIndex - 1"
 * so insert index is "-BinarySearch() - 1"
 */
static inline int BinarySearch(ArrayList* elements, intptr_t key)
{
	int high  = elements->size;
	int low   = -1;
	int guess = -1;

	while (high - low > 1)
	{
		// not consider int overflow
		guess               = (high + low) >> 1;
	    intptr_t elementKey = AArrayListGet(elements, guess, ArrayIntMapElement*)->key;

		if (elementKey < key)
		{
			low  = guess;
		}
		else if (elementKey > key)
		{
			high = guess;
		}
		else if (elementKey == key)
		{
			// find the key
			return guess;
		}
	}

	// if guess == high the guess is bigger than key in ArrayIntMap and insert value at guess

	if (guess == low)
	{
		// the guess is smaller than key in ArrayIntMap and insert value behind
		// or if ArrayIntMap empty then guess is -1, also do this make guess at 0
		guess++;
	}

	// when ArrayIntMap empty guess is 0, so we -1 make sure return negative value
	return -guess - 1;
}


//--------------------------------------------------------------------------------------------------


static void* Put(ArrayIntMap* arrayIntMap, intptr_t key, void* valuePtr)
{
	int guess = BinarySearch(arrayIntMap->arrayList, key);

	ALogA(guess < 0, "ArrayIntMap put key = %zd, has already exist", key);

	ArrayIntMapElement* element = (ArrayIntMapElement*) malloc(sizeof(ArrayIntMapElement) + arrayIntMap->typeSize);
	element->key                = key;
	element->valuePtr           = (char*) element + sizeof(ArrayIntMapElement);

	AArrayListInsert(arrayIntMap->arrayList, -guess - 1, element);

    return memcpy(element->valuePtr, valuePtr, arrayIntMap->typeSize);
}


static void* Get(ArrayIntMap* arrayIntMap, intptr_t key, void* defaultValutPtr)
{
	int guess = BinarySearch(arrayIntMap->arrayList, key);

	return guess >= 0 ? AArrayListGet(arrayIntMap->arrayList, guess, ArrayIntMapElement*)->valuePtr : defaultValutPtr;
}


static void* Set(ArrayIntMap* arrayIntMap, intptr_t key, void* valuePtr)
{
	int guess = BinarySearch(arrayIntMap->arrayList, key);

	ALogA(guess >= 0, "ArrayIntMap Set key = %zd, has not exist", key);

	return memcpy
		   (
			   AArrayListGet(arrayIntMap->arrayList, guess, ArrayIntMapElement*)->valuePtr,
			   valuePtr,
			   arrayIntMap->typeSize
		   );
}


static bool TryRemove(ArrayIntMap* arrayIntMap, intptr_t key)
{
	int guess = BinarySearch(arrayIntMap->arrayList, key);

	if (guess >= 0)
	{
		free
		(
			AArrayListGet(arrayIntMap->arrayList, guess, ArrayIntMapElement*)
		);

		AArrayList->Remove(arrayIntMap->arrayList, guess);

		return true;
	}
	
	return false;
}


static void Clear(ArrayIntMap* arrayIntMap)
{
	for (int i = 0; i < arrayIntMap->arrayList->size; i++)
	{
		free
		(
			AArrayListGet(arrayIntMap->arrayList, i, ArrayIntMapElement*)
		);
	}

	AArrayList->Clear(arrayIntMap->arrayList);
}


static void* InsertAt(ArrayIntMap* arrayIntMap, intptr_t key, int index, void* valuePtr)
{
	CheckInsertIndex("InsertAt");

	ArrayIntMapElement* element = (ArrayIntMapElement*) malloc(sizeof(ArrayIntMapElement) + arrayIntMap->typeSize);

	AArrayListInsert(arrayIntMap->arrayList, index, element);
	element->key                = key;
	element->valuePtr           = (char*) element + sizeof(ArrayIntMapElement);

	return memcpy(element->valuePtr, valuePtr, arrayIntMap->typeSize);
}


static int GetIndex(ArrayIntMap* arrayIntMap, intptr_t key)
{
	return BinarySearch(arrayIntMap->arrayList, key);
}


static intptr_t GetKey(ArrayIntMap* arrayIntMap, int index)
{
	CheckIndex("GetKey");
	return AArrayListGet(arrayIntMap->arrayList, index, ArrayIntMapElement*)->key;
}

static void* GetAt(ArrayIntMap* arrayIntMap, int index)
{
	CheckIndex("GetAt");
	return AArrayListGet(arrayIntMap->arrayList, index, ArrayIntMapElement*)->valuePtr;
}


static void* SetAt(ArrayIntMap* arrayIntMap, int index, void* valuePtr)
{
	CheckIndex("SetAt");

	return memcpy
		   (
			   AArrayListGet(arrayIntMap->arrayList, index, ArrayIntMapElement*)->valuePtr,
			   valuePtr,
			   arrayIntMap->typeSize
		   );
}


static void RemoveAt(ArrayIntMap* arrayIntMap, int index)
{
	CheckIndex("RemoveAt");

	free
	(
		AArrayListGet(arrayIntMap->arrayList, index, ArrayIntMapElement*)
	);

	AArrayList->Remove(arrayIntMap->arrayList, index);
}


static void Release(ArrayIntMap* arrayIntMap)
{
	for (int i = 0; i < arrayIntMap->arrayList->size; i++)
	{
		free
		(
			AArrayListGet(arrayIntMap->arrayList, i, ArrayIntMapElement*)
		);
	}

	AArrayList->Release(arrayIntMap->arrayList);
}


static void InitWithCapacity(int typeSize, int capacity, ArrayIntMap* outArrayIntMap)
{
	if (capacity == 0)
	{
		AArrayList->Init(sizeof(ArrayIntMapElement*), outArrayIntMap->arrayList);
	}
	else
	{
		AArrayList->InitWithCapacity(sizeof(ArrayIntMapElement*), capacity, outArrayIntMap->arrayList);
	}

	outArrayIntMap->typeSize = typeSize;
}


static ArrayIntMap* CreateWithCapacity(int typeSize, int capacity)
{
	ArrayIntMap* arrayIntMap = (ArrayIntMap*) malloc(sizeof(ArrayIntMap));
	InitWithCapacity(typeSize, capacity, arrayIntMap);

	return arrayIntMap;
}


static void Init(int typeSize, ArrayIntMap* outArrayIntMap)
{
	InitWithCapacity(typeSize, 0, outArrayIntMap);
}


static ArrayIntMap* Create(int typeSize)
{
	return CreateWithCapacity(typeSize, 0);
}


struct AArrayIntMap AArrayIntMap[1] =
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



