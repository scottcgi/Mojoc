/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2016-8-13
 * Author : scott.cgi
 * Version: 0.1
 */


#include <stdbool.h>
#include "Engine/Toolkit/Head/String.h"
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderMesh.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Graphics/OpenGL/Shader/Shader.h"


static void Use(Matrix4* mvpMatrix)
{
    if (AShader->program != AShaderMesh->program)
    {
        AShader->program = AShaderMesh->program;
        glUseProgram(AShaderMesh->program);
    }

    glUniformMatrix4fv(AShaderMesh->uniformMVPMatrix, 1, false, mvpMatrix->m);

    // set the sampler to texture unit 0
    glUniform1i       (AShaderMesh->uniformSample2D, 0);
}


static void Init()
{
    AShaderMesh->program = AGLTool->LoadProgram
    (
        AString_Make
        (
            precision highp float;
            uniform   mat4  uMVPMatrix;
            attribute vec4  aPosition;
            attribute vec2  aTexcoord;
            attribute float aOpacity;
            attribute vec3  aRGB;

            varying   vec2  vTexcoord;
            varying   vec4  vColor;

            void main()
            {
                gl_Position = uMVPMatrix * aPosition;
                vTexcoord   = aTexcoord;
                vColor      = vec4(aRGB, aOpacity);
            }
        ),

        AString_Make
        (
            precision lowp      float;
            uniform   sampler2D uSampler2D;
            varying   vec2      vTexcoord;
            varying   vec4      vColor;

            void main()
            {
                gl_FragColor = texture2D(uSampler2D, vTexcoord) * vColor;
            }
        )
    );

    // Get the attribute locations
    AShaderMesh->attribPosition   = glGetAttribLocation(AShaderMesh->program, "aPosition");
    AShaderMesh->attribTexcoord   = glGetAttribLocation(AShaderMesh->program, "aTexcoord");
    AShaderMesh->attribOpacity    = glGetAttribLocation(AShaderMesh->program, "aOpacity");
    AShaderMesh->attribRGB        = glGetAttribLocation(AShaderMesh->program, "aRGB");

    AShaderMesh->uniformSample2D  = glGetUniformLocation(AShaderMesh->program, "uSampler2D");
    AShaderMesh->uniformMVPMatrix = glGetUniformLocation(AShaderMesh->program, "uMVPMatrix");

    ALog_A
    (
        AShaderMesh->uniformSample2D != -1,
        "AShaderMesh could not glGetUniformLocation for uniformSample2D"
    );

    ALog_A
    (
        AShaderMesh->uniformMVPMatrix != -1,
        "AShaderMesh could not glGetUniformLocation for uMVPMatrix"
    );

    glEnableVertexAttribArray(AShaderMesh->attribPosition);
    glEnableVertexAttribArray(AShaderMesh->attribTexcoord);
    glEnableVertexAttribArray(AShaderMesh->attribOpacity);
    glEnableVertexAttribArray(AShaderMesh->attribRGB);
}


struct AShaderMesh AShaderMesh[1] =
{
    {
        .Use  = Use,
        .Init = Init,
    }
};
