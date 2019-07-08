/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2012-12-22
 * Update : 2019-1-23
 * Author : scott.cgi
 */


#include <string.h>
#include <stdlib.h>
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Toolkit/Utils/FileTool.h"
#include "Engine/Graphics/Utils/Image.h"


static void SetSize(int width, int height)
{
    // set the OpenGL viewport to the same size as the surface.
    glViewport(0, 0, width, height);

    AGLTool->screenWidth          = (float) width;
    AGLTool->screenHeight         = (float) height;
    AGLTool->screenRatio          = (float) width  / (float) height;
    AGLTool->screenHalfWidth      = (float) width  / 2;
    AGLTool->screenHalfHeight     = (float) height / 2;

    AGLTool->ratioDivideHalfWidth = AGLTool->screenRatio     / AGLTool->screenHalfWidth;
    AGLTool->halfWidthDivideRatio = AGLTool->screenHalfWidth / AGLTool->screenRatio;
}


static GLuint LoadShader(GLenum shaderType, const char* shaderSourceStr)
{
    // create the shader object
    GLuint shader = glCreateShader(shaderType);

    if (shader == 0)
    {
        ALog_E("AGLTool LoadShader glCreateShader failed !");
        return shader;
    }

    // load the shader source
    glShaderSource(shader, 1, &shaderSourceStr, NULL);

    // compile the shader
    glCompileShader(shader);

    GLint compiled;

    // check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (compiled == GL_FALSE)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 0)
        {
            char buffer[infoLen];
            glGetShaderInfoLog(shader, infoLen, NULL, buffer);
            ALog_E("AGLTool LoadShader could not compile shader: %s, log: %s", shaderSourceStr, buffer);
            glDeleteShader(shader);
        }
        else
        {
            ALog_E("AGLTool LoadShader could not compile shader: %s", shaderSourceStr);
        }

        return 0;
    }

    return shader;
}


static GLuint LoadProgram(const char* vertexSourceStr, const char* fragmentSourceStr)
{
    GLuint program;

    // load the vertex shader
    GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vertexSourceStr);

    if (vertexShader == 0)
    {
        ALog_E("AGLTool LoadProgram failed, cannot load shader: %s", vertexSourceStr);
        return 0;
    }

    // load the fragment shader
    GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentSourceStr);

    if (fragmentShader == 0)
    {
        ALog_E("AGLTool LoadProgram failed, cannot load shader: %s", fragmentSourceStr);
        return 0;
    }

    // create the program object
    program = glCreateProgram();

    if (program == 0)
    {
        ALog_E("AGLTool LoadProgram glCreateProgram failed，from shader: %s, %s", vertexSourceStr, fragmentSourceStr);
        return 0;
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // link the program
    glLinkProgram(program);

    GLint linkStatus;
    // check the link status
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE)
    {
        GLint bufLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);

        if (bufLength > 0)
        {
            char buffer[bufLength];
            glGetProgramInfoLog(program, bufLength, NULL, buffer);
            ALog_E
            (
                "AGLTool LoadProgram cannot link program from shader: %s, %s, log: %s",
                vertexSourceStr,
                fragmentSourceStr,
                buffer
            );
        }
        else
        {
            ALog_E
            (
                "AGLTool LoadProgram cannot link program from shader: %s, %s",
                vertexSourceStr,
                fragmentSourceStr
            );
        }

        glDeleteProgram(program);

        return 0;
    }

    // free up no longer needed shader resources
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}


GLuint LoadProgramFromFile(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
    char* vertexShader   = AFileTool->CreateStringFromResource(vertexShaderFilePath);
    char* fragmentShader = AFileTool->CreateStringFromResource(fragmentShaderFilePath);
    GLuint program       = LoadProgram(vertexShader, fragmentShader);

    free((void*) vertexShader);
    free((void*) fragmentShader);

    return program;
}


static void LoadTexture(const char* textureFilePath, Texture* outTexture)
{
     GLuint textureId;

     // use tightly packed data
     // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

     // generate a outTexture object
     glGenTextures(1, &textureId);

     // bind to the outTexture
     glBindTexture(GL_TEXTURE_2D, textureId);

     outTexture->id = textureId;

     // set filtering
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

     float width;
     float height;

     void* pixels = AImage->CreatePixelDataFromPNG(textureFilePath, &width, &height);
     ALog_A(pixels != NULL, "AGLTool LoadTexture failed, no pixels data found");

     // load the data into the bound outTexture
     glTexImage2D
     (
         GL_TEXTURE_2D,
         0,
         GL_RGBA,
         (GLsizei) width,
         (GLsizei) height, 
         0,
         GL_RGBA,
         GL_UNSIGNED_BYTE,
         pixels
     );

     outTexture->width  = AGLTool_ToGLWidth (width);
     outTexture->height = AGLTool_ToGLHeight(height);

     free(pixels);
}


struct AGLTool AGLTool[1] =
{
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,

    SetSize,

    LoadShader,
    LoadProgram,

    LoadProgramFromFile,
    LoadTexture,
};

