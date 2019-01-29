/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2014-2-26
 * Update: 2019-1-24
 * Author: scott.cgi
 */


#include "Engine/Graphics/OpenGL/GLPrimitive.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderPrimitive.h"
#include "Engine/Graphics/OpenGL/Camera.h"


static Matrix4 identityMatrix[1] = MATRIX4_IDENTITY;
static Matrix4 mvpMatrix     [1];


static inline void SetMatrix()
{
    AMatrix->MultiplyMM
    (
        ACamera->vp,
        AGLPrimitive->modelMatrix ? AGLPrimitive->modelMatrix : identityMatrix,
        mvpMatrix
    );

    glLineWidth(AGLPrimitive->lineWidth);
    AShaderPrimitive->Use(mvpMatrix, AGLPrimitive->color, AGLPrimitive->pointSize);
}


static void DrawPoints(Array(float)* pointArr)
{
    SetMatrix();
    glVertexAttribPointer((GLuint) AShaderPrimitive->attribPosition, 2, GL_FLOAT, false, 0, pointArr->data);
    glDrawArrays(GL_POINTS, 0, pointArr->length >> 1); // NOLINT(hicpp-signed-bitwise)
}


static void DrawPolygon(Array(float)* vertexArr)
{
    SetMatrix();
    glVertexAttribPointer((GLuint) AShaderPrimitive->attribPosition, 2, GL_FLOAT, false, 0, vertexArr->data);
    glDrawArrays(GL_LINE_LOOP, 0, vertexArr->length >> 1); // NOLINT(hicpp-signed-bitwise)
}


static void DrawLines(Array(float)* vertexArr)
{
    SetMatrix();
    glVertexAttribPointer((GLuint) AShaderPrimitive->attribPosition, 2, GL_FLOAT, false, 0, vertexArr->data);
    glDrawArrays(GL_LINES, 0, vertexArr->length >> 1); // NOLINT(hicpp-signed-bitwise)
}


static void DrawRect(Rect* rect)
{
    SetMatrix();

    glVertexAttribPointer
    (
        (GLuint) AShaderPrimitive->attribPosition,
        2,
        GL_FLOAT,
        false,
        0,
        (float[sizeof(Rect)])
        {
            rect->top,    rect->left,
            rect->bottom, rect->left,
            rect->bottom, rect->right,
            rect->top,    rect->right,
        }
    );

    glDrawArrays(GL_LINE_LOOP, 0, 4);
}


struct AGLPrimitive AGLPrimitive[1] =
{
    1.0f,
    1.0f,

    COLOR_WHITE,
    NULL,

    DrawPoints,
    DrawPolygon,
    DrawLines,
    DrawRect,
};
