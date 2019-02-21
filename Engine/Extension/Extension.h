/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2014-5-4
 * Update: 2019-2-1
 * Author: scott.cgi
 */


#ifndef EXTENSION_H
#define EXTENSION_H


#include "Engine/Toolkit/Toolkit.h"


struct AExtension
{
    void (*Init)();
};


extern struct AExtension AExtension[1];


#endif
