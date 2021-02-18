/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2014-12-22
 * Update   : 2018-1-8
 * Author   : scott.cgi
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
 * memberPtr: the pointer that point struct's member.
 *            it's the member address offset from struct address.
 * 
 */
#define AStruct_GetParent(memberPtr, ParentType) \
    ((ParentType*) ((char*) (memberPtr) - offsetof(ParentType, memberPtr)))


/**
 * Get struct pointer from member pointer with memberName.
 * this for memberPtr not same as memberName.
 *
 * memberPtr: the pointer that point struct's member.
 *            it's the member address offset from struct address.
 */
#define AStruct_GetParentWithName(memberPtr, ParentType, memberName) \
    ((ParentType*) ((char*) (memberPtr) - offsetof(ParentType, memberName)))


#endif
