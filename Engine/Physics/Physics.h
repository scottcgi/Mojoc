/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2014-6-11
 * Update   : 2019-1-17
 * Author   : scott.cgi
 */


#ifndef PHYSICS_H
#define PHYSICS_H


#include "Engine/Toolkit/Toolkit.h"


/**
 * Manage Physics module.
 */
struct APhysics
{
    void (*Init)(void);
};


extern struct APhysics APhysics[1];


#endif
