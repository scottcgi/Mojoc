/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
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


/**
 * Read different types of data from buffer.
 */
struct ABufferReader
{
    /**
     * Read long from range in buffer, and move range->start behind.
     */
    int64_t (*ReadInt64)   (const char* buffer, ArrayRange* range);

    /**
     * Read int from range in buffer, and move range->start behind.
     */
    int32_t (*ReadInt32)   (const char* buffer, ArrayRange* range);

    /**
     * Read short from range in buffer, and move range->start behind.
     */
    int16_t (*ReadInt16)   (const char* buffer, ArrayRange* range);

    /**
     * Read byte from range in buffer, and move range->start behind.
     */
    int8_t (*ReadInt8)     (const char* buffer, ArrayRange* range);

    /**
     * Read line from range in buffer, move range->start to newline, and record line in outLine.
     * the outLine->end may be '\n' or '\r' or range->end (not found newline char).
     */
    void   (*ReadLine)     (const char* buffer, ArrayRange* range, ArrayRange* outLine);

    /**
     * Find str from range in buffer.
     *
     * if return true
     *     found the str and move range->start behind str in buffer, but not more than range->end.
     * else
     *     not found str and keep range->start.
     */
    bool   (*TryFindString)(const char* buffer, ArrayRange* range, const char* str);
};


extern struct ABufferReader ABufferReader[1];


#endif
