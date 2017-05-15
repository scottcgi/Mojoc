/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-3-14
 * Author : scott.cgi
 * Version: 0.1
 */


#ifndef BITWISE_H
#define BITWISE_H


/*
-------------------------------------------------------------------------------
	The biteState is usually an expression
	the inline function can not use expression for bitState
	so if not use macro we need pass the third parameter that's bitState owner
-------------------------------------------------------------------------------
*/


#define ABitwise_Set(bitState,     bits) \
	((bitState)  |= (bits))


#define ABitwise_SetOnly(bitState, bits) \
    ((bitState)   = (bits))


#define ABitwise_Clear(bitState,   bits) \
	((bitState)  &= ~(bits))


#define ABitwise_Check(bitState,   bits) \
	(((bitState) & (bits)) != 0)


#define ABitwise_ClearAndSet(bitState, clearBits, setBits) \
	((bitState)  = ((bitState) & ~(clearBits)) | (setBits))


#endif
