/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2013-4-6
 * Author: scott.cgi
 */

#include "Engine/Toolkit/Utils/BufferReader.h"
#include "Engine/Toolkit/Platform/Log.h"


#define CheckIndex(tag) \
	ALogA(range->start <= range->end, "ArrayRange " tag " start[%d] > end[%d]", range->start, range->end)


static int64_t ReadInt64(char* buffer, ArrayRange* range)
{
	CheckIndex("ReadInt64");
	
	int   pos     = range->start;
	char* data    = buffer + pos;

	range->start += sizeof(int64_t);
	
	return 	(
				((int64_t)(data[0] & 0xff) << 56) +
				((int64_t)(data[1] & 0xff) << 48) +
				((int64_t)(data[2] & 0xff) << 40) +
				((int64_t)(data[3] & 0xff) << 32) +
				((int64_t)(data[4] & 0xff) << 24) +
				(         (data[5] & 0xff) << 16) +
				(         (data[6] & 0xff) <<  8) +
				(         (data[7] & 0xff) <<  0)
			);
}


/**
 * Read int from buffer data, where ArrayRange in buffer
 */
static int32_t ReadInt32(char* buffer, ArrayRange* range)
{
	CheckIndex("ReadInt32");
	
	int   pos        = range->start;
	char* data       = buffer + pos;

    int   ch1        = data[0];
    int   ch2        = data[1];
    int   ch3        = data[2];
    int   ch4        = data[3];

	range->start    += sizeof(int32_t);

	return ((ch1 << 24) + (ch2 << 16) + (ch3 << 8) + (ch4 << 0));
}


/**
 * Read short from buffer data, where ArrayRange in buffer
 */
static int16_t ReadInt16(char* buffer, ArrayRange* range)
{
	CheckIndex("ReadInt16");
	
	int   pos     = range->start;
	char* data    = buffer + pos;

    int   ch1     = data[0];
    int   ch2     = data[1];

	range->start += sizeof(int16_t);

	return ((ch1 << 8) + (ch2 << 0));
}


/**
 * Read byte from buffer data, where ArrayRange in buffer
 */
static int8_t ReadInt8(char* buffer, ArrayRange* range)
{
	CheckIndex("ReadInt8");
	
	int   pos     = range->start;
	char* data    = buffer + pos;

	range->start += sizeof(int8_t);

	return data[0];
}


static void ReadLine(char* buffer, ArrayRange* range, ArrayRange* outLine)
{
	CheckIndex("ReadLine");

	int start      = range->start;
	int end        = range->end;

	outLine->start = outLine->end = start;

	for (; start <= end; start++)
	{
		if (buffer[start] == '\n')
		{
			outLine->end = start;
			break;
		}
		else if (buffer[start] == '\r')
		{
			if (start < end)
			{
				char c = buffer[start + 1];

				if (c == '\n')
				{
					start++;
				}
				else
				{
					outLine->end = start;
				}

				break;
			}
		}
	}

	if (start < end)
	{
		start++; // next new start
	}

	range->start = start;

	ALogD("ReadLine: string = %.*s", outLine->end - outLine->start + 1, buffer + outLine->start);
}


static bool TryFindString(char* buffer, ArrayRange* range, char* str)
{
	CheckIndex("TryFindString");

	int  start   = range->start;
	int  end     = range->end;
	int  pos     = 0;
	bool isFound = false;

	for (; start <= end; start++)
	{
		if (buffer[start] == str[pos])
		{
			while (str[pos] && buffer[start] == str[pos])
			{
				start++;
				pos++;
			}

			if (str[pos] == '\0')
			{
				isFound = true;
				ALogD("TryFindString %s", str);
			}

			break;
		}
	}

	// next new start
	range->start = start;

	return isFound;
}


struct ABufferReader ABufferReader[1] =
{
	ReadInt64,
	ReadInt32,
	ReadInt16,
	ReadInt8,
    ReadLine,
	TryFindString,
};


#undef CheckIndex

