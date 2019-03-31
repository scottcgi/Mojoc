/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The project Mojoc is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2013-2-1
 * Update: 2019-1-8
 * Author: scott.cgi
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
