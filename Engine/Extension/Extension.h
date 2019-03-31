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
 * The project Mojoc is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2014-5-4
 * Update: 2019-2-1
 * Author: scott.cgi
 */


#ifndef EXTENSION_H
#define EXTENSION_H


#include "Engine/Toolkit/Toolkit.h"


struct AExtension
{
    void (*Init)(void);
};


extern struct AExtension AExtension[1];


#endif
