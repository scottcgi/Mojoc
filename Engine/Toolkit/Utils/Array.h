/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-2-1
 * Author : scott.cgi
 * Version: 0.1
 */


#ifndef ARRAY_H
#define ARRAY_H


#include "Engine/Toolkit/Head/MacroDefine.h"


typedef struct
{
	/**
	 * Elements memory space ptr
	 */
	void* data;

	/**
	 * Elements count
	 */
	int   length;
}
Array;


struct AArray
{
	/**
	 * Array struct space and elements space in one malloc
	 * the data ptr hold the offset of malloc return address
	 *
	 * elementTypeSize: sizeof element type
	 * length         : elements count
	 */
	Array* (*Create)(int elementTypeSize, int length);
};


extern struct AArray AArray[1];


/**
 * Marked Array element type
 *
 * elementType: element data type
 */
#define Array(elementType) Array


/**
 * Create Array composite literal
 *
 * elementType: element data type
 * length     : element count
 * ...        : each element
 */
#define AArray_Make(elementType, length, ...) \
	(Array[1])                                \
	{                                         \
		(elementType[length]) {__VA_ARGS__},  \
		length,                               \
	}


/**
 * Initialize constant Array
 *
 * elementType: element data type
 * length     : element count
 * ...        : each element
 */
#define AArray_Init(elementType, length, ...) \
	{                                         \
		(elementType[length]) {__VA_ARGS__},  \
		length,                               \
	}


/**
 * Get Array data patr
 *
 * array      : Array ptr
 * elementType: element data type
 */
#define AArray_GetData(array, elementType) \
	(elementType*) ((array)->data)


/**
 * Get element at index
 *
 * array      : Array ptr
 * index      : element index of array
 * elementType: element data type
 */
#define AArray_Get(array, index, elementType) \
	(AArray_GetData(array, elementType))[index]


/**
 * Get element ptr at index
 *
 * array      : Array ptr
 * index      : element index of array
 * elementType: element data type
 */
#define AArray_GetPtr(array, index, elementType) \
	(AArray_GetData(array, elementType) + (index))


/**
 * Set element at index
 *
 * array      : Array ptr
 * index      : element index of array
 * element    : variable
 * elementType: element data type
 */
#define AArray_Set(array, index, element, elementType) \
	AArray_Get(array, index, elementType) = element


#endif
