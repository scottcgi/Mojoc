/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2013-2-1
 * Update   : 2019-1-8
 * Author   : scott.cgi
 */


#ifndef ARRAY_RANGE_H
#define ARRAY_RANGE_H


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
