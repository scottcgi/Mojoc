/*
 * Copyright (C) scott.cgi All Rights Reserved.
 *
 * Since  : 2014-12-22
 * Author : scott.cgi
 */

#ifndef struct_h
#define struct_h


#include <stddef.h>


/*
--------------------------------------------
   The macro can use generic parameter
--------------------------------------------
*/


/**
 * Get struct pointer from member pointer
 */
#define AStructGetParent2(memberPtr, structType) \
	((structType*) ((char*) memberPtr - offsetof(structType, memberPtr)))


/**
 * Get struct pointer from member pointer with memberName
 */
#define AStructGetParent3(memberPtr, structType, memberName) \
	((structType*) ((char*) memberPtr - offsetof(structType, memberName)))



#endif
