/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-2-1
 * Update: 2019-1-8
 * Author: scott.cgi
 */


#ifndef ARRAY_RANGE_H
#define  ARRAY_RANGE_H


/**
 * The range [start, end] marked in array.
 */
typedef struct
{
    /**
     * Start index in array.
     */
    int start;

    /**
     * End index in array, so the range length is (end - start + 1).
     */
    int end;
}
ArrayRange;


#endif
