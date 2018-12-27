/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2014-12-22
 * Author: scott.cgi
 */

#ifndef STRUCT_H
#define  STRUCT_H


#include <stddef.h>


/**
 * Get struct pointer from member pointer
 */
#define AStruct_GetParent2(memberPtr, structType) \
    ((structType*) ((char*) memberPtr - offsetof(structType, memberPtr)))


/**
 * Get struct pointer from member pointer with memberName
 */
#define AStruct_GetParent3(memberPtr, structType, memberName) \
    ((structType*) ((char*) memberPtr - offsetof(structType, memberName)))



#endif
