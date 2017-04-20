/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2013-2-1
 * Author : scott.cgi
 */

#ifndef array_h
#define array_h


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
	 * typeSize: sizeof element type
	 * length  : elements count
	 */
	Array* (*Create)(int typeSize, int length);
};


extern struct AArray AArray[1];


/**
 * Marked Array element type
 *
 * type: element data type
 */
#define Array(type) Array


/**
 * Create Array composite literal
 *
 * type  : element data type
 * length: element count
 * ...   : each element
 */
#define AArrayMake(type, length, ...)    \
	(Array[1])                           \
	{                                    \
		(type[length]) {__VA_ARGS__},    \
		length,                          \
	}


/**
 * Initialize constant Array
 *
 * type  : element data type
 * length: element count
 * ...   : each element
 */
#define AArrayInit(type, length, ...)   \
	{                                   \
		(type[length]) {__VA_ARGS__},   \
		length,                         \
	}


/**
 * Get Array data patr
 *
 * array: Array ptr
 * type : element data type
 */
#define AArrayGetData(array, type) \
	(type*) ((array)->data)


/**
 * Get element at index
 *
 * array: Array ptr
 * index: element index of array
 * type : element data type
 */
#define AArrayGet(array, index, type) \
	(AArrayGetData(array, type))[index]


/**
 * Get element ptr at index
 *
 * array: Array ptr
 * index: element index of array
 * type : element data type
 */
#define AArrayGetPtr(array, index, type) \
	(AArrayGetData(array, type) + (index))


/**
 * Set element at index
 *
 * array  : Array ptr
 * index  : element index of array
 * element: variable
 * type   : element data type
 */
#define AArraySet(array, index, element, type) \
	AArrayGet(array, index, type) = element


#endif
