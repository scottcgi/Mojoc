/*
 *
 *
 *  Created on: 2013-4-6
 *      Author: scott.cgi
 */

#ifndef buffer_reader_h
#define buffer_reader_h

#include <stdbool.h>
#include <stdint.h>

#include "Engine/Toolkit/Head/ArrayRange.h"

struct ABufferReader
{
	/**
	 * Read long from buffer data, where ArrayRange in buffer
	 */
	int64_t (*ReadInt64)   (const char* buffer, ArrayRange* range);

	/**
	 * Read int from buffer data, where ArrayRange in buffer
	 */
	int32_t (*ReadInt32)   (const char* buffer, ArrayRange* range);

	/**
	 * Read short from buffer data, where ArrayRange in buffer
	 */
	int16_t (*ReadInt16)   (const char* buffer, ArrayRange* range);

	/**
	 * Read byte from buffer data, where ArrayRange in buffer
	 */
	int8_t (*ReadInt8)     (const char* buffer, ArrayRange* range);

	/**
	 * Read line from buffer data, where ArrayRange in buffer
	 * and record line ArrayRange
	 */
	void   (*ReadLine)     (char* buffer, ArrayRange* range, ArrayRange* outLine);

    /**
     * Find string in buffer and make range start after string
     * return true found string or false not found
     */
	bool   (*TryFindString)(char* buffer, ArrayRange* range, const char* str);
};

extern struct ABufferReader ABufferReader[1];

#endif
