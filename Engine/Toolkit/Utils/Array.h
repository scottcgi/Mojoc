/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-2-1
 * Updata: 2017-1-17
 * Author: scott.cgi
 */


#ifndef ARRAY_H
#define  ARRAY_H


#include "Engine/Toolkit/HeaderUtils/Define.h"


/**
 * The array with length.
 */
typedef struct
{
    /**
     * Elements memory space ptr.
     */
    void* data;

    /**
     * Elements count.
     */
    int   length;
}
Array;


/**
 * Control Array.
 */
struct AArray
{
    /**
     * Array struct space and elements space in one malloc.
     * the data ptr store the offset of malloc return address.
     *
     * elementTypeSize: sizeof element type
     * length         : elements count
     */
    Array* (*Create)(int elementTypeSize, int length);
};


extern struct AArray AArray[1];


/**
 * Marked Array element type.
 * elementType: element data type
 */
#define Array(elementType) Array


/**
 * Create Array composite literal.
 *
 * elementType: element data type
 * length     : elements count
 * ...        : each elements
 */
#define AArray_Make(elementType, length, ...) \
    (Array[1])                               \
    {                                        \
        (elementType[length]) {__VA_ARGS__}, \
        length,                              \
    }


/**
 * Init constant Array.
 *
 * elementType: element data type
 * length     : elements count
 * ...        : each elements
 */
#define AArray_Init(elementType, length, ...) \
    {                                        \
        (elementType[length]) {__VA_ARGS__}, \
        length,                              \
    }


/**
 * Get Array data ptr with type.
 *
 * array      : Array ptr
 * elementType: element data type
 */
#define AArray_GetData(array, elementType) \
    (elementType*) ((array)->data)


/**
 * Get element at index.
 *
 * array      : Array ptr
 * index      : element index of array
 * elementType: element data type
 */
#define AArray_Get(array, index, elementType) \
    (AArray_GetData(array, elementType))[index]


/**
 * Get element ptr at index.
 *
 * array      : Array ptr
 * index      : element index of array
 * elementType: element data type
 */
#define AArray_GetPtr(array, index, elementType) \
    (AArray_GetData(array, elementType) + (index))


/**
 * Set element at index.
 *
 * array      : Array ptr
 * index      : element index of array
 * element    : element value
 * elementType: element data type
 */
#define AArray_Set(array, index, element, elementType) \
    AArray_Get(array, index, elementType) = element


#endif
