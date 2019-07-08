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


#include "Engine/Graphics/OpenGL/GLPrimitive.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderPrimitive.h"
#include "Engine/Graphics/OpenGL/Camera.h"


static inline void Render
(
    GLenum   mode,
    void*    data,
    Matrix4* mvpMatrix,
    Color*   color,
    float    pointOrLineSize,
    int      count,
    bool     isLine
)
{
    if (isLine)
    {
        AShaderPrimitive->Use(mvpMatrix, color, 1.0f);
        glLineWidth(pointOrLineSize);
    }
    else
    {
        AShaderPrimitive->Use(mvpMatrix, color, pointOrLineSize);
    }

    glVertexAttribPointer((GLuint) AShaderPrimitive->attribPosition, 2, GL_FLOAT, false, 0, data);
    glDrawArrays(mode, 0, count);
}


static void RenderPoints(Array(float)* pointArr, Matrix4* mvpMatrix, Color* color, float pointSize)
{
    Render(GL_POINTS, pointArr->data, mvpMatrix, color, pointSize, pointArr->length >> 1, false); // NOLINT(hicpp-signed-bitwise)
}


static void RenderPolygon(Array(float)* vertexArr, Matrix4* mvpMatrix, Color* color, float lineWidth)
{
    Render(GL_LINE_LOOP, vertexArr->data, mvpMatrix, color, lineWidth, vertexArr->length >> 1, true); // NOLINT(hicpp-signed-bitwise)
}


static void RenderLines(Array(float)* lineArr, Matrix4* mvpMatrix, Color* color, float lineWidth)
{
    Render(GL_LINE_STRIP, lineArr->data, mvpMatrix, color, lineWidth, lineArr->length >> 1, true); // NOLINT(hicpp-signed-bitwise)
}


static void RenderRect(Rect* rect, Matrix4* mvpMatrix, Color* color, float lineWidth)
{
    Render
    (
        GL_LINE_LOOP,
        (float[8])
        {
            rect->top,    rect->left,
            rect->bottom, rect->left,
            rect->bottom, rect->right,
            rect->top,    rect->right,
        },
        mvpMatrix,
        color,
        lineWidth,
        4,
        true
    );
}


struct AGLPrimitive AGLPrimitive[1] =
{
    RenderPoints,
    RenderPolygon,
    RenderLines,
    RenderRect,
};
