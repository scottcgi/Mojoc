/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2013-9-8
 * Update   : 2019-1-19
 * Author   : scott.cgi
 */


#include "Engine/Graphics/Graphics.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderMesh.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderSprite.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderPrimitive.h"
#include "Engine/Graphics/OpenGL/GLInfo.h"


static void Init()
{
    AGLInfo         ->Init();
    AShaderMesh     ->Init();
    AShaderSprite   ->Init();
    AShaderPrimitive->Init();
}

struct AGraphics AGraphics[1] =
{{
    Init,
}};
