/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-3-11
 * Author : scott.cgi
 * Version: 0.0.0
 */


#include <stdlib.h>
#include "Engine/Toolkit/Utils/Array.h"


static Array* Create(int elementTypeSize, int length)
{
    Array* array  = (Array*) malloc (sizeof(Array) + elementTypeSize * length);
    array->data   = (char*)  array + sizeof(Array);
    array->length = length;

    return array;
}


struct AArray AArray[1] =
{
    Create,
};

