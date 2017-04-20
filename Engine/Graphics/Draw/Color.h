/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2013-2-1
 * Author : scott.cgi
 */

#ifndef color_h
#define color_h


typedef struct
{
	float r;
	float g;
	float b;
	float a;
}
Color;


struct AColor
{
	/**
	 * String format 000000 - ffffffff
	 */
	void (*SetWithString)(char* colorStr, Color* outColor);
};


extern struct AColor AColor[1];


#define color_white {1.0f, 1.0f, 1.0f, 1.0f}


#endif
