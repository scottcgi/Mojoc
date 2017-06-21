/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-2-1
 * Author : scott.cgi
 * Version: 0.1
 */


#ifndef COLOR_H
#define COLOR_H


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


#define COLOR_WHITE {1.0f, 1.0f, 1.0f, 1.0f}


#endif
