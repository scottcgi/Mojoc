/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-2-4
 * Update: 2019-1-17
 * Author: scott.cgi
 */


#ifndef ARRAY_LIST_H
#define  ARRAY_LIST_H


#include "Engine/Toolkit/Utils/Array.h"


/**
 * The list can dynamically increase memory capacity that implemented by array.
 */
typedef struct
{
    /**
     * Increase memory space when needed, default 10.
     */
    int                increase;

    /**
     * The sizeof element type.
     */
    int                elementTypeSize;

    /**
     * Elements count.
     */
    int                size;

    /**
     * Stores memory data, the length is memory capacity.
     * if increase capacity, memory data will realloc,
     * so the data address may changed.
     */
    Array(elementType) elementArr[1];
}
ArrayList;


/**
 * Control ArrayList.
 */
struct AArrayList
{
    ArrayList* (*Create)            (int elementTypeSize);
    void       (*Init)              (int elementTypeSize, ArrayList* outArrayList);

    ArrayList* (*CreateWithSize)    (int elementTypeSize, int size);
    void       (*InitWithSize)      (int elementTypeSize, int size, ArrayList* outArrayList);

    ArrayList* (*CreateWithCapacity)(int elementTypeSize, int capacity);
    void       (*InitWithCapacity)  (int elementTypeSize, int capacity, ArrayList* outArrayList);

    void       (*Release)           (ArrayList* arrayList);

    /**
     * Increase one element memory (elementTypeSize data) appended to ArrayList.
     * return elementPtr point the increase element.
     */
    void*      (*GetAdd)            (ArrayList* arrayList);

    /**
     * Increase one element memory (elementTypeSize data) at the index,
     * and move index behind elements one position.
     *
     * index: range in [0, size - 1]
     *
     * return elementPtr point the increase element.
     */
    void*      (*GetInsert)         (ArrayList* arrayList, int index);

    /**
     * Copy element data into getAdd memory.
     * elementPtr: point to element
     *
     * return elementPtr in ArrayList.
     */
    void*      (*Add)               (ArrayList* arrayList, void* elementPtr);

    /**
     * Copy element data into getInsert memory.
     * elementPtr: point to element
     *
     * return elementPtr in ArrayList.
     */
    void*      (*Insert)            (ArrayList* arrayList, int index, void* elementPtr);

    /**
     * Remove last element and return removed elementPtr.
     * if empty return defaultElementPtr.
     */
    void*      (*Pop)               (ArrayList* arrayList, void* defaultElementPtr);

    /**
     * Add copy of array data.
     */
    void       (*AddArray)          (ArrayList* arrayList, void* data, int length, int elementTypeSize);

    /**
     * Remove index of element and move behind elements.
     */
    void       (*Remove)            (ArrayList* arrayList, int index);

    /**
     * Remove a range [fromIndex, toIndex] of elements.
     */
    void       (*RemoveRange)       (ArrayList* arrayList, int fromIndex, int toIndex);

    /**
     * Remove index of element and replace last element to the index. (no need to move any elements)
     */
    void       (*RemoveByLast)      (ArrayList* arrayList, int index);

    /**
     * Clear size to 0, keep memory space.
     */
    void       (*Clear)             (ArrayList* arrayList);

    /**
     * Release not used memory data which is after ArrayList size.
     * if ArrayList size is 0 will be free all memory data.
     */
    void       (*Shrink)            (ArrayList* arrayList);

    /**
     * Set ArrayList size and increase capacity if needed.
     */
    void       (*SetSize)           (ArrayList* arrayList, int size);

    /**
     * Set ArrayList capacity and increase capacity if needed.
     */
    void       (*SetCapacity)       (ArrayList* arrayList, int capacity);
};


extern struct AArrayList AArrayList[1];


/**
 * Marked ArrayList element type.
 * elementType: element data type
 */
#define ArrayList(elementType) ArrayList


/**
 * Init constant ArrayList.
 * example: ArrayList list[1] = AArrayList_Init(elementType, increase)
 */
#define AArrayList_Init(elementType, increase)  \
    {                                          \
        increase,                              \
        sizeof(elementType),                   \
        0,                                     \
        {                                      \
            NULL,                              \
            0,                                 \
        },                                     \
    }


/**
 * Init constant ArrayList with fixed capacity, unable to expansion capacity.
 * example: ArrayList list[1] = AArayList_InitFixed(elementType, capacity, size, ...)
 */
#define AArayList_InitFixed(elementType, capacity, size, ...) \
    {                                                        \
        0,                                                   \
        sizeof(elementType),                                 \
        size,                                                \
        AArray_Init(elementType, capacity, __VA_ARGS__),     \
    }


/**
 * Shortcut of AArrayList->GetAdd.
 * return element.
 */
#define AArrayList_GetAdd(arrayList, elementType) \
    (*(elementType*) AArrayList->GetAdd(arrayList))


/**
 * Shortcut of AArrayList->GetAdd.
 * return elementPtr.
 */
#define AArrayList_GetPtrAdd(arrayList, elementType) \
    ((elementType*) AArrayList->GetAdd(arrayList))


/**
 * Shortcut of AArrayList->GetInsert.
 * return element.
 */
#define AArrayList_GetInsert(arrayList, index, elementType) \
    (*(elementType*) AArrayList->GetInsert(arrayList, index))


/**
 * Shortcut of AArrayList->GetInsert.
 * return elementPtr.
 */
#define AArrayList_GetPtrInsert(arrayList, index, elementType) \
    ((elementType*) AArrayList->GetInsert(arrayList, index))


/**
 * Shortcut of AArrayList->Add.
 */
#define AArrayList_Add(arrayList, element) \
    AArrayList->Add(arrayList, &(element))


/**
 * Shortcut of AArrayList->Insert.
 */
#define AArrayList_Insert(arrayList, index, element) \
    AArrayList->Insert(arrayList, index, &(element))


/**
 * Get the type ptr of elements.
 */
#define AArrayList_GetData(arrayList, elementType) \
    ((elementType*) ((arrayList)->elementArr->data))


/**
 * Get the element with type.
 */
#define AArrayList_Get(arrayList, index, elementType) \
    (AArrayList_GetData(arrayList, elementType))[index]


/**
 * Get the elementPtr with type.
 */
#define AArrayList_GetPtr(arrayList, index, elementType) \
    (AArrayList_GetData(arrayList, elementType) + (index))


/**
 * Shortcut of AArrayList->Pop.
 * return element.
 */
#define AArrayList_PopWithDefault(arrayList, elementType, defaultValue) \
    (*(elementType*) AArrayList->Pop(arrayList, &(defaultValue)))


/**
 * Set the element at index.
 */
#define AArrayList_Set(arrayList, index, element, elementType) \
    AArrayList_Get(arrayList, index, elementType) = element


/**
 * Shortcut of AArrayList->Pop
 * return element
 */
#define AArrayList_Pop(arrayList, elementType) \
    (*(elementType*) AArrayList->Pop(arrayList, NULL_PTR))

#endif
