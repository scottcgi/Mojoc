/*
 *
 *
 *  Created on: 2013-9-8
 *      Author: scott.cgi
 */


#include "Mojoc/Graphics/Graphics.h"
#include "Mojoc/Graphics/OpenGL/Shader/ShaderMesh.h"
#include "Mojoc/Graphics/OpenGL/Shader/ShaderSprite.h"
#include "Mojoc/Graphics/OpenGL/Shader/ShaderPrimitive.h"
#include "Mojoc/Graphics/OpenGL/GLInfo.h"
#include "Mojoc/Toolkit/Platform/Log.h"


static void Init()
{
	AGLInfo         ->Init();
	AShaderMesh     ->Init();
	AShaderSprite   ->Init();
	AShaderPrimitive->Init();

	AGraphics->isUseVBO       = false;
    AGraphics->isUseVAO       = false;
    AGraphics->isUseMapBuffer = false;
}

static void SetUseVAO(bool isUseVAO)
{
    if (isUseVAO && AGLInfo->isSupportVAO)
    {
        AGraphics->isUseVAO = true;
        AGraphics->isUseVBO = true;
    }
    else
    {
        AGraphics->isUseVAO = false;
        AGraphics->isUseVBO = false;
    }
}

static void SetUseMapBuffer(bool isUseMapBuffer)
{
    if (isUseMapBuffer && AGLInfo->version > 2.0f)
    {
        AGraphics->isUseMapBuffer = true;
    }
    else
    {
        AGraphics->isUseMapBuffer = false;
    }
}

struct AGraphics AGraphics[1] =
{
	{
		.Init            = Init,
        .SetUseVAO       = SetUseVAO,
        .SetUseMapBuffer = SetUseMapBuffer,
	}
};
