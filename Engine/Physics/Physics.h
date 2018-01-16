/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2014-6-11
 * Author: scott.cgi
 */

#ifndef PHYSICS_H
#define PHYSICS_H


#include "Engine/Toolkit/Toolkit.h"


struct APhysics
{
    void (*Init)();
};


extern struct APhysics APhysics[1];


#endif
