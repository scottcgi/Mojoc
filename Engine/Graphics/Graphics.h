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
 * Since : 2013-9-8
 * Update: 2019-1-19
 * Author: scott.cgi
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
     * Default false.
     */
    bool isUseVBO;

    /**
     * Default false.
     * setting by method SetUseVAO.
     */
    bool isUseVAO;

    /**
     * Default false.
     * setting by method SetUseMapBuffer.
     */
    bool isUseMapBuffer;

    /**
     * Init graphics each parts.
     */
    void (*Init)           (void);

    /**
     * If VAO not support nothing changed.
     * if use VAO also use VBO.
     */
    void (*SetUseVAO)      (bool isUseVAO);

    /**
     * If glMapBufferRange not support will use glBufferSubData.
     * if not use VBO no effect.
     */
    void (*SetUseMapBuffer)(bool isUseMapBuffer);
};


extern struct AGraphics AGraphics[1];


#endif
