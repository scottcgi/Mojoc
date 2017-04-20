/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2013-3-14
 * Author : scott.cgi
 */

#ifndef bitwise_h
#define bitwise_h


/*
-------------------------------------------------------------------------------
	The biteState is usually an expression
	the inline function can not use expression for bitState
	so if not use macro we need pass the third parameter that's bitState owner
-------------------------------------------------------------------------------
*/


#define ABitwiseSet(bitState,     bits) \
	((bitState)  |= (bits))


#define ABitwiseSetOnly(bitState, bits) \
    ((bitState)   = (bits))


#define ABitwiseClear(bitState,   bits) \
	((bitState)  &= ~(bits))


#define ABitwiseCheck(bitState,   bits) \
	(((bitState) & (bits)) != 0)


#define ABitwiseClearAndSet(bitState, clearBits, setBits) \
	((bitState)  = ((bitState) & ~(clearBits)) | (setBits))


#endif
