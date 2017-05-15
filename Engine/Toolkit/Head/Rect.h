/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-2-1
 * Author : scott.cgi
 * Version: 0.1
 */


#ifndef RECT_H
#define RECT_H


#include <stdbool.h>


typedef struct
{
	float left;
	float top;
	float right;
	float bottom;
}
Rect;


static inline bool ARect_TestPoint(Rect* rect, float x, float y)
{
	return (x >= rect->left && x <= rect->right && y >= rect->bottom && y <= rect->top);
}


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
