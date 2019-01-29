/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2012-12-25
 * Update: 2019-1-23
 * Author: scott.cgi
 */


#include <string.h>
#include <stdlib.h>
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/Draw/Quad.h"
#include "Engine/Graphics/OpenGL/GLTool.h"


static void Init(float width, float height, Quad* outQuad)
{
    outQuad->width          =  width;
    outQuad->height         =  height;
    
    outQuad->offsetCenterX  = -width  / 2;
    outQuad->offsetCenterY  =  height / 2;
    
    outQuad->offsetTextureX =  0.0f;
    outQuad->offsetTextureY =  0.0f;
}


static Quad* Create(float width, float height)
{
    Quad* quad = malloc(sizeof(Quad));
    Init(width, height, quad);

    return quad;
}


static void GetMaxSize(Array(Quad)* quadArr,  float* outWidth, float* outHeight)
{
    Quad* quads = AArray_GetData(quadArr, Quad);
    Quad* quad  = quads;

    float  minX  = quad->offsetCenterX;
    float  maxY  = quad->offsetCenterY;
    
    float  maxX  = minX + quad->width;
    float  minY  = maxY - quad->height;

    for (int i = 1; i < quadArr->length; ++i)
    {
        quad         = quads + i;

        float leftX   = quad->offsetCenterX;
        float topY    = quad->offsetCenterY;
        
        float rightX  = leftX + quad->width;
        float bottomY = topY - quad->height;

        // find the min x
        if (leftX < minX)
        {
            minX = leftX;
        }

        // find the max y
        if (topY > maxY)
        {
            maxY = topY;
        }

        // find the max x
        if (rightX > maxX)
        {
            maxX = rightX;
        }

        // find the min y
        if (bottomY < minY)
        {
            minY = bottomY;
        }
    }

    *outWidth  = maxX - minX;
    *outHeight = maxY - minY;
}


static void GetPosition2UV(Quad* quad, Texture* texture, float outPosition2UVData[Quad_Position2UVNum])
{
    float qx = quad->offsetCenterX;
    float qy = quad->offsetCenterY;

    float qw = qx + quad->width;
    float qh = qy - quad->height;

    float tx = AGLTool_ToUVWidth (quad->offsetTextureX, texture->width);
    float ty = AGLTool_ToUVHeight(quad->offsetTextureY, texture->height);

    float tw = tx + AGLTool_ToUVWidth (quad->width,  texture->width);
    float th = ty + AGLTool_ToUVHeight(quad->height, texture->height);

    memcpy
    (
        outPosition2UVData,
        (float[])
        {
            qx, qy, // position 0
            tx, ty, // texCoord 0

            qx, qh, // position 1
            tx, th, // texCoord 1

            qw, qh, // position 2
            tw, th, // texCoord 2

            qw, qy, // position 3
            tw, ty, // texCoord 3
        },
        Quad_Position2UVSize
    );
}


static void GetPosition3(Quad* quad, float outBornPositionData[Quad_Position3Num])
{
    float qx = quad->offsetCenterX;
    float qy = quad->offsetCenterY;

    float qw = qx + quad->width;
    float qh = qy - quad->height;

    memcpy
    (
        outBornPositionData,
        (float[])
        {
            qx, qy, 0.0f, // position 0
        
            qx, qh, 0.0f, // position 1
        
            qw ,qh, 0.0f, // position 2
        
            qw, qy, 0.0f, // position 3
        },
        Quad_Position3Size
    );
}


static void GetUV(Quad* quad, Texture* texture, float outUVData[Quad_UVNum])
{
    float tx = AGLTool_ToUVWidth (quad->offsetTextureX, texture->width);
    float ty = AGLTool_ToUVHeight(quad->offsetTextureY, texture->height);

    float tw = tx + AGLTool_ToUVWidth (quad->width,  texture->width);
    float th = ty + AGLTool_ToUVHeight(quad->height, texture->height);

    memcpy
    (
        outUVData,
        (float[])
        {
            tx, ty, // texCoord 0

            tx, th, // texCoord 1

            tw, th, // texCoord 2

            tw, ty, // texCoord 3
        },
        Quad_UVSize
    );
}


static void GetIndex(int vertexNumBefore, short outIndexData[Quad_IndexNum])
{
    memcpy
    (
        outIndexData,
        (short[])
        {
            (short) (0 + vertexNumBefore),
            (short) (1 + vertexNumBefore),
            (short) (2 + vertexNumBefore),

            (short) (2 + vertexNumBefore),
            (short) (3 + vertexNumBefore),
            (short) (0 + vertexNumBefore),
        },
        Quad_IndexSize
    );
}


struct AQuad AQuad[1] =
{
    Create,
    Init,
    GetMaxSize,

    GetPosition2UV,
    GetPosition3,
    GetUV,
    GetIndex,
};
