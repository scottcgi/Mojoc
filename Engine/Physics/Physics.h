/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2014-6-11
 * Author: scott.cgi
 */

#ifndef physics_h
#define physics_h


#include "Engine/Toolkit/Toolkit.h"


struct APhysics
{
	void (*Init)();
};


extern struct APhysics APhysics[1];


#endif
