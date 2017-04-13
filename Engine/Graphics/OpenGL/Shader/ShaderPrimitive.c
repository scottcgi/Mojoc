/*
 *
 *
 *  Created on: 2014-2-26
 *  Author: scott.cgi
 */

#include <stdbool.h>

#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderPrimitive.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Graphics/OpenGL/Shader/Shader.h"
#include "Engine/Toolkit/Head/MacroDefine.h"


static void Use(Matrix4* mvpMatrix, Color* color, GLint pointSize)
{
	if (AShader->program != AShaderPrimitive->program)
	{
	    AShader->program = AShaderPrimitive->program;

	    glUseProgram(AShaderPrimitive->program);
	    glEnableVertexAttribArray(AShaderPrimitive->attribPosition);
	}

    // Passed matrix into shader program
    glUniformMatrix4fv(AShaderPrimitive->uniformMVPMatrix, 1, false, mvpMatrix->m);
    glUniform4f(AShaderPrimitive->uniformColor, color->r, color->g, color->b, color->a);
    glUniform1f(AShaderPrimitive->uniformPointSize, pointSize);
}


static void Init()
{
	AShaderPrimitive->program = AGLTool->LoadProgram
	(
		Stringizing
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

		Stringizing
		(
            precision lowp float;
			uniform   vec4  uColor;

			void main()
			{
			  gl_FragColor = uColor;
			}
		)
	);

    // Get the attribute locations
	AShaderPrimitive->attribPosition   = glGetAttribLocation(AShaderPrimitive->program,  "aPosition");
	AShaderPrimitive->uniformMVPMatrix = glGetUniformLocation(AShaderPrimitive->program, "uMVPMatrix");
	AShaderPrimitive->uniformColor     = glGetUniformLocation(AShaderPrimitive->program, "uColor");
	AShaderPrimitive->uniformPointSize = glGetUniformLocation(AShaderPrimitive->program, "uPointSize");

	ALogA
	(
		AShaderPrimitive->uniformMVPMatrix != -1,
	    "AShaderPrimitive could not glGetUniformLocation for uMVPMatrix"
	);

	ALogA
	(
		AShaderPrimitive->uniformColor != -1,
	    "AShaderPrimitive could not glGetUniformLocation for uColor"
	);

	ALogA
	(
		AShaderPrimitive->uniformPointSize != -1,
		"AShaderPrimitive could not glGetUniformLocation for uPointSize"
	);
}


struct AShaderPrimitive AShaderPrimitive[1] =
{
	{
		.Use  = Use,
		.Init = Init,
	}
};

