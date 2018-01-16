/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2014-2-26
 * Author: scott.cgi
 */

#ifndef GL_PRIMITIVE_H
#define GL_PRIMITIVE_H


#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Head/Rect.h"
#include "Engine/Graphics/Draw/Color.h"
#include "Engine/Toolkit/Utils/Array.h"


struct AGLPrimitive
{
    /**
     * Default 1.0f
     */
    GLfloat  pointSize;

    /**
     * Default 1.0f
     */
    GLfloat  lineWidth;

    /**
     * Default {1.0f, 1.0f, 1.0f, 1.0f}
     */
    Color    color[1];

    /**
     * Default NULL
     */
    Matrix4* modelMatrix;

    /**
     * Each point pair of x, y if model NULL will use identity matrix
     */
    void (*DrawPoints) (Array(float)* pointArr);

    /**
     * Each vertex pair of x, y if model NULL will use identity matrix
     */
    void (*DrawPolygon)(Array(float)* vertexArr);

    /**
     * Each vertex pair of x, y if model NULL will use identity matrix
     */
    void (*DrawLines)  (Array(float)* vertexArr);

    /**
     * If model NULL will use identity matrix
     */
    void (*DrawRect)   (Rect* rect);
};


extern struct AGLPrimitive AGLPrimitive[1];


#endif
