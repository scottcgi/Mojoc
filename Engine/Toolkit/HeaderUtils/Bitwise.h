/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License:
 * https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * Since : 2013-3-14
 * Update: 2019-1-8
 * Author: scott.cgi
 */

 
#ifndef BITWISE_H
#define  BITWISE_H


/*
------------------------------------------------------------------------------------------------------------------------
The biteState is usually an expression,
but the inline function can not use expression for bitState,
so if not use macro we need pass the third parameter that's bitState owner.
------------------------------------------------------------------------------------------------------------------------
*/


#define ABitwise_Add(bitState, bits)   \
    ((bitState)  |= (bits))


#define ABitwise_Set(bitState, bits)   \
    ((bitState)   = (bits))


#define ABitwise_Clear(bitState, bits) \
    ((bitState)  &= ~(bits))


#define ABitwise_Check(bitState, bits) \
    (((bitState) & (bits)) != 0)


#define ABitwise_ClearAndAdd(bitState, clearBits, addBits) \
    ((bitState)  = ((bitState) & ~(clearBits)) | (addBits))


#define ABitwise_ClearAll(bitState) \
    ((bitState)  = 0)


#endif
