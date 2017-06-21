/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-11-17
 * Author : scott.cgi
 * Version: 0.1
 */


#include <stdio.h>
#include "Engine/Graphics/Draw/Color.h"


static void SetWithString(char* colorStr, Color* outColor)
{
    int rgba[4];

    sscanf(colorStr,     "%2x", rgba);
    sscanf(colorStr + 2, "%2x", rgba + 1);
    sscanf(colorStr + 4, "%2x", rgba + 2);
    sscanf(colorStr + 6, "%2x", rgba + 3);

    outColor->r = rgba[0] / 255.0f;
    outColor->g = rgba[1] / 255.0f;
    outColor->b = rgba[2] / 255.0f;
    outColor->a = rgba[3] / 255.0f;
}


struct AColor AColor[1] =
{
    SetWithString,
};
