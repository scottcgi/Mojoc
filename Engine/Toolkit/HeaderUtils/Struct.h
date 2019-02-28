/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2014-12-22
 * UpDate: 2018-1-8
 * Author: scott.cgi
 */

 
#ifndef STRUCT_H
#define STRUCT_H


#include <stddef.h>

/*
 Examples:
     struct Parent
     {
         int  arr[1];
         int  value;
         int* ptr;
     }

     Parent parent[1];

     output true:
     AStruct_GetParentWithName(parent->arr,    Parent, arr)   == parent
     AStruct_GetParentWithName(&parent->arr,   Parent, arr)   == parent
     AStruct_GetParentWithName(&parent->value, Parent, value) == parent
     AStruct_GetParentWithName(&parent->ptr,   Parent, ptr)   == parent

     arr      = parent->arr
     value    = parent->value
     ptr      = parent->ptr
     valuePtr = &parent->value
     ptrPtr   = &parent->ptr

     output true:
     AStruct_GetParent(arr,      Parent) == parent
     AStruct_GetParent(&arr,     Parent) == parent
     AStruct_GetParent(valuePtr, Parent) == parent
     AStruct_GetParent(ptrPtr,   Parent) == parent

     output false:
     AStruct_GetParent(value,    Parent) == parent
     AStruct_GetParent(ptr,      Parent) == parent
     AStruct_GetParent(&value,   Parent) == parent
     AStruct_GetParent(&ptr,     Parent) == parent
*/


/**
 * Get struct pointer from member pointer.
 * this for memberPtr same as memberName.
 * 
 * memberPtr：the pointer that point struct's member.
 *           it's the member address offset from struct address.
 * 
 */
#define AStruct_GetParent(memberPtr, ParentType) \
    ((ParentType*) ((char*) (memberPtr) - offsetof(ParentType, memberPtr)))


/**
 * Get struct pointer from member pointer with memberName.
 * this for memberPtr not same as memberName.
 *
 * memberPtr：the pointer that point struct's member.
 *           it's the member address offset from struct address.
 */
#define AStruct_GetParentWithName(memberPtr, ParentType, memberName) \
    ((ParentType*) ((char*) (memberPtr) - offsetof(ParentType, memberName)))


#endif
