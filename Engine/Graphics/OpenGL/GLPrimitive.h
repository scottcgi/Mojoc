/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2014-2-26
 * Update   : 2019-1-24
 * Author   : scott.cgi
 */


#ifndef GL_PRIMITIVE_H
#define GL_PRIMITIVE_H


#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/HeaderUtils/Rect.h"
#include "Engine/Graphics/Draw/Color.h"
#include "Engine/Toolkit/Utils/Array.h"


/**
 * Draw something without texture.
 * can render with Drawable's Render function.
 */
struct AGLPrimitive
{
    /**
     * Render array of points, the point is pair of x, y.
     */
    void (*RenderPoints) (Array(float)* pointArr,  Matrix4* mvpMatrix, Color* color, float pointSize);

    /**
     * Render array of vertices, the vertex is pair of x, y.
     */
    void (*RenderPolygon)(Array(float)* vertexArr, Matrix4* mvpMatrix, Color* color, float lineWidth);

    /**
     * Render array of lines, the line is two pairs of x, y.
     */
    void (*RenderLines)  (Array(float)* lineArr,   Matrix4* mvpMatrix, Color* color, float lineWidth);

    /**
     * Render rect.
     */
    void (*RenderRect)   (Rect*         rect,      Matrix4* mvpMatrix, Color* color, float lineWidth);
};


extern struct AGLPrimitive AGLPrimitive[1];


#endif
