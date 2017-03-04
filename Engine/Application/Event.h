/*
 *
 *
 *  Created on: 2013-3-13
 *      Author: scott.cgi
 */

#ifndef event_h
#define event_h

typedef enum
{
    event_touch_up     = 1,
    event_touch_down   = 2,
    event_touch_move   = 3,
	event_touch_cancel = 4,
}
EventTouchType;

typedef struct
{
	/** OpenGL coordinate */
	float          x;
	float          y;

	/**
	 * Identify touch point, 0-9 corresponding 10 touch points
	 */
	int            id;
	EventTouchType type;
}
EventTouchPoint;

#endif
