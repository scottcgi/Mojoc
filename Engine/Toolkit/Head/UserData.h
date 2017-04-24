/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2017-4-21
 * Author: scott.cgi
 */

#ifndef user_data_h
#define user_data_h


#include <stddef.h>


typedef union
{
    int   intValue;
    float floatValue;
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


static inline void AUserDataInit(UserData* userData)
{
    userData->slot0->ptrValue = NULL;
    userData->slot1->ptrValue = NULL;
    userData->slot2->ptrValue = NULL;
    userData->slot3->ptrValue = NULL;
}


#endif
