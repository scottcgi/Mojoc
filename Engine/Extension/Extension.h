/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2014-5-4
 * Author: scott.cgi
 */

#ifndef extension_h
#define extension_h

#include "Engine/Toolkit/Toolkit.h"


struct AExtension
{
	void (*Init)();
};


extern struct AExtension AExtension[1];


#endif
