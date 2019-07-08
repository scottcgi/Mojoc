/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2012-12-25
 * Update   : 2019-1-23
 * Author   : scott.cgi
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
    Quad* quads = quadArr->data;
    Quad* quad  = quads;

    float  minX  = quad->offsetCenterX;
    float  maxY  = quad->offsetCenterY;
    
    float  maxX  = minX + quad->width;
    float  minY  = maxY - quad->height;

    for (int i = 1; i < quadArr->length; ++i)
    {
        quad          = quads + i;

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
        (float[Quad_Position2UVNum])
        {
            qx, qy, // position 0, top    left
            tx, ty, // texcoord 0, top    left

            qx, qh, // position 1, bottom left
            tx, th, // texcoord 1, bottom left

            qw, qh, // position 2, bottom right
            tw, th, // texcoord 2, bottom right

            qw, qy, // position 3, top    right
            tw, ty, // texcoord 3, top    right
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
        (float[Quad_Position3Num])
        {
            qx, qy, 0.0f, // position 0, top    left
            qx, qh, 0.0f, // position 1, bottom left
            qw ,qh, 0.0f, // position 2, bottom right
            qw, qy, 0.0f, // position 3, top    right
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
        (float[Quad_UVNum])
        {
            tx, ty, // texCoord 0, top    left
            tx, th, // texCoord 1, bottom left
            tw, th, // texCoord 2, bottom right
            tw, ty, // texCoord 3, top    right
        },
        Quad_UVSize
    );
}


static void GetIndex(int vertexNumBefore, short outIndexData[Quad_IndexNum])
{
    memcpy
    (
        outIndexData,
        (short[Quad_IndexNum])
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
