/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2013-1-8
 * Update   : 2019-1-24
 * Author   : scott.cgi
 */


#include <stdbool.h>
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderSprite.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Graphics/OpenGL/Shader/Shader.h"
#include "Engine/Toolkit/HeaderUtils/String.h"


static void Use(Matrix4* mvpMatrix, Color* color)
{
    if (AShader->program != AShaderSprite->program)
    {
        AShader->program = AShaderSprite->program;
        glUseProgram(AShaderSprite->program);
    }

    glUniformMatrix4fv(AShaderSprite->uniformMVPMatrix, 1,        false,    mvpMatrix->m);
    glUniform4f       (AShaderSprite->uniformColor,     color->r, color->g, color->b, color->a);
    // set the sampler to texture unit 0
    glUniform1i       (AShaderSprite->uniformSampler2D, 0);
}


static void Init()
{
    AShaderSprite->program = AGLTool->LoadProgram
    (
        AString_Make
        (
            precision highp float;
            uniform   mat4  uMVPMatrix;
            attribute vec4  aPositionTexcoord;
            varying   vec2  vTexcoord;

            void main()
            {
                gl_Position = uMVPMatrix * vec4(aPositionTexcoord.xy, 0.0, 1.0);
                vTexcoord   = aPositionTexcoord.zw;
            }
        ),

        AString_Make
        (
            precision lowp float;
            uniform   sampler2D uSampler2D;
            uniform   vec4      uColor;
            varying   vec2      vTexcoord;

            void main()
            {
                gl_FragColor = texture2D(uSampler2D, vTexcoord) * uColor;
            }
        )
    );

    // get the attribute locations
    AShaderSprite->attribPositionTexcoord = glGetAttribLocation(AShaderSprite->program,  "aPositionTexcoord");
    AShaderSprite->uniformSampler2D       = glGetUniformLocation(AShaderSprite->program, "uSampler2D");
    AShaderSprite->uniformMVPMatrix       = glGetUniformLocation(AShaderSprite->program, "uMVPMatrix");
    AShaderSprite->uniformColor           = glGetUniformLocation(AShaderSprite->program, "uColor");

    ALog_A
    (
        AShaderSprite->uniformSampler2D != -1,
        "AShaderSprite cannot glGetUniformLocation for uniformSampler2D"
    );

    ALog_A
    (
        AShaderSprite->uniformMVPMatrix != -1,
        "AShaderSprite cannot glGetUniformLocation for uniformMVPMatrix"
    );

    ALog_A
    (
        AShaderSprite->uniformColor != -1,
        "AShaderSprite cannot glGetUniformLocation for uniformColor"
    );

    glEnableVertexAttribArray((GLuint) AShaderSprite->attribPositionTexcoord);
}


struct AShaderSprite AShaderSprite[1] =
{{
    .Use  = Use,
    .Init = Init,
}};
