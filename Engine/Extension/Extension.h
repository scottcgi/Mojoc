/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2014-5-4
 * Author : scott.cgi
 * Version: 0.1
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
