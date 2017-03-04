/*
 *
 *
 *  Created on: 2013-3-11
 *      Author: scott.cgi
 */

#include <malloc.h>
#include "Engine/Toolkit/Utils/Array.h"


static Array* Create(int typeSize, int length)
{
	Array* array  = (Array*) malloc(sizeof(Array) + typeSize * length);
	array->data   = (char*)  array + sizeof(Array);
	array->length = length;

	return array;
}

struct AArray AArray[1] =
{
	Create,
};

