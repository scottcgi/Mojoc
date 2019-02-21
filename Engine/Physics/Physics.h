/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2014-6-11
 * Update: 2019-1-17
 * Author: scott.cgi
 */


#ifndef PHYSICS_H
#define PHYSICS_H


#include "Engine/Toolkit/Toolkit.h"


/**
 * Manage Physics module.
 */
struct APhysics
{
    void (*Init)();
};


extern struct APhysics APhysics[1];


#endif
