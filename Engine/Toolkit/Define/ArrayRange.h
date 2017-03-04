/*
 *
 *
 *  Created on: 2013-2-1
 *  Author: scott.cgi
 */

#ifndef array_range_h
#define array_range_h

typedef struct
{
	/** Start index in array */
	int start;
	/** End index in array, length is end - start + 1 */
	int end;
}
ArrayRange;

#endif
