/*
 *
 *
 *  Created on: 2014-12-22
 *      Author: scott.cgi
 */

#ifndef struct_from_h
#define struct_from_h

#include <stddef.h>


//--------------------------------------------------
// The macro can use generic parameter 'structType'
//--------------------------------------------------

/**
 * Get struct pointer from member pointer
 */
#define StructFrom2(memberPtr, structType) \
	((structType*) ((char*) memberPtr - offsetof(structType, memberPtr)))


/**
 * Get struct pointer from member pointer
 */
#define StructFrom3(memberPtr, structType, memberName) \
	((structType*) ((char*) memberPtr - offsetof(structType, memberName)))



#endif
