/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The Mojoc game engine is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2017-4-21
 * Update: 2019-1-8
 * Author: scott.cgi
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
 * Set UserData slot value from slotIndex by int.
 */
static inline void AUserData_SetSlotInt(UserData* userData, int slotIndex, int value)
{
    userData->slots[slotIndex].intValue = value;
}


/**
 * Set UserData slot value from slotIndex by float.
 */
static inline void AUserData_SetSlotFloat(UserData* userData, int slotIndex, float value)
{
    userData->slots[slotIndex].floatValue = value;
}


/**
 * Set UserData slot value from slotIndex by ptr.
 */
static inline void AUserData_SetSlotPtr(UserData* userData, int slotIndex, void* value)
{
    userData->slots[slotIndex].ptrValue = value;
}


/**
 * Set UserData slot value from slotIndex by string.
 */
static inline void AUserData_SetSlotString(UserData* userData, int slotIndex, char* value)
{
    userData->slots[slotIndex].stringValue = value;
}


/**
 * Get UserData slot int value from slotIndex.
 */
static inline int AUserData_GetSlotInt(UserData* userData, int slotIndex)
{
    return userData->slots[slotIndex].intValue;
}


/**
 * Get UserData slot float value from slotIndex.
 */
static inline float AUserData_GetSlotFloat(UserData* userData, int slotIndex)
{
    return userData->slots[slotIndex].floatValue;
}


/**
 * Get UserData slot ptr value from slotIndex.
 */
static inline void* AUserData_GetSlotPtr(UserData* userData, int slotIndex)
{
    return userData->slots[slotIndex].ptrValue;
}


/**
 * Get UserData slot string value from slotIndex.
 */
static inline char* AUserData_GetSlotString(UserData* userData, int slotIndex)
{
    return userData->slots[slotIndex].stringValue;
}


/**
 * Get slot ptr value with PtrType.
 */
#define AUserData_GetSlotPtrWithType(userData, slotIndex, PtrType) \
    ((PtrType) (userData)->slots[slotIndex].ptrValue)

#endif
