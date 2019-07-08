/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2013-3-11
 * Update : 2019-1-17
 * Author : scott.cgi
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

