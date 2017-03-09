/*
 *
 *
 *  Created on: 2013-1-8
 *  Author: scott.cgi
 */

#include <stdbool.h>

#include "Engine/Graphics/OpenGL/Head/gl3.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderSprite.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Graphics/OpenGL/Shader/Shader.h"
#include "Engine/Toolkit/Head/Def.h"


static void Use(Matrix4* mvpMatrix, Color* color)
{
	if (AShader->program != AShaderSprite->program)
	{
		AShader->program = AShaderSprite->program;
		glUseProgram(AShaderSprite->program);
	}

    glUniformMatrix4fv(AShaderSprite->uniformMVPMatrix, 1,        false,    mvpMatrix->m);
    glUniform4f       (AShaderSprite->uniformColor,     color->r, color->g, color->b, color->a);
}

static void Init()
{
	AShaderSprite->program = AGLTool->LoadProgram
	(
		Stringizing
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

		Stringizing
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

	// Get the attribute locations
	AShaderSprite->attribPositionTexcoord = glGetAttribLocation(AShaderSprite->program,  "aPositionTexcoord");
	AShaderSprite->uniformSampler2D       = glGetUniformLocation(AShaderSprite->program, "uSampler2D");
	AShaderSprite->uniformMVPMatrix       = glGetUniformLocation(AShaderSprite->program, "uMVPMatrix");
	AShaderSprite->uniformColor           = glGetUniformLocation(AShaderSprite->program, "uColor");

	ALogA
	(
		AShaderSprite->uniformSampler2D != -1,
		"AShaderSprite could not glGetUniformLocation for uniformSampler2D"
	);

	ALogA
	(
		AShaderSprite->uniformMVPMatrix != -1,
		"AShaderSprite could not glGetUniformLocation for uMVPMatrix"
	);

	ALogA
	(
		AShaderSprite->uniformColor != -1,
		"AShaderSprite could not glGetUniformLocation for uColor"
	);

	glEnableVertexAttribArray(AShaderSprite->attribPositionTexcoord);
	// set the sampler to texture unit 0
	glUniform1i(AShaderSprite->uniformSampler2D, 0);
}

struct AShaderSprite AShaderSprite[1] =
{
	{
		.Use  = Use,
		.Init = Init,
	}
};
