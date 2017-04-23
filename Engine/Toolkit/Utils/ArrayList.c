/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2013-2-4
 * Author : scott.cgi
 */

#include <string.h>
#include <stdlib.h>

#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Toolkit/Platform/Log.h"


#define CheckIndex(tag, index) \
	ALogA(((index) < arrayList->size && (index) >= 0),  "ArrayList " tag " error, " #index " = %d, size = %d", index, arrayList->size);


#define CheckInsertIndex(tag) \
	ALogA(((index) <= arrayList->size && (index) >= 0), "ArrayList " tag " error, index = %d, size = %d", index, arrayList->size);


static inline void AddCapacity(ArrayList* arrayList, int increase)
{
	ALogA(increase > 0, "ArrayList error, increase = %d can not <= 0", increase);

	void* data = realloc(arrayList->elementArray->data, (increase + arrayList->elementArray->length) * arrayList->typeSize);

	ALogA
	(
	    data != NULL,
		"ArrayList error, unable to realloc memory, size = %d, length = %d, increase = %d",
		arrayList->size, arrayList->elementArray->length, increase
	);

/*
----------------------------------------------------------------------------------------------------
	//set increase data to 0, but it seems not very necessary
	memset
	(
	    (char*) data + arrayList->typeSize * arrayList->array->length,
		 0,
		 arrayList->typeSize * increase
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

    return (char*) arrayList->elementArray->data + arrayList->typeSize * (arrayList->size++);
}


static void* GetInsert(ArrayList* arrayList, int index)
{
	// insert index can equal size
	CheckInsertIndex("GetInsert");

	if (arrayList->size == arrayList->elementArray->length)
	{
		AddCapacity(arrayList, arrayList->increase);
	}

	void* from  = (char*) arrayList->elementArray->data + arrayList->typeSize * index;
	void* to    = (char*) from                          + arrayList->typeSize;

	memmove(to, from, arrayList->typeSize * (arrayList->size - index));
	arrayList->size++;

	return from;
}


static void* Add(ArrayList* arrayList, void* elementPtr)
{
	return memcpy(GetAdd(arrayList), elementPtr, arrayList->typeSize);
}


static void* Insert(ArrayList* arrayList, int index, void* elementPtr)
{
	return memcpy(GetInsert(arrayList, index), elementPtr, arrayList->typeSize);
}


/*
----------------------------------------------------------------------------------------------------
// use macro instead
static void* Get(ArrayList* arrayList, int index)
{
	CheckIndex("Get", index);
	return (char*) arrayList->array->data + arrayList->typeSize * index;
}
----------------------------------------------------------------------------------------------------
*/


/*
----------------------------------------------------------------------------------------------------
// use macro instead
static void Set(ArrayList* arrayList, int index, void* elementPtr)
{
	CheckIndex("Set", index);
	memcpy((char*) arrayList->array->data + arrayList->typeSize * index, elementPtr, arrayList->typeSize);
}
----------------------------------------------------------------------------------------------------
*/


static void* Pop(ArrayList* arrayList, void* defaultElementPtr)
{
	if (arrayList->size > 0)
	{
		// now size equal last index
		return (char*) arrayList->elementArray->data + arrayList->typeSize * (--arrayList->size);
	}
	else
	{
		return defaultElementPtr;
	}
}


static void AddArray(ArrayList* arrayList, void* data, int length, int typeSize)
{
    ALogA
    (
        arrayList->typeSize == typeSize,
        "AddArray ArrayList typeSize %d must equal Array typeSize %d",
        arrayList->typeSize,
        typeSize
    );

    int n = arrayList->elementArray->length - arrayList->size - length;

    if (n < 0)
    {
        AddCapacity(arrayList, -n + arrayList->increase);
    }

    memcpy
    (
        (char*) arrayList->elementArray->data + arrayList->typeSize * arrayList->size,
        data,
        arrayList->typeSize * length
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
		void* to   = (char*) arrayList->elementArray->data + arrayList->typeSize * index;
		void* from = (char*) to                            + arrayList->typeSize;

		// move between index and last index element
		memmove(to, from, arrayList->typeSize * (arrayList->size - index));

		// set last element 0, but it seems not very necessary
		// memset((char*) arrayList->array->data + arrayList->typeSize * arrayList->size, 0, arrayList->typeSize);
	}
/*
----------------------------------------------------------------------------------------------------
	else
	{
		// remove the last element only set 0, but it seems not very necessary
		// memset((char*) arrayList->array->data + arrayList->typeSize * index, 0, arrayList->typeSize);
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
		void* to    = (char*) arrayList->elementArray->data + arrayList->typeSize * fromIndex;
		void* from  = (char*) to                            + arrayList->typeSize * num;

		// move between lastIndex and toIndex element
		memmove(to, from, arrayList->typeSize * (lastIndex - toIndex));

		// set last range elements 0, but it seems not very necessary
		// memset((char*) arrayList->array->data + arrayList->typeSize * arrayList->size, 0, arrayList->typeSize * num);

	}
/*
----------------------------------------------------------------------------------------------------
	else
	{
		// range reach the end, but it seems not very necessary
		// memset((char*) arrayList->array->data + arrayList->typeSize * fromIndex, 0, arrayList->typeSize * num);
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
			(char*) arrayList->elementArray->data + arrayList->typeSize * index,
			(char*) arrayList->elementArray->data + arrayList->typeSize * arrayList->size,
			arrayList->typeSize
		);
	}
/*
----------------------------------------------------------------------------------------------------
	else
	{
		// remove the last element only set 0, but it seems not very necessary
		// memset((char*) arrayList->array->data + arrayList->typeSize * index, 0, arrayList->typeSize);
	}
----------------------------------------------------------------------------------------------------
*/
}


static void Clear(ArrayList* arrayList)
{
	arrayList->size = 0;

	// set data 0, but it seems not very necessary
	// memset(arrayList->array->data, 0, arrayList->typeSize * arrayList->size);
}


static void Shrink(ArrayList* arrayList)
{
	if (arrayList->size == 0)
	{
		arrayList->elementArray->length = 0;
		free(arrayList->elementArray->data);
		arrayList->elementArray->data   = NULL;
	}
	else
	{
		void* data = realloc(arrayList->elementArray->data, arrayList->size * arrayList->typeSize);
		ALogA(data, "ArrayList Shrink error, size = %d ", arrayList->size);

		arrayList->elementArray->data   = data;
		arrayList->elementArray->length = arrayList->size;
	}
}


static void Release(ArrayList* arrayList)
{
	arrayList->size                 = 0;
	arrayList->elementArray->length = 0;
	free(arrayList->elementArray->data);
	arrayList->elementArray->data   = NULL;
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


static inline void InitArrayList(int typeSize, ArrayList* arrayList)
{
	arrayList->elementArray->length = 0;
	arrayList->elementArray->data   = NULL;
	arrayList->typeSize             = typeSize;
	arrayList->size                 = 0;
	arrayList->increase             = 20;
}


static ArrayList* Create(int typeSize)
{
	ArrayList* arrayList = (ArrayList*) malloc(sizeof(ArrayList));
	InitArrayList(typeSize, arrayList);
	
	return arrayList;
}


static void init(int typeSize, ArrayList* outArrayList)
{
	InitArrayList(typeSize, outArrayList);
}


static ArrayList* CreateWithSize(int typeSize, int size)
{
	ArrayList* arrayList = Create(typeSize);
	SetSize(arrayList, size);

	return arrayList;
}


static void InitWithSize(int typeSize, int size, ArrayList* outArrayList)
{
	InitArrayList(typeSize, outArrayList);
	SetSize(outArrayList, size);
}


static ArrayList* CreateWithCapacity(int typeSize, int capacity)
{
	ArrayList* arrayList = Create(typeSize);
	SetCapacity(arrayList, capacity);
	
	return arrayList;
}


static void InitWithCapacity(int typeSize, int capacity, ArrayList* outArrayList)
{
	InitArrayList(typeSize, outArrayList);
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
