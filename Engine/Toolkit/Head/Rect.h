/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2013-2-1
 * Author: scott.cgi
 */

#ifndef rect_h
#define rect_h


#include <stdbool.h>


typedef struct
{
	float left;
	float top;
	float right;
	float bottom;
}
Rect;


static inline bool ARectTestPoint(Rect* rect, float x, float y)
{
	return (x >= rect->left && x <= rect->right && y >= rect->bottom && y <= rect->top);
}


static inline bool ARectTestRect(Rect* rectA, Rect* rectB)
{
	return !(
				rectA->right  < rectB->left   ||
				rectA->left   > rectB->right  ||
				rectA->top    < rectB->bottom ||
				rectA->bottom > rectB->top
			);
}


#endif
