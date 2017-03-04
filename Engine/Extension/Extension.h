/*
 *  Created on: 2014-5-4
 *      Author: scott.cgi
 */

#ifndef extension_h
#define extension_h

#include "Mojoc/Toolkit/Toolkit.h"

struct AExtension
{
	void (*Init)();
};

extern struct AExtension AExtension[1];

#endif
