/*
 *
 *
 *  Created on: 2014-6-11
 *      Author: scott.cgi
 */

#ifndef physics_h
#define physics_h

#include "Mojoc/Toolkit/Toolkit.h"


struct APhysics
{
	void (*Init)();
};

extern struct APhysics APhysics[1];


#endif
