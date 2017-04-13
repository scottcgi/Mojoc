/*
 *
 *
 *  Created on: 2013-2-4
 *  Author: scott.cgi
 */

#ifndef array_list_h
#define array_list_h

#include "Engine/Toolkit/Utils/Array.h"


typedef struct
{
	/**
	 * Increase memory space, default 10
	 */
	int            increase;

	/**
	 * Hold memory data, the length is memory capacity
	 * if increase capacity, memory data will realloc
	 * so the data address may changed
	 */
	Array          array[1];

	/**
	 * The sizeof element type
	 */
	int	  get_only typeSize;

	/**
	 * Element count
	 */
	int   get_only size;
}
ArrayList;


struct AArrayList
{
	ArrayList* (*Create)            (int typeSize);
	void       (*Init)              (int typeSize, ArrayList* out_param arrayList);

	ArrayList* (*CreateWithSize)    (int typeSize, int size);
	void       (*InitWithSize)      (int typeSize, int size, ArrayList* out_param arrayList);

	ArrayList* (*CreateWithCapacity)(int typeSize, int capacity);
	void       (*InitWithCapacity)  (int typeSize, int capacity, ArrayList* out_param arrayList);

	/**
	 * Release member memory space
	 */
	void       (*Release)           (ArrayList*  arrayList);

	/**
	 * Increase one element memory (typeSize data) appended to ArrayList
	 * return elementPtr point the increase element
	 */
	void*      (*GetAdd)            (ArrayList* arrayList);

	/**
	 * Increase one element memory (typeSize data) at the index
	 * and move index behind elements one position
	 *
	 * index is which in [0, size - 1]
	 * return elementPtr point the increase element
	 */
	void*      (*GetInsert)         (ArrayList* arrayList, int index);

	/**
	 * Copy element data into getAdd memory
	 * elementPtr point to element
	 *
	 * return elementPtr in ArrayList
	 */
	void*      (*Add)               (ArrayList* arrayList, void* elementPtr);

	/**
	 * Copy element data into getInsert memory
	 * elementPtr point to element
	 *
	 * return elementPtr in ArrayList
	 */
	void*      (*Insert)            (ArrayList* arrayList, int index, void* elementPtr);

	/**
	 * Remove last element and return removed elementPtr
	 * if empty return defaultElementPtr
	 */
	void*      (*Pop)               (ArrayList* arrayList, void* defaultElementPtr);

	/**
	 * Add copy of array data
	 */
	void       (*AddArray)          (ArrayList* arrayList, void* data, int length, int typeSize);

	/**
	 * Remove index of element and move behind elements
	 */
	void       (*Remove)            (ArrayList* arrayList, int index);

	/**
	 * Remove a range of elements
	 */
	void       (*RemoveRange)       (ArrayList* arrayList, int fromIndex, int toIndex);

	/**
	 * Remove index of element and replace last element to the index
	 */
	void       (*RemoveByLast)      (ArrayList* arrayList, int index);

	/**
	 * Clear size to 0 and memory data rest to 0, keep memory space
	 */
	void       (*Clear)             (ArrayList* arrayList);

	/**
	 * Release not used memory data which is after ArrayList size
	 * if ArrayList size is 0 will be free all memory data
	 */
	void       (*Shrink)            (ArrayList* arrayList);

	/**
	 * Set ArrayList size and unused memory initialized 0
	 */
	void       (*SetSize)           (ArrayList* arrayList, int size);

	/**
	 * Set ArrayList capacity and unused memory initialized 0
	 */
	void       (*SetCapacity)       (ArrayList* arrayList, int capacity);
};


extern struct AArrayList AArrayList[1];


/**
 * The type is the ArrayList element type
 */
#define ArrayList(type) ArrayList


/**
 * Initialize constant ArrayList, macro can use type parameter
 */
#define AArrayListInit(type, increase) \
	{                                  \
		increase,                      \
		{                              \
			NULL,                      \
			0,                         \
		},                             \
		sizeof(type),                  \
		0,                             \
	}


/**
 * Initialize constant ArrayList with fixed capacity, unable to expansion capacity
 */
#define AArayListInitFix(type, capacity, size, ...) \
	{                                               \
		0,                                          \
		AArrayInit(type, capacity, __VA_ARGS__),    \
		sizeof(type),                               \
		size,                                       \
	}


/**
 * Shortcut of AArrayList->GetAdd
 * return element
 */
#define AArrayListGetAdd(arrayList, type) \
	(*(type*) AArrayList->GetAdd(arrayList))


/**
 * Shortcut of AArrayList->GetAdd
 * return elementPtr
 */
#define AArrayListGetPtrAdd(arrayList, type) \
	((type*) AArrayList->GetAdd(arrayList))


/**
 * Shortcut of AArrayList->GetInsert
 * return element
 */
#define AArrayListGetInsert(arrayList, index, type) \
	(*(type*) AArrayList->GetInsert(arrayList, index))


/**
 * Shortcut of AArrayList->GetInsert
 * return elementPtr
 */
#define AArrayListGetPtrInsert(arrayList, index, type) \
	((type*) AArrayList->GetInsert(arrayList, index))


/**
 * Shortcut of AArrayList->Add
 */
#define AArrayListAdd(arrayList, element) \
	AArrayList->Add(arrayList, &(element))


/**
 * Shortcut of AArrayList->Insert
 */
#define AArrayListInsert(arrayList, index, element) \
	AArrayList->Insert(arrayList, index, &(element))


/**
 * The type is element type
 */
#define AArrayListGetData(arrayList, type) \
	((type*) ((arrayList)->array->data))


/**
 * Instead of AArrayList->get for quick iterate element
 * return element
 */
#define AArrayListGet(arrayList, index, type) \
	(AArrayListGetData(arrayList, type))[index]


/**
 * Instead of AArrayList->get for quick iterate element
 * return elementPtr
 */
#define AArrayListGetPtr(arrayList, index, type) \
	(AArrayListGetData(arrayList, type) + (index))


/**
 * Instead of AArrayList->set for quick set element
 */
#define AArrayListSet(arrayList, index, element, type) \
	AArrayListGet(arrayList, index, type) = element


/**
 * Shortcut of AArrayList->Pop
 * return element
 */
#define AArrayListPop(arrayList, type) \
	(*(type*) AArrayList->Pop(arrayList, null_ptr))


/**
 * Shortcut of AArrayList->Pop
 * return element
 */
#define AArrayListPopWithDefault(arrayList, type, defaultValue) \
	(*(type*) AArrayList->Pop(arrayList, &(defaultValue)))


#endif
