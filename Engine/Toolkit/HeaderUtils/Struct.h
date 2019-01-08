/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License:
 * https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * Since : 2014-12-22
 * UpDate: 2018-1-8
 * Author: scott.cgi
 */

 
#ifndef STRUCT_H
#define  STRUCT_H


#include <stddef.h>


/**
 * Get struct pointer from member pointer.
 */
#define AStruct_GetParent2(memberPtr, structType) \
    ((structType*) ((char*) memberPtr - offsetof(structType, memberPtr)))


/**
 * Get struct pointer from member pointer with memberName.
 */
#define AStruct_GetParent3(memberPtr, structType, memberName) \
    ((structType*) ((char*) memberPtr - offsetof(structType, memberName)))


#endif
