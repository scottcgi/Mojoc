/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License:
 * https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * Since : 2013-4-6
 * Update: 2019-19
 * Author: scott.cgi
 */


#ifndef BUFFER_READER_H
#define  BUFFER_READER_H


#include <stdbool.h>
#include <stdint.h>
#include "Engine/Toolkit/HeaderUtils/ArrayRange.h"


struct ABufferReader
{
    /**
     * Read long from buffer data, where ArrayRange in buffer.
     */
    int64_t (*ReadInt64)   (const char* buffer, ArrayRange* range);

    /**
     * Read int from buffer data, where ArrayRange in buffer.
     */
    int32_t (*ReadInt32)   (const char* buffer, ArrayRange* range);

    /**
     * Read short from buffer data, where ArrayRange in buffer.
     */
    int16_t (*ReadInt16)   (const char* buffer, ArrayRange* range);

    /**
     * Read byte from buffer data, where ArrayRange in buffer.
     */
    int8_t (*ReadInt8)     (const char* buffer, ArrayRange* range);

    /**
     * Read line from buffer, and record line in ArrayRange.
     */
    void   (*ReadLine)     (const char* buffer, ArrayRange* range, ArrayRange* outLine);

    /**
     * Find string in buffer and make range start after string.
     * return true found the string or false not found.
     */
    bool   (*TryFindString)(const char* buffer, ArrayRange* range, const char* str);
};


extern struct ABufferReader ABufferReader[1];


#endif
