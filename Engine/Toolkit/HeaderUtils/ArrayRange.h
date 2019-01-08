/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License:
 * https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * Since : 2013-2-1
 * Update: 2019-1-8
 * Author: scott.cgi
 */


#ifndef ARRAY_RANGE_H
#define  ARRAY_RANGE_H


typedef struct
{
    /**
     * Start index in array.
     */
    int start;

    /**
     * End index in array, length is (end - start + 1).
     */
    int end;
}
ArrayRange;


#endif
