/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-2-1
 * Author : scott.cgi
 * Version: 0.0.0
 */


#ifndef ARRAY_RANGE_H
#define ARRAY_RANGE_H


typedef struct
{
    /**
     * Start index in array
     */
    int start;

    /**
     * End index in array, length is end - start + 1
     */
    int end;
}
ArrayRange;


#endif
