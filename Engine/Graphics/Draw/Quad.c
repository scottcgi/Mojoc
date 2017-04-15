/*
 *
 *
 *  Created on: 2012-12-25
 *  Author: scott.cgi
 */

#include <string.h>
#include <stdlib.h>

#include "Engine/Graphics/Draw/Quad.h"
#include "Engine/Graphics/OpenGL/GLTool.h"


static void Init(float width, float height, Quad* out_param quad)
{
	quad->width          =  width;
	quad->height         =  height;
	
	quad->offsetCenterX  = -width  / 2;
	quad->offsetCenterY  =  height / 2;
	
	quad->offsetTextureX =  0.0f;
	quad->offsetTextureY =  0.0f;
}


static Quad* Create(float width, float height)
{
	Quad* quad = (Quad*) malloc(sizeof(Quad));
	Init(width, height, quad);

	return quad;
}


static void MaxSize(Array(Quad)* quadArr,  float* out_param width, float* out_param height)
{
	Quad* quads  = AArrayGetData(quadArr, Quad);
    Quad* quad   = quads;

    float leftX  = quad->offsetCenterX;
    float leftY  = quad->offsetCenterY;

    float rightX = leftX + quad->width;
    float rightY = leftY - quad->height;

    for (int i = 1; i < quadArr->length; i++)
    {
    	quad            = quads + i;
        float tmpLeftX  = quad->offsetCenterX;
        float tmpLeftY  = quad->offsetCenterY;

        float tmpRightX = tmpLeftX + quad->width;
        float tmpRightY = tmpLeftY - quad->height;

        // find the min x
        if (tmpLeftX < leftX)
        {
            leftX = tmpLeftX;
        }

        // find the max y
        if (tmpLeftY > leftY)
        {
            leftY = tmpLeftY;
        }

        // find the max x
        if (tmpRightX > rightX)
        {
            rightX = tmpRightX;
        }

        // find the min y
        if (tmpRightY < rightY)
        {
            rightY = tmpRightY;
        }
    }

    *width  = rightX - leftX;
    *height = leftY  - rightY;
}


static void GetQuadVertex(Quad* quad, Texture* texture, float out_param vertexData[quad_vertex_num])
{
    float qx = quad->offsetCenterX;
    float qy = quad->offsetCenterY;

    float qw = qx + quad->width;
    float qh = qy - quad->height;

    float tx = AGLToolToUVWidth (quad->offsetTextureX, texture->width);
    float ty = AGLToolToUVHeight(quad->offsetTextureY, texture->height);

    float tw = tx + AGLToolToUVWidth (quad->width,  texture->width);
    float th = ty + AGLToolToUVHeight(quad->height, texture->height);

    memcpy
	(
		vertexData,
        (
            (float[])
            {
                qx, qy, // Position 0
                tx, ty, // TexCoord 0
          
                qx, qh, // Position 1
                tx, th, // TexCoord 1
          
                qw, qh, // Position 2
                tw, th, // TexCoord 2
          
                qw, qy, // Position 3
                tw, ty, // TexCoord 3
            }
        ),
		quad_vertex_num_byte
	);
}


static void GetQuadPosition3(Quad* quad, float out_param bornPositionData[quad_position3_num])
{
    float qx = quad->offsetCenterX;
    float qy = quad->offsetCenterY;

    float qw = qx + quad->width;
    float qh = qy - quad->height;

    memcpy
	(
		bornPositionData,
        (
            (float[])
            {
                qx, qy, 0.0f, // Position 0
        
                qx, qh, 0.0f, // Position 1
        
                qw ,qh, 0.0f, // Position 2
        
                qw, qy, 0.0f, // Position 3
            }
         ),
		quad_position3_num_byte
	);
}


static void GetQuadUV(Quad* quad, Texture* texture, float out_param uvData[quad_uv_num])
{
    float tx = AGLToolToUVWidth (quad->offsetTextureX, texture->width);
    float ty = AGLToolToUVHeight(quad->offsetTextureY, texture->height);

    float tw = tx + AGLToolToUVWidth (quad->width,  texture->width);
    float th = ty + AGLToolToUVHeight(quad->height, texture->height);

    memcpy
	(
		uvData,
		(
            (float[])
            {
                tx, ty, // TexCoord 0

                tx, th, // TexCoord 1

                tw, th, // TexCoord 2
                
                tw, ty, // TexCoord 3
            }
        ),
		quad_uv_num_byte
	);
}


static void GetQuadIndex(int vertexNumBefore, short out_param indexData[quad_index_num])
{
    memcpy
	(
		indexData,
		((short[])
		{
			(short) (0 + vertexNumBefore),
			(short) (1 + vertexNumBefore),
			(short) (2 + vertexNumBefore),

			(short) (2 + vertexNumBefore),
			(short) (3 + vertexNumBefore),
			(short) (0 + vertexNumBefore),
		}),
		quad_index_num_byte
	);
}


struct AQuad AQuad[1] =
{
	Create,
	Init,
	MaxSize,

	GetQuadVertex,
	GetQuadPosition3,
	GetQuadUV,
	GetQuadIndex,
};
