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
 * Since : 2014-2-26
 * Update: 2019-1-24
 * Author: scott.cgi
 *
 */


#include <stdbool.h>
#include "Engine/Toolkit/HeaderUtils/String.h"
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderPrimitive.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Graphics/OpenGL/Shader/Shader.h"


static void Use(Matrix4* mvpMatrix, Color* color, GLfloat pointSize)
{
    if (AShader->program != AShaderPrimitive->program)
    {
        AShader->program = AShaderPrimitive->program;

        glUseProgram             (AShaderPrimitive->program);
        glEnableVertexAttribArray((GLuint) AShaderPrimitive->attribPosition);
    }

    // passed matrix into shader program
    glUniformMatrix4fv(AShaderPrimitive->uniformMVPMatrix, 1,        false,    mvpMatrix->m);
    glUniform4f       (AShaderPrimitive->uniformColor,     color->r, color->g, color->b, color->a);
    glUniform1f       (AShaderPrimitive->uniformPointSize, pointSize);
}


static void Init()
{
    AShaderPrimitive->program = AGLTool->LoadProgram
    (
        AString_Make
        (
            precision highp float;
            uniform   mat4  uMVPMatrix;
            attribute vec2  aPosition;
            uniform   float uPointSize;

            void main()
            {
                gl_Position  = uMVPMatrix * vec4(aPosition, 0.0, 1.0);
                gl_PointSize = uPointSize;
            }
        ),

        AString_Make
        (
            precision lowp float;
            uniform   vec4  uColor;

            void main()
            {
              gl_FragColor = uColor;
            }
        )
    );

    // get the attribute locations
    AShaderPrimitive->attribPosition   = glGetAttribLocation (AShaderPrimitive->program, "aPosition");
    AShaderPrimitive->uniformMVPMatrix = glGetUniformLocation(AShaderPrimitive->program, "uMVPMatrix");
    AShaderPrimitive->uniformColor     = glGetUniformLocation(AShaderPrimitive->program, "uColor");
    AShaderPrimitive->uniformPointSize = glGetUniformLocation(AShaderPrimitive->program, "uPointSize");

    ALog_A
    (
        AShaderPrimitive->uniformMVPMatrix != -1,
        "AShaderPrimitive could not glGetUniformLocation for uniformMVPMatrix"
    );

    ALog_A
    (
        AShaderPrimitive->uniformColor != -1,
        "AShaderPrimitive could not glGetUniformLocation for uniformColor"
    );

    ALog_A
    (
        AShaderPrimitive->uniformPointSize != -1,
        "AShaderPrimitive could not glGetUniformLocation for uniformPointSize"
    );
}


struct AShaderPrimitive AShaderPrimitive[1] =
{{
    .Use  = Use,
    .Init = Init,
}};

