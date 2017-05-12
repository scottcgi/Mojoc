/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2013-2-4
 * Author: scott.cgi
 */

#include <string.h>
#include <stdlib.h>

#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Toolkit/Platform/Log.h"


#define CheckIndex(tag, index) \
	ALogA(((index) < arrayList->size && (index) >= 0),  "ArrayList " tag " failed index error, index = %d, size = %d", index, arrayList->size);


#define CheckInsertIndex(tag) \
	ALogA(((index) <= arrayList->size && (index) >= 0), "ArrayList " tag " failed index error, index = %d, size = %d", index, arrayList->size);


static inline void AddCapacity(ArrayList* arrayList, int increase)
{
	ALogA(increase > 0, "ArrayList AddCapacity failed, increase = %d can not <= 0", increase);

	void* data = realloc(arrayList->elementArray->data, (increase + arrayList->elementArray->length) * arrayList->elementTypeSize);

	ALogA
	(
	    data != NULL,
		"ArrayList AddCapacity failed, unable to realloc memory, size = %d, length = %d, increase = %d",
		arrayList->size, arrayList->elementArray->length, increase
	);

/*
----------------------------------------------------------------------------------------------------
	//set increase data to 0, but it seems not very necessary
	memset
	(
	    (char*) data + arrayList->elementTypeSize * arrayList->array->length,
		 0,
		 arrayList->elementTypeSize * increase
    );
----------------------------------------------------------------------------------------------------
*/

	arrayList->elementArray->data    = data;
	arrayList->elementArray->length += increase;
}


static void* GetAdd(ArrayList* arrayList)
{
	if (arrayList->size == arrayList->elementArray->length)
	{
		AddCapacity(arrayList, arrayList->increase);
	}

    return (char*) arrayList->elementArray->data + arrayList->elementTypeSize * (arrayList->size++);
}


static void* GetInsert(ArrayList* arrayList, int index)
{
	// insert index can equal size
	CheckInsertIndex("GetInsert");

	if (arrayList->size == arrayList->elementArray->length)
	{
		AddCapacity(arrayList, arrayList->increase);
	}

	void* from = (char*) arrayList->elementArray->data + arrayList->elementTypeSize * index;
	void* to   = (char*) from                          + arrayList->elementTypeSize;

	memmove(to, from, arrayList->elementTypeSize * (arrayList->size - index));
	arrayList->size++;

	return from;
}


static void* Add(ArrayList* arrayList, void* elementPtr)
{
	return memcpy(GetAdd(arrayList), elementPtr, arrayList->elementTypeSize);
}


static void* Insert(ArrayList* arrayList, int index, void* elementPtr)
{
	return memcpy(GetInsert(arrayList, index), elementPtr, arrayList->elementTypeSize);
}


/*
----------------------------------------------------------------------------------------------------
// use macro instead
static void* Get(ArrayList* arrayList, int index)
{
	CheckIndex("Get", index);
	return (char*) arrayList->array->data + arrayList->elementTypeSize * index;
}
----------------------------------------------------------------------------------------------------
*/


/*
----------------------------------------------------------------------------------------------------
// use macro instead
static void Set(ArrayList* arrayList, int index, void* elementPtr)
{
	CheckIndex("Set", index);
	memcpy((char*) arrayList->array->data + arrayList->elementTypeSize * index, elementPtr, arrayList->elementTypeSize);
}
----------------------------------------------------------------------------------------------------
*/


static void* Pop(ArrayList* arrayList, void* defaultElementPtr)
{
	if (arrayList->size > 0)
	{
		// now size equal last index
		return (char*) arrayList->elementArray->data + arrayList->elementTypeSize * (--arrayList->size);
	}
	else
	{
		return defaultElementPtr;
	}
}


static void AddArray(ArrayList* arrayList, void* data, int length, int elementTypeSize)
{
    ALogA
    (
        arrayList->elementTypeSize == elementTypeSize,
        "AddArray ArrayList elementTypeSize %d must equal Array elementTypeSize %d",
        arrayList->elementTypeSize,
        elementTypeSize
    );

    int n = arrayList->elementArray->length - arrayList->size - length;

    if (n < 0)
    {
        AddCapacity(arrayList, -n + arrayList->increase);
    }

    memcpy
    (
        (char*) arrayList->elementArray->data + arrayList->elementTypeSize * arrayList->size,
        data,
        arrayList->elementTypeSize * length
    );

    arrayList->size += length;
}


static void Remove(ArrayList* arrayList, int index)
{
	CheckIndex("Remove", index);

	// now size equal last index
	arrayList->size--;

	if (index != arrayList->size)
	{
		void* to   = (char*) arrayList->elementArray->data + arrayList->elementTypeSize * index;
		void* from = (char*) to                            + arrayList->elementTypeSize;

		// move between index and last index element
		memcpy(to, from, arrayList->elementTypeSize * (arrayList->size - index));

		// set last element 0, but it seems not very necessary
		// memset((char*) arrayList->array->data + arrayList->elementTypeSize * arrayList->size, 0, arrayList->elementTypeSize);
	}
/*
----------------------------------------------------------------------------------------------------
	else
	{
		// remove the last element only set 0, but it seems not very necessary
		// memset((char*) arrayList->array->data + arrayList->elementTypeSize * index, 0, arrayList->elementTypeSize);
	}
----------------------------------------------------------------------------------------------------
*/
}


static void RemoveRange(ArrayList* arrayList, int fromIndex, int toIndex)
{
	CheckIndex("RemoveRange", fromIndex);
	CheckIndex("RemoveRange", toIndex);
	ALogA     (toIndex >=     fromIndex, "ArrayList RemoveRange toIndex must more than fromIndex");

	int num          = toIndex         - fromIndex + 1;
	int lastIndex    = arrayList->size - 1;

	arrayList->size -= num;

	if (toIndex != lastIndex)
	{
		void* to    = (char*) arrayList->elementArray->data + arrayList->elementTypeSize * fromIndex;
		void* from  = (char*) to                            + arrayList->elementTypeSize * num;

		// move between lastIndex and toIndex element
		memcpy(to, from, arrayList->elementTypeSize * (lastIndex - toIndex));

		// set last range elements 0, but it seems not very necessary
		// memset((char*) arrayList->array->data + arrayList->elementTypeSize * arrayList->size, 0, arrayList->elementTypeSize * num);

	}
/*
----------------------------------------------------------------------------------------------------
	else
	{
		// range reach the end, but it seems not very necessary
		// memset((char*) arrayList->array->data + arrayList->elementTypeSize * fromIndex, 0, arrayList->elementTypeSize * num);
	}
----------------------------------------------------------------------------------------------------
*/
}


static void RemoveByLast(ArrayList* arrayList, int index)
{
	CheckIndex("RemoveByLast", index);

	// now size equal last index
	arrayList->size--;

	if (index != arrayList->size)
	{
		memcpy
		(
			(char*) arrayList->elementArray->data + arrayList->elementTypeSize * index,
			(char*) arrayList->elementArray->data + arrayList->elementTypeSize * arrayList->size,
			arrayList->elementTypeSize
		);
	}
/*
----------------------------------------------------------------------------------------------------
	else
	{
		// remove the last element only set 0, but it seems not very necessary
		// memset((char*) arrayList->array->data + arrayList->elementTypeSize * index, 0, arrayList->elementTypeSize);
	}
----------------------------------------------------------------------------------------------------
*/
}


static void Clear(ArrayList* arrayList)
{
	arrayList->size = 0;

	// set data 0, but it seems not very necessary
	// memset(arrayList->array->data, 0, arrayList->elementTypeSize * arrayList->size);
}


static void Shrink(ArrayList* arrayList)
{
	if (arrayList->size == 0)
	{
		free(arrayList->elementArray->data);
		arrayList->elementArray->data   = NULL;
        arrayList->elementArray->length = 0;
	}
	else
	{
		void* data = realloc(arrayList->elementArray->data, arrayList->size * arrayList->elementTypeSize);
		ALogA(data, "ArrayList Shrink error, size = %d ", arrayList->size);

		arrayList->elementArray->data   = data;
		arrayList->elementArray->length = arrayList->size;
	}
}


static void Release(ArrayList* arrayList)
{
	free(arrayList->elementArray->data);
	arrayList->elementArray->data   = NULL;
    arrayList->elementArray->length = 0;
    arrayList->size                 = 0;
}


static void SetSize(ArrayList* arrayList, int size)
{
	arrayList->size = size;

	if (arrayList->elementArray->length >= size)
	{
		return;
	}

	AddCapacity(arrayList, size - arrayList->elementArray->length);
}


static void SetCapacity(ArrayList* arrayList, int capacity)
{
	if (arrayList->elementArray->length >= capacity)
	{
		return;
	}
	
	AddCapacity(arrayList, capacity - arrayList->elementArray->length);
}


static inline void InitArrayList(int elementTypeSize, ArrayList* arrayList)
{
	arrayList->elementArray->data   = NULL;
    arrayList->elementArray->length = 0;
	arrayList->elementTypeSize      = elementTypeSize;
	arrayList->size                 = 0;
	arrayList->increase             = 20;
}


static ArrayList* Create(int elementTypeSize)
{
	ArrayList* arrayList = (ArrayList*) malloc(sizeof(ArrayList));
	InitArrayList(elementTypeSize, arrayList);
	
	return arrayList;
}


static void init(int elementTypeSize, ArrayList* outArrayList)
{
	InitArrayList(elementTypeSize, outArrayList);
}


static ArrayList* CreateWithSize(int elementTypeSize, int size)
{
	ArrayList* arrayList = Create(elementTypeSize);
	SetSize(arrayList, size);

	return arrayList;
}


static void InitWithSize(int elementTypeSize, int size, ArrayList* outArrayList)
{
	InitArrayList(elementTypeSize, outArrayList);
	SetSize(outArrayList, size);
}


static ArrayList* CreateWithCapacity(int elementTypeSize, int capacity)
{
	ArrayList* arrayList = Create(elementTypeSize);
	SetCapacity(arrayList, capacity);
	
	return arrayList;
}


static void InitWithCapacity(int elementTypeSize, int capacity, ArrayList* outArrayList)
{
	InitArrayList(elementTypeSize, outArrayList);
	SetCapacity(outArrayList, capacity);
}


struct AArrayList AArrayList[1] =
{
	Create,
	init,
	CreateWithSize,
	InitWithSize,
	CreateWithCapacity,
	InitWithCapacity,

	Release,

	GetAdd,
	GetInsert,
	Add,
	Insert,
//	get,
//	set,
	Pop,
    AddArray,
	Remove,
	RemoveRange,
	RemoveByLast,
	Clear,
	Shrink,
	SetSize,
	SetCapacity,
};


#undef CheckIndex
#undef CheckInsertIndex
