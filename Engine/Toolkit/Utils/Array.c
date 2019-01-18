/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-3-11
 * Update: 2019-1-17
 * Author: scott.cgi
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

