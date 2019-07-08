/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2013-2-1
 * Update : 2019-1-8
 * Author : scott.cgi
 */

 
#ifndef RECT_H
#define RECT_H


#include <stdbool.h>


/**
 * A rectangle made up of two diagonal vertices,
 * and which are top-left and bottom-right.
 */
typedef struct
{
    float left;
    float top;
    float right;
    float bottom;
}
Rect;


/**
 * Test whether one point inside rect.
 */
static inline bool ARect_TestPoint(Rect* rect, float x, float y)
{
    return (x >= rect->left && x <= rect->right && y >= rect->bottom && y <= rect->top);
}


/**
 * Test whether one rect intersects another rect.
 */
static inline bool ARect_TestRect(Rect* rectA, Rect* rectB)
{
    return !(
                rectA->right  < rectB->left   ||
                rectA->left   > rectB->right  ||
                rectA->top    < rectB->bottom ||
                rectA->bottom > rectB->top
            );
}


#endif
