/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2017-4-21
 * Author: scott.cgi
 */

#ifndef USER_DATA_H
#define  USER_DATA_H


#include <stddef.h>


typedef union
{
    int   intValue;
    float  floatValue;
    void* ptrValue;
    char* stringValue;
}
UserDataSlot;


typedef struct
{
    UserDataSlot slot0[1];
    UserDataSlot slot1[1];
    UserDataSlot slot2[1];
    UserDataSlot slot3[1];
}
UserData;


static inline void AUserData_Init(UserData* userData)
{
    userData->slot0->ptrValue = NULL;
    userData->slot1->ptrValue = NULL;
    userData->slot2->ptrValue = NULL;
    userData->slot3->ptrValue = NULL;
}


#endif
