/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2013-4-6
 * Update   : 2019-1-9
 * Author   : scott.cgi
 */


#include <string.h>
#include "Engine/Toolkit/Utils/BufferReader.h"
#include "Engine/Toolkit/Platform/Log.h"


#define CheckRange(tag)                                           \
    ALog_A                                                        \
    (                                                             \
        range->start <= range->end,                               \
        "ABufferReader " tag " range error start[%d] > end[%d].", \
        range->start,                                             \
        range->end                                                \
    )

#define ReadLineLog() \
    ALog_D("ABufferReader ReadLine = %.*s", outLine->end - outLine->start + 1, buffer + outLine->start)


static void ReadLine(const char* buffer, ArrayRange* range, ArrayRange* outLine)
{
    CheckRange("ReadLine");

    int start      = range->start;
    int end        = range->end;
    outLine->start = start;
    outLine->end   = start;

    for (; start <= end; ++start)
    {
        if (buffer[start] == '\n')
        {
            outLine->end = start;

            if (start != end)
            {
                // move to new start
                range->start = start + 1;
            }
            else
            {
                // scan over
                range->start = end;
            }

            ReadLineLog();
            return;
        }
        else if (buffer[start] == '\r')
        {
            if (start != end)
            {
                // check next char
                ++start;
                
                if (buffer[start] == '\n')
                {
                    outLine->end = start;

                    if (start != end)
                    {
                        // move to new start
                        range->start = start + 1;
                    }
                    else
                    {
                        // scan over
                        range->start = end;
                    }
                }
                else
                {
                    // back to '\r'
                    outLine->end = start - 1;
                    // start is next
                    range->start = start;
                }
            }
            else
            {
                outLine->end = start;
                // scan over
                range->start = end;
            }

            ReadLineLog();
            return;
        }
    }

    // not found newline char
    outLine->end = end;
    range->end   = end;
    ReadLineLog();
}


/**
 * Redundancy is better so don't delete.
 */
static bool TryFindStringByLoop(const char* buffer, ArrayRange* range, const char* str)
{
    CheckRange("TryFindString");

    int  start = range->start;
    int  end   = range->end;
    int  pos   = 0;
    char first  = str[0];
    int  cmp;

    for (; start <= end; ++start)
    {
        // always test from first char
        if (buffer[start] == first)
        {
            // compare from start
            cmp = start;

            while (true)
            {
                ++pos;

                if (str[pos] == '\0')
                {
                    if (cmp != end)
                    {
                        // move to next char
                        range->start = cmp + 1;
                    }
                    else
                    {
                        // scan over
                        range->start = end;
                    }

                    ALog_D
                    (
                        "ABufferReader TryFindString found str = %.*s, after str = '%c'",
                        cmp - start + 1,
                        buffer + start,
                        buffer[range->start]
                    );

                    return true;
                }

                ++cmp;

                if (cmp > end)
                {
                    // scan over
                    goto NotFoundStr;
                }

                if (buffer[cmp] != str[pos])
                {
                    // compare failure reset to first
                    pos = 0;
                    break;
                }
            }
        }
    }

    NotFoundStr:
    ALog_D("ABufferReader TryFindString not found str = %s", str);
    // keep range->start
    return false;
}


static bool TryFindStringByMemcmp(const char* buffer, ArrayRange* range, const char* str)
{
    CheckRange("TryFindString");

    char* bufferStart = (char*) buffer + range->start;
    int   len1        = range->end     - range->start + 1; // count [start, end]
    int   len2        = (int)   strlen(str);

    while (len1 >= len2)
    {
        if (memcmp(bufferStart, str, (size_t) len2) == 0)
        {
            // move to next char
            // newStart + 1 = ((range->end - len1 + 1) + len2 - 1) + 1
            range->start = range->end - len1 + len2 + 1;

            if (range->start > range->end)
            {
                range->start = range->end;
            }

            ALog_D
            (
                "ABufferReader TryFindString found str = %.*s, after str = '%c'",
                len2,
                bufferStart,
                buffer[range->start]
            );
            
            return true;
        }

        --len1;
        ++bufferStart;
    }

    ALog_D("ABufferReader TryFindString not found str = %s", str);
    // keep range->start
    return false;
}


struct ABufferReader ABufferReader[1] =
{{
    ReadLine,
    TryFindStringByMemcmp,
}};


#undef ReadLineLog

