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
 * Since    : 2017-4-21
 * Update   : 2019-1-8
 * Author   : scott.cgi
 */

 
#ifndef USER_DATA_H
#define USER_DATA_H


#include <stddef.h>


/**
 * A data slot in UserData that can stores one type of data in different types.
 */
typedef union
{
    int   intValue;
    float floatValue;
    void* ptrValue;
    char* stringValue;
}
UserDataSlot;


/**
 * Store user custom data in UserDataSlot array.
 */
typedef struct
{
    UserDataSlot slots[4];
}
UserData;


/**
 * Init UserData all slot data.
 */
static inline void AUserData_Init(UserData* userData)
{
    userData->slots[0].ptrValue = NULL;
    userData->slots[1].ptrValue = NULL;
    userData->slots[2].ptrValue = NULL;
    userData->slots[3].ptrValue = NULL;
}


/**
 * Set int value of UserData slot at slotIndex.
 */
static inline void AUserData_SetSlotInt(UserData* userData, int slotIndex, int value)
{
    userData->slots[slotIndex].intValue = value;
}


/**
 * Add int value of UserData slot at slotIndex.
 */
static inline void AUserData_AddSlotInt(UserData* userData, int slotIndex, int value)
{
    userData->slots[slotIndex].intValue += value;
}


/**
 * Set float value of UserData slot at slotIndex.
 */
static inline void AUserData_SetSlotFloat(UserData* userData, int slotIndex, float value)
{
    userData->slots[slotIndex].floatValue = value;
}


/**
 * Add float value of UserData slot at slotIndex.
 */
static inline void AUserData_AddSlotFloat(UserData* userData, int slotIndex, float value)
{
    userData->slots[slotIndex].floatValue += value;
}


/**
 * Set ptr value of UserData slot at slotIndex.
 */
static inline void AUserData_SetSlotPtr(UserData* userData, int slotIndex, void* value)
{
    userData->slots[slotIndex].ptrValue = value;
}


/**
 * Set string value of UserData slot at slotIndex.
 */
static inline void AUserData_SetSlotString(UserData* userData, int slotIndex, char* value)
{
    userData->slots[slotIndex].stringValue = value;
}


/**
 * Get int value of UserData slot at slotIndex.
 */
static inline int AUserData_GetSlotInt(UserData* userData, int slotIndex)
{
    return userData->slots[slotIndex].intValue;
}


/**
 * Get float value of UserData slot at slotIndex.
 */
static inline float AUserData_GetSlotFloat(UserData* userData, int slotIndex)
{
    return userData->slots[slotIndex].floatValue;
}


/**
 * Get ptr value of UserData slot at slotIndex.
 */
static inline void* AUserData_GetSlotPtr(UserData* userData, int slotIndex)
{
    return userData->slots[slotIndex].ptrValue;
}


/**
 * Get string value of UserData slot at slotIndex.
 */
static inline char* AUserData_GetSlotString(UserData* userData, int slotIndex)
{
    return userData->slots[slotIndex].stringValue;
}


/**
 * Get ptr value of UserData slot at slotIndex with PtrType.
 */
#define AUserData_GetSlotPtrWithType(userData, slotIndex, PtrType) \
    ((PtrType) (userData)->slots[slotIndex].ptrValue)

#endif
