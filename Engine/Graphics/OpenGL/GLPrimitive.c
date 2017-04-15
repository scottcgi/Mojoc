/*
 *
 *
 *  Created on: 2014-2-26
 *      Author: scott.cgi
 */

#include "Engine/Graphics/OpenGL/GLPrimitive.h"
#include "Engine/Graphics/OpenGL/Shader/ShaderPrimitive.h"
#include "Engine/Graphics/OpenGL/Camera.h"


static Matrix4 identityMatrix[1] = matrix4_identity;
static Matrix4 mvpMatrix     [1];


static inline void SetMatrix()
{
	AMatrix->MultiplyMM
	(
		ACamera->vp,
		AGLPrimitive->modelMatrix ? AGLPrimitive->modelMatrix : identityMatrix,
		mvpMatrix
	);

	glLineWidth(AGLPrimitive->lineWidth);
	AShaderPrimitive->Use(mvpMatrix, AGLPrimitive->color, AGLPrimitive->pointSize);
}


static void DrawPoints(Array(float)* pointArr)
{
	SetMatrix();
	glVertexAttribPointer(AShaderPrimitive->attribPosition, 2, GL_FLOAT, false, 0, pointArr->data);
	glDrawArrays(GL_POINTS, 0, pointArr->length >> 1);
}


static void DrawPolygon(Array(float)* vertexArr)
{
	SetMatrix();
	glVertexAttribPointer(AShaderPrimitive->attribPosition, 2, GL_FLOAT, false, 0, vertexArr->data);
	glDrawArrays(GL_LINE_LOOP, 0, vertexArr->length >> 1);
}


static void DrawLines(Array(float)* vertexArr)
{
	SetMatrix();
	glVertexAttribPointer(AShaderPrimitive->attribPosition, 2, GL_FLOAT, false, 0, vertexArr->data);
	glDrawArrays(GL_LINES, 0, vertexArr->length >> 1);
}


static void DrawRect(Rect* rect)
{
	SetMatrix();

	glVertexAttribPointer
	(
		AShaderPrimitive->attribPosition,
		2,
		GL_FLOAT,
		false,
		0,
		(float[])
		{
			rect->top,    rect->left,
			rect->bottom, rect->left,
			rect->bottom, rect->right,
			rect->top,    rect->right,
		}
	);

	glDrawArrays(GL_LINE_LOOP, 0, 4);
}


struct AGLPrimitive AGLPrimitive[1] =
{
	1.0f,
	1.0f,

	color_white,
	NULL,

	DrawPoints,
	DrawPolygon,
	DrawLines,
	DrawRect,
};
