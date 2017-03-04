/*
 *
 *
 *  Created on: 2014-9-8
 *      Author: scott.cgi
 */

#ifndef tool_kit_h
#define tool_kit_h


struct AToolkit
{
	void (*Init)();
};

extern struct AToolkit AToolkit[1];

#endif 
