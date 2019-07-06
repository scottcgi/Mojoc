/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License.
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub.
 * This Mojoc game engine is a serious project with extreme code pursuit, and will continue to iterate.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2013-3-14
 * Update : 2019-1-8
 * Author : scott.cgi
 */

 
#ifndef BITWISE_H
#define BITWISE_H


/*
The biteState is usually an expression,
but the inline function cannot use expression for bitState,
so if not use macro we need pass the third parameter that is the bitState owner.
*/



/**
 * Add bits into biteState.
 */
#define ABitwise_Add(bitState, bits)   \
    ((bitState)  |= (bits))


/**
 * Set biteState bits.
 */
#define ABitwise_Set(bitState, bits)   \
    ((bitState)   = (bits))


/**
 * Clear bits from bitState.
 */
#define ABitwise_Clear(bitState, bits) \
    ((bitState)  &= ~(bits))

/**
 * Check biteState whether has bites.
 */
#define ABitwise_Check(bitState, bits) \
    (((bitState) & (bits)) != 0)


/**
 * Clear clearBits from bitState, and add addBits into biteState.
 */
#define ABitwise_ClearAndAdd(bitState, clearBits, addBits) \
    ((bitState)  = ((bitState) & ~(clearBits)) | (addBits))


/**
 * Reset bitState to 0.
 */
#define ABitwise_ClearAll(bitState) \
    ((bitState)  = 0)


#endif
