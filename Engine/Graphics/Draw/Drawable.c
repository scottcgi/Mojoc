/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-1-2
 * Author: scott.cgi
 */

#include <stdlib.h>
#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/Camera.h"


static ArrayList(Drawable*) renderQueue[1] = AArrayList_Init(Drawable*, 150);


static void Draw(Drawable* drawable)
{
    if (ADrawable_CheckVisible(drawable))
    {
        bool isHasParent = drawable->parent != NULL;

        // parent not has DrawableState_DrawChanged state
        if (isHasParent && (ADrawable_CheckState(drawable->parent, DrawableState_DrawChanged) == false))
        {
            ADrawable_ClearState(drawable, DrawableState_DrawChanged);
            return;
        }

        // check transform flag
        if
        (
            ADrawable_CheckState(drawable, DrawableState_Transform)
            ||
            (isHasParent && ADrawable_CheckState(drawable->parent, DrawableState_TransformChanged))
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
                *drawable->modelMatrix = *(Matrix4[]) MATRIX4_IDENTITY;
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

            if (ADrawable_CheckState(drawable, DrawableState_IsUpdateMVP))
            {
                AMatrix->MultiplyMM(ACamera->vp, drawable->modelMatrix, drawable->mvpMatrix);
            }

            // flag transform for child and flag need update inverse
            ADrawable_SetState(drawable, DrawableState_TransformChanged | DrawableState_UpdateInverse);
        }
        else
        {
            ADrawable_ClearState(drawable, DrawableState_TransformChanged);
        }

//----------------------------------------------------------------------------------------------------------------------

        // check color flag
        if
        (
            ADrawable_CheckState(drawable, DrawableState_Color)
            ||
            (isHasParent && ADrawable_CheckState(drawable->parent, DrawableState_ColorChanged))
        )
        {
            if (isHasParent && ADrawable_CheckState(drawable, DrawableState_IsBlendColor))
            {
                if (ADrawable_CheckState(drawable, DrawableState_Opacity) || ADrawable_CheckState(drawable->parent, DrawableState_OpacityChanged))
                {
                    // blend opacity
                    drawable->blendColor->a = drawable->color->a * drawable->parent->blendColor->a;

                    // flag rgb for child
                    ADrawable_SetState(drawable, DrawableState_OpacityChanged);
                }
                else
                {
                    ADrawable_ClearState(drawable, DrawableState_OpacityChanged);
                }

                if (ADrawable_CheckState(drawable, DrawableState_RGB) || ADrawable_CheckState(drawable->parent, DrawableState_RGBChanged))
                {
                    // blend rgb
                    drawable->blendColor->r = drawable->color->r * drawable->parent->blendColor->r;
                    drawable->blendColor->g = drawable->color->g * drawable->parent->blendColor->g;
                    drawable->blendColor->b = drawable->color->b * drawable->parent->blendColor->b;

                    // flag opacity for child
                    ADrawable_SetState(drawable, DrawableState_RGBChanged);
                }
                else
                {
                    ADrawable_ClearState(drawable, DrawableState_RGBChanged);
                }
            }
            else
            {
                if (ADrawable_CheckState(drawable, DrawableState_Opacity))
                {
                    // sync opacity
                    drawable->blendColor->a = drawable->color->a;

                    // flag rgb for child
                    ADrawable_SetState(drawable, DrawableState_OpacityChanged);
                }
                else
                {
                    ADrawable_ClearState(drawable, DrawableState_OpacityChanged);
                }

                if (ADrawable_CheckState(drawable, DrawableState_RGB))
                {
                    // sync rgb
                    drawable->blendColor->r = drawable->color->r;
                    drawable->blendColor->g = drawable->color->g;
                    drawable->blendColor->b = drawable->color->b;

                    // flag opacity for child
                    ADrawable_SetState(drawable, DrawableState_RGBChanged);
                }
                else
                {
                    ADrawable_ClearState(drawable, DrawableState_RGBChanged);
                }
            }
        }
        else
        {
            ADrawable_ClearState(drawable, DrawableState_ColorChanged);
        }

//----------------------------------------------------------------------------------------------------------------------

        // custom handler
        if (drawable->Draw != NULL)
        {
            // we can check all state change
            drawable->Draw(drawable);
        }

        // clear self all state change
        ADrawable_ClearAndSetState(drawable, DrawableState_Change, DrawableState_DrawChanged);

        // if has render push into queue
        if (drawable->Render != NULL)
        {
            AArrayList_Add(renderQueue, drawable);
        }
    }
    else
    {
        ADrawable_ClearState(drawable, DrawableState_DrawChanged);
    }
}


static void RenderQueue()
{
    Drawable* drawable;

    for (int i = 0;;)
    {
        while (i + 19 < renderQueue->size)
        {
            drawable = AArrayList_Get(renderQueue, i,      Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 1,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 2,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 3,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 4,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 5,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 6,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 7,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 8,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 9,  Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 10, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 11, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 12, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 13, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 14, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 15, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 16, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 17, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 18, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 19, Drawable*);
            drawable->Render(drawable);

            i += 20;
        }

        while (i + 9 < renderQueue->size)
        {
            drawable = AArrayList_Get(renderQueue, i,     Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 1, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 2, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 3, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 4, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 5, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 6, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 7, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 8, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 9, Drawable*);
            drawable->Render(drawable);

            i += 10;
        }

        while (i + 4 < renderQueue->size)
        {
            drawable = AArrayList_Get(renderQueue, i,     Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 1, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 2, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 3, Drawable*);
            drawable->Render(drawable);

            drawable = AArrayList_Get(renderQueue, i + 4, Drawable*);
            drawable->Render(drawable);

            i += 5;
        }

        while (i < renderQueue->size)
        {
            drawable = AArrayList_Get(renderQueue, i++, Drawable*);
            drawable->Render(drawable);
        }

        break;
    }

    AArrayList->Clear(renderQueue);
}


//----------------------------------------------------------------------------------------------------------------------


static inline float GetWorldRotationZ(Drawable* drawable)
{
    // unit x axis vector's x and y is matrix->m0, matrix->m1
    return AMath_Atan2(drawable->modelMatrix->m0, drawable->modelMatrix->m1);
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

    // the scale in the x,y,z axis is the length of the matrix column vector values
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


//----------------------------------------------------------------------------------------------------------------------


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
    if (ADrawable_CheckState(localParent, DrawableState_UpdateInverse))
    {
        ADrawable_ClearState(localParent, DrawableState_UpdateInverse);
        AMatrix->Inverse(localParent->modelMatrix, localParent->inverseMatrix);
    }

    return AMatrix->MultiplyMX(localParent->inverseMatrix, worldX);
}


static float ConvertToLocalY(Drawable* localParent, float worldY)
{
    if (ADrawable_CheckState(localParent, DrawableState_UpdateInverse))
    {
        ADrawable_ClearState(localParent, DrawableState_UpdateInverse);
        AMatrix->Inverse(localParent->modelMatrix, localParent->inverseMatrix);
    }

    return AMatrix->MultiplyMY(localParent->inverseMatrix, worldY);
}


static void ConvertToLocalPoint(Drawable* localParent, Vector2* worldPoint, Vector2* outLocalPoint)
{
    if (ADrawable_CheckState(localParent, DrawableState_UpdateInverse))
    {
        ADrawable_ClearState(localParent, DrawableState_UpdateInverse);
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
        Matrix4 model[1] = MATRIX4_IDENTITY;
        AMatrix->MultiplyMV2(model, drawable->positionX, drawable->positionY , worldPoint);
    }

    float rotationZ;

    if (parent != NULL)
    {
        Vector2 localPoint[1];
        ConvertToLocalPoint(parent, worldPoint, localPoint);

        ADrawable_SetPosition2(drawable, localPoint->x, localPoint->y);

        float parentScaleX   = GetWorldScaleX(parent);
        float parentScaleY   = GetWorldScaleY(parent);

        ALog_A(parentScaleX  != 0.0f, "ADrawable ConvertToParent failed, parent getWorldScaleX can not 0.0f");
        ALog_A(parentScaleY  != 0.0f, "ADrawable ConvertToParent failed, parent getWorldScaleY can not 0.0f");

        ADrawable_SetScale2(drawable, worldScaleX / parentScaleX, worldScaleY / parentScaleY);

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

        ADrawable_SetPosition2(drawable, worldPoint->x, worldPoint->y);
        ADrawable_SetScale2   (drawable, worldScaleX,   worldScaleY);
    }

    if (drawable->scaleX < 0.0f)
    {
        rotationZ = 180.0f - drawable->rotationZ;
    }

    if (drawable->scaleY < 0.0f)
    {
        rotationZ = -drawable->rotationZ;
    }

    ADrawable_SetRotationZ(drawable, rotationZ);
    ADrawable_SetParent   (drawable, parent);
}


static float ConvertBetweenLocalX(Drawable* parentA, float localXA, Drawable* parentB)
{
    ALog_A(parentA != NULL, "ADrawable ConvertBetweenLocalX parentA not NULL");
    ALog_A(parentB != NULL, "ADrawable ConvertBetweenLocalX parentB not NULL");

    return ConvertToLocalX(parentB, AMatrix->MultiplyMX(parentA->modelMatrix, localXA));
}


static float ConvertBetweenLocalY(Drawable* parentA, float localYA, Drawable* parentB)
{
    ALog_A(parentA != NULL, "ADrawable ConvertBetweenLocalY parentA not NULL");
    ALog_A(parentB != NULL, "ADrawable ConvertBetweenLocalY parentB not NULL");

    return ConvertToLocalY(parentB, AMatrix->MultiplyMY(parentA->modelMatrix, localYA));
}


static void ConvertBetweenLocal(Drawable* parentA, Vector2* localPointA, Drawable* parentB, Vector2* outLocalPointB)
{
    ALog_A(parentA        != NULL, "ADrawable ConvertBetweenLocal parentA        not NULL");
    ALog_A(localPointA    != NULL, "ADrawable ConvertBetweenLocal localPointA    not NULL");
    ALog_A(parentB        != NULL, "ADrawable ConvertBetweenLocal parentB        not NULL");
    ALog_A(outLocalPointB != NULL, "ADrawable ConvertBetweenLocal outLocalPointB not NULL");

    Vector2 worldPoint[1];
    AMatrix->MultiplyMV2(parentA->modelMatrix, localPointA->x, localPointA->y, worldPoint);
    ConvertToLocalPoint(parentB, worldPoint, outLocalPointB);
}


//----------------------------------------------------------------------------------------------------------------------


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
    AUserData_Init(outDrawable->userData);
    outDrawable->width         = 0.0f;
    outDrawable->height        = 0.0f;

//----------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------

   *outDrawable->modelMatrix   = *(Matrix4[]) MATRIX4_IDENTITY;
    outDrawable->state         = 0;
    outDrawable->Draw          = NULL;
    outDrawable->Render        = NULL;

    // first born make matrix update
    // first born inverse matrix need update
    ADrawable_SetState
    (
        outDrawable,
        DrawableState_Transform     |
        DrawableState_UpdateInverse |
        DrawableState_Color         |
        DrawableState_IsBlendColor  |
        DrawableState_DrawChanged
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
