/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2013-9-8
 * Update   : 2019-1-19
 * Author   : scott.cgi
 */


#ifndef GRAPHICS_H
#define GRAPHICS_H


#include <stdbool.h>


/**
 * Manage and control Graphics module.
 */
struct AGraphics
{
    /**
     * Init graphics each parts.
     */
    void (*Init)           (void);
};


extern struct AGraphics AGraphics[1];


#endif
