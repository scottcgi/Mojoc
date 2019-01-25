/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-2-1
 * Update: 2019-1-25
 * Author: scott.cgi
 */


#ifndef COLOR_H
#define  COLOR_H


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
     * String format to 000000 - ffffffff
     */
    void (*SetWithString)(char* colorStr, Color* outColor);
};


extern struct AColor AColor[1];


#define COLOR_WHITE {1.0f, 1.0f, 1.0f, 1.0f}
#define COLOR_BLACK {0.0f, 0.0f, 0.0f, 0.0f}


#endif
