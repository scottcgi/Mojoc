/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The Mojoc game engine is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2013-3-11
 * Update: 2019-1-17
 * Author: scott.cgi
 */


#include <stdlib.h>
#include "Engine/Toolkit/Utils/Array.h"


static Array* Create(int elementTypeSize, int length)
{
    Array* array  = malloc(sizeof(Array) + elementTypeSize * length);
    array->data   = (char*) array + sizeof(Array);
    array->length = length;

    return array;
}


struct AArray AArray[1] =
{
    Create,
};

