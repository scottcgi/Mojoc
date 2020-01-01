/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2013-2-1
 * Update   : 2017-1-17
 * Author   : scott.cgi
 */


#ifndef ARRAY_H
#define ARRAY_H


#include "Engine/Toolkit/HeaderUtils/Define.h"


/**
 * The array with length.
 */
typedef struct
{
    /**
     * Elements memory space ptr.
     * if use "void data[]" will not be able to set the data manually.
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
 * ElementType: element data type
 */
#define Array(ElementType) Array


/**
 * Construct Array composite literal.
 *
 * ElementType: element data type
 * length     : elements count
 * ...        : each elements
 */
#define AArray_Make(ElementType, length, ...) \
    (Array[1])                                \
    {                                         \
        (ElementType[length]) {__VA_ARGS__},  \
        length,                               \
    }


/**
 * Construct Array composite literal.
 *
 * data  : elements data
 * length: elements count
 */
#define AArray_MakeWithData(data, length) \
    (Array[1]) {data, length}


/**
 * Init constant Array.
 *
 * ElementType: element data type
 * length     : elements count
 * ...        : each elements
 */
#define AArray_Init(ElementType, length, ...) \
    {                                         \
        (ElementType[length]) {__VA_ARGS__},  \
        length,                               \
    }


/**
 * Get element at index.
 *
 * array      : Array ptr
 * index      : element index of array
 * ElementType: element data type
 */
#define AArray_Get(array, index, ElementType) \
    ((ElementType*) (array)->data)[index]


/**
 * Get element ptr at index.
 *
 * array      : Array ptr
 * index      : element index of array
 * ElementType: element data type
 */
#define AArray_GetPtr(array, index, ElementType) \
    ((ElementType*) (array)->data + (index))


/**
 * Set element at index.
 *
 * array      : Array ptr
 * index      : element index of array
 * element    : element value
 * ElementType: element data type
 */
#define AArray_Set(array, index, element, ElementType) \
    AArray_Get(array, index, ElementType) = element


#endif
