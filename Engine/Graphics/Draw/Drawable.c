/*
 *
 *
 *  Created on: 2013-1-2
 *  Author: scott.cgi
 */

#include <stdlib.h>
#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/Camera.h"

static ArrayList(Drawable*) renderQueue[1] = AArrayListInit(Drawable*, 150);


static void Draw(Drawable* drawable)
{
	if (ADrawableCheckVisible(drawable))
	{
		bool isHasParent = drawable->parent != NULL;

		// parent not has drawable_state_draw_ed state
		if (isHasParent && (ADrawableCheckState(drawable->parent, drawable_state_draw_ed) == false))
		{
			ADrawableClearState(drawable, drawable_state_draw_ed);
			return;
		}

		// check transform flag
		if
        (
            ADrawableCheckState(drawable, drawable_state_transform)
            ||
            (isHasParent && ADrawableCheckState(drawable->parent, drawable_state_transform_ed))
        )
		{
			if (isHasParent)
			{
				// child under father matrix
				*drawable->modelMatrix = *drawable->parent->modelMatrix;
			}
			else
			{
				// self under identity matrix
				*drawable->modelMatrix = *(Matrix4[]) matrix4_identity;
			}

			AMatrix->Translate
			(
				drawable->modelMatrix,
				drawable->positionX,
				drawable->positionY,
				drawable->positionZ
			);

			AMatrix->Scale
			(
				drawable->modelMatrix,
				drawable->scaleX,
				drawable->scaleY,
				drawable->scaleZ
			);

			if (drawable->rotationZ != 0.0f)
			{
				AMatrix->RotateZ(drawable->modelMatrix, drawable->rotationZ);
			}

			if (ADrawableCheckState(drawable, drawable_state_is_update_mvp))
			{
				AMatrix->MultiplyMM(ACamera->vp, drawable->modelMatrix, drawable->mvpMatrix);
			}

			// flag transform for child and flag need update inverse
			ADrawableSetState(drawable, drawable_state_transform_ed | drawable_state_update_inverse);
		}
		else
		{
			ADrawableClearState(drawable, drawable_state_transform_ed);
		}

//--------------------------------------------------------------------------------------------------

		// check color flag
		if
        (
            ADrawableCheckState(drawable, drawable_state_color)
            ||
            (isHasParent && ADrawableCheckState(drawable->parent, drawable_state_color_ed))
		)
		{
			if (isHasParent && ADrawableCheckState(drawable, drawable_state_is_blend_color))
			{
				if (ADrawableCheckState(drawable, drawable_state_opacity) || ADrawableCheckState(drawable->parent, drawable_state_opacity_ed))
				{
					// blend opacity
					drawable->blendColor->a = drawable->color->a * drawable->parent->blendColor->a;

					// flag rgb for child
					ADrawableSetState(drawable, drawable_state_opacity_ed);
				}
				else
				{
					ADrawableClearState(drawable, drawable_state_opacity_ed);
				}

				if (ADrawableCheckState(drawable, drawable_state_rgb) || ADrawableCheckState(drawable->parent, drawable_state_rgb_ed))
				{
					// blend rgb
					drawable->blendColor->r = drawable->color->r * drawable->parent->blendColor->r;
					drawable->blendColor->g = drawable->color->g * drawable->parent->blendColor->g;
					drawable->blendColor->b = drawable->color->b * drawable->parent->blendColor->b;

					// flag opacity for child
					ADrawableSetState(drawable, drawable_state_rgb_ed);
				}
				else
				{
					ADrawableClearState(drawable, drawable_state_rgb_ed);
				}
			}
			else
			{
				if (ADrawableCheckState(drawable, drawable_state_opacity))
				{
					// sync opacity
					drawable->blendColor->a = drawable->color->a;

					// flag rgb for child
					ADrawableSetState(drawable, drawable_state_opacity_ed);
				}
				else
				{
					ADrawableClearState(drawable, drawable_state_opacity_ed);
				}

				if (ADrawableCheckState(drawable, drawable_state_rgb))
				{
					// sync rgb
					drawable->blendColor->r = drawable->color->r;
					drawable->blendColor->g = drawable->color->g;
					drawable->blendColor->b = drawable->color->b;

					// flag opacity for child
					ADrawableSetState(drawable, drawable_state_rgb_ed);
				}
				else
				{
					ADrawableClearState(drawable, drawable_state_rgb_ed);
				}
			}
		}
		else
		{
			ADrawableClearState(drawable, drawable_state_color_ed);
		}

//--------------------------------------------------------------------------------------------------

		// custom handler
		if (drawable->Draw != NULL)
		{
			// we can check all state change
			drawable->Draw(drawable);
		}

		// clear self all state change
		ADrawableClearAndSetState(drawable, drawable_state_change, drawable_state_draw_ed);

		// if has render push into queue
		if (drawable->Render != NULL)
		{
			AArrayListAdd(renderQueue, drawable);
		}
	}
	else
	{
		ADrawableClearState(drawable, drawable_state_draw_ed);
	}
}


static void RenderQueue()
{
    Drawable* drawable;

	for (int i = 0;;)
	{
        while (i + 19 < renderQueue->size)
        {
            drawable = AArrayListGet(renderQueue, i,      Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 1,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 2,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 3,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 4,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 5,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 6,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 7,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 8,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 9,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 10, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 11, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 12, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 13, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 14, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 15, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 16, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 17, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 18, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 19, Drawable*);
            drawable->Render(drawable);

            i += 20;
        }

        while (i + 9 < renderQueue->size)
        {
            drawable = AArrayListGet(renderQueue, i,     Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 1, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 2, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 3, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 4, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 5, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 6, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 7, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 8, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 9, Drawable*);
            drawable->Render(drawable);

            i += 10;
        }

        while (i + 4 < renderQueue->size)
        {
            drawable = AArrayListGet(renderQueue, i,     Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 1, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 2, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 3, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayListGet(renderQueue, i + 4, Drawable*);
            drawable->Render(drawable);

            i += 5;
        }

        while (i < renderQueue->size)
        {
            drawable = AArrayListGet(renderQueue, i++, Drawable*);
            drawable->Render(drawable);
        }

        break;
	}

	AArrayList->Clear(renderQueue);
}

//--------------------------------------------------------------------------------------------------

static inline float GetWorldRotationZ(Drawable* drawable)
{
	// unit x axis vector's x and y is matrix->m0, matrix->m1
	return AMathAtan2(drawable->modelMatrix->m0, drawable->modelMatrix->m1);
}


static inline float GetWorldScaleX(Drawable* drawable)
{
	int       sign   = drawable->scaleX < 0.0f ? -1 : 1;
	Drawable* parent = drawable->parent;

	while (parent != NULL)
	{
		if (parent->scaleX < 0.0f)
		{
			sign = -sign;
		}

		parent = parent->parent;
	}

	// The scale in the x,y,z axis is the length of the matrix column vector values
	Vector3 vector[1] =
	{
		{
			drawable->modelMatrix->m0,
			drawable->modelMatrix->m1,
			drawable->modelMatrix->m2,
		}
	};

	return AVector3_Length(vector) * sign;
}

static inline float GetWorldScaleY(Drawable* drawable)
{
	int       sign   = drawable->scaleY < 0.0f ? -1 : 1;
	Drawable* parent = drawable->parent;

	while (parent != NULL)
	{
		if (parent->scaleY < 0.0f)
		{
			sign = -sign;
		}

		parent = parent->parent;
	}

	// The scale in the x,y,z axis is the length of the matrix column vector values
	Vector3 vector[1] =
	{
		{
			drawable->modelMatrix->m4,
			drawable->modelMatrix->m5,
			drawable->modelMatrix->m6,
		}
	};

	return AVector3_Length(vector) * sign;
}



//--------------------------------------------------------------------------------------------------

static float ConvertToWorldX(Drawable* localParent, float localX)
{
	return AMatrix->MultiplyMX(localParent->modelMatrix, localX);
}


static float ConvertToWorldY(Drawable* localParent, float localY)
{
	return AMatrix->MultiplyMY(localParent->modelMatrix, localY);
}

static void ConvertToWorldPoint(Drawable* localParent, Vector2* localPoint, Vector2* outWorldPoint)
{
	AMatrix->MultiplyMV2(localParent->modelMatrix, localPoint->x, localPoint->y , outWorldPoint);
}



static float ConvertToLocalX(Drawable* localParent, float worldX)
{
	if (ADrawableCheckState(localParent, drawable_state_update_inverse))
	{
		ADrawableClearState(localParent, drawable_state_update_inverse);
		AMatrix->Inverse(localParent->modelMatrix, localParent->inverseMatrix);
	}

	return AMatrix->MultiplyMX(localParent->inverseMatrix, worldX);
}


static float ConvertToLocalY(Drawable* localParent, float worldY)
{
	if (ADrawableCheckState(localParent, drawable_state_update_inverse))
	{
		ADrawableClearState(localParent, drawable_state_update_inverse);
		AMatrix->Inverse(localParent->modelMatrix, localParent->inverseMatrix);
	}

	return AMatrix->MultiplyMY(localParent->inverseMatrix, worldY);
}

static void ConvertToLocalPoint(Drawable* localParent, Vector2* worldPoint, Vector2* outLocalPoint)
{
	if (ADrawableCheckState(localParent, drawable_state_update_inverse))
	{
		ADrawableClearState(localParent, drawable_state_update_inverse);
		AMatrix->Inverse(localParent->modelMatrix, localParent->inverseMatrix);
	}

	AMatrix->MultiplyMV2(localParent->inverseMatrix, worldPoint->x, worldPoint->y, outLocalPoint);
}


static void ConvertToParent(Drawable* drawable, Drawable* parent)
{
	Vector2 worldPoint[1];
	float   worldRotationZ = GetWorldRotationZ(drawable);
	float   worldScaleX    = GetWorldScaleX   (drawable);
	float   worldScaleY    = GetWorldScaleY   (drawable);

	if (drawable->parent != NULL)
	{
		AMatrix->MultiplyMV2(drawable->parent->modelMatrix, drawable->positionX, drawable->positionY , worldPoint);
	}
	else
	{
		Matrix4 model[1] = matrix4_identity;
		AMatrix->MultiplyMV2(model, drawable->positionX, drawable->positionY , worldPoint);
	}

	float rotationZ;

	if (parent != NULL)
	{
		Vector2 localPoint[1];
		ConvertToLocalPoint(parent, worldPoint, localPoint);

		ADrawableSetPosition2(drawable, localPoint->x, localPoint->y);

		float parentScaleX   = GetWorldScaleX(parent);
		float parentScaleY   = GetWorldScaleY(parent);

		ALogA(parentScaleX  != 0.0f, "ConvertToParent failed, parent getWorldScaleX can not 0.0f");
		ALogA(parentScaleY  != 0.0f, "ConvertToParent failed, parent getWorldScaleY can not 0.0f");

		ADrawableSetScale2(drawable, worldScaleX / parentScaleX, worldScaleY / parentScaleY);

		// if parent flipped Convert world rotationZ to parent flipped coordinate

		float parentRotationZ = GetWorldRotationZ(parent);

		if (parentScaleX < 0.0f)
		{
			worldRotationZ  = 180.0f - worldRotationZ;
			parentRotationZ = 180.0f - parentRotationZ;
		}

		if (parentScaleY < 0.0f)
		{
			worldRotationZ  = -worldRotationZ;
			parentRotationZ = -parentRotationZ;
		}

		// delta rotationZ between parent to world
		rotationZ = worldRotationZ - parentRotationZ;
	}
	else
	{
		rotationZ = worldRotationZ;

		ADrawableSetPosition2(drawable, worldPoint->x, worldPoint->y);
		ADrawableSetScale2   (drawable, worldScaleX,   worldScaleY);
	}

	if (drawable->scaleX < 0.0f)
	{
		rotationZ = 180.0f - drawable->rotationZ;
	}

	if (drawable->scaleY < 0.0f)
	{
		rotationZ = -drawable->rotationZ;
	}

	ADrawableSetRotationZ(drawable, rotationZ);
	ADrawableSetParent   (drawable, parent);
}


static float ConvertBetweenLocalX(Drawable* parentA, float localXA, Drawable* parentB)
{
	ALogA(parentA != NULL, "ConvertBetweenLocalX parentA not NULL");
	ALogA(parentB != NULL, "ConvertBetweenLocalX parentB not NULL");

	return ConvertToLocalX(parentB, AMatrix->MultiplyMX(parentA->modelMatrix, localXA));
}


static float ConvertBetweenLocalY(Drawable* parentA, float localYA, Drawable* parentB)
{
	ALogA(parentA != NULL, "ConvertBetweenLocalY parentA not NULL");
	ALogA(parentB != NULL, "ConvertBetweenLocalY parentB not NULL");

	return ConvertToLocalY(parentB, AMatrix->MultiplyMY(parentA->modelMatrix, localYA));
}

static void ConvertBetweenLocal(Drawable* parentA, Vector2* localPointA, Drawable* parentB, Vector2* outLocalPointB)
{
	ALogA(parentA        != NULL, "ConvertBetweenLocal parentA     not NULL");
	ALogA(localPointA    != NULL, "ConvertBetweenLocal localPointA not NULL");
	ALogA(parentB        != NULL, "ConvertBetweenLocal parentB     not NULL");
	ALogA(outLocalPointB != NULL, "ConvertBetweenLocal localPointB not NULL");

	Vector2 worldPoint[1];
	AMatrix->MultiplyMV2(parentA->modelMatrix, localPointA->x, localPointA->y, worldPoint);
	ConvertToLocalPoint(parentB, worldPoint, outLocalPointB);
}

//--------------------------------------------------------------------------------------------------



static float GetFlipRotationZ(Drawable* drawable, float rotationZ)
{
	if (drawable->scaleX < 0.0f)
	{
		rotationZ = 180.0f - rotationZ;
	}

	if (drawable->scaleY < 0.0f)
	{
		rotationZ = -rotationZ; // same as 360.0f - rotationZ;
	}

	return rotationZ;
}




static void Init(Drawable* outDrawable)
{
	outDrawable->userData      = NULL;
	outDrawable->width         = 0.0f;
	outDrawable->height        = 0.0f;

//--------------------------------------------------------------------------------------------------

	outDrawable->parent        = NULL;

	outDrawable->positionX     = 0.0f;
	outDrawable->positionY     = 0.0f;
	outDrawable->positionZ     = 0.0f;

	outDrawable->scaleX        = 1.0f;
	outDrawable->scaleY        = 1.0f;
	outDrawable->scaleZ        = 1.0f;

	outDrawable->rotationZ     = 0.0f;

	outDrawable->color->r      = 1.0f;
	outDrawable->color->g      = 1.0f;
	outDrawable->color->b      = 1.0f;
	outDrawable->color->a      = 1.0f;

	outDrawable->blendColor->r = 1.0f;
	outDrawable->blendColor->g = 1.0f;
	outDrawable->blendColor->b = 1.0f;
	outDrawable->blendColor->a = 1.0f;

//--------------------------------------------------------------------------------------------------

   *outDrawable->modelMatrix   = *(Matrix4[]) matrix4_identity;
	outDrawable->state         = 0;
	outDrawable->Draw          = NULL;
	outDrawable->Render        = NULL;

	// first born make matrix update
	// first born inverse matrix need update
	ADrawableSetState
	(
		outDrawable,
		drawable_state_transform      |
		drawable_state_update_inverse |
		drawable_state_color          |
		drawable_state_is_blend_color |
		drawable_state_draw_ed
	);
}

static Drawable* Create()
{
	Drawable* drawable = (Drawable*) malloc(sizeof(Drawable));
	Init(drawable);

	return drawable;
}


struct ADrawable ADrawable[1] =
{
	Create,
	Init,

	Draw,
	RenderQueue,

	ConvertToWorldX,
	ConvertToWorldY,
	ConvertToWorldPoint,

	ConvertToLocalX,
	ConvertToLocalY,
	ConvertToLocalPoint,

	ConvertToParent,

	ConvertBetweenLocalX,
	ConvertBetweenLocalY,
	ConvertBetweenLocal,

	GetFlipRotationZ,
	GetWorldRotationZ,
	GetWorldScaleX,
	GetWorldScaleY,
};
