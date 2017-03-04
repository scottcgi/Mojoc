/*
 *
 *
 *  Created on: 2013-2-1
 *  Author: scott.cgi
 */

#ifndef array_h
#define array_h

typedef struct
{
	/** Elements memory data */
	void* data;
	/** Elements count */
	int   length;
}
Array;


struct AArray
{
	/**
	 * Array space and data space in one malloc
	 * Array data hold the offset of malloc return address
	 */
	Array* (*Create)(int typeSize, int length);
};

extern struct AArray AArray[1];


/** The type is the array element type */
#define Array(type) Array


/**
 * Create Array composite literal, macro can use type and varargs parameter
 */
#define AArrayMake(type, length, ...)    \
	(Array[1])                           \
	{                                    \
		(type[length]) {__VA_ARGS__},    \
		length,                          \
	}


/**
 * Initialize constant Array, macro can use type and varargs parameter
 */
#define AArrayInit(type, length, ...)   \
	{                                   \
		(type[length]) {__VA_ARGS__},   \
		length,                         \
	}


/**
 * The type is element type，macro can use type parameter
 */
#define AArrayGetData(array, type) \
	(type*) ((array)->data)


/**
 * Get element, macro can use type parameter
 */
#define AArrayGet(array, index, type) \
	(AArrayGetData(array, type))[index]


/**
 * Get element ptr, macro can use type parameter
 */
#define AArrayGetPtr(array, index, type) \
	(AArrayGetData(array, type) + (index))


/**
 * Set element, macro can use type parameter
 */
#define AArraySet(array, index, element, type) \
	AArrayGet(array, index, type) = element

#endif
