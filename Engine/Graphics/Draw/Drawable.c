/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-1-2
 * Author: scott.cgi
 */

#include <stdlib.h>
#include <Engine/Toolkit/Math/Vector.h>
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
                *drawable->modelMatrix = *(Matrix4[1]) MATRIX4_IDENTITY;
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
            ADrawable_AddState(drawable, DrawableState_TransformChanged | DrawableState_UpdateInverse);
        }
        else
        {
            ADrawable_ClearState(drawable, DrawableState_TransformChanged);
        }

//----------------------------------------------------------------------------------------------------------------------

        // check color flag
        if
        (
            ADrawable_CheckState(drawable, DrawableState_Color) ||
            (isHasParent && ADrawable_CheckState(drawable->parent, DrawableState_ColorChanged))
        )
        {
            if (isHasParent && ADrawable_CheckState(drawable, DrawableState_IsBlendColor))
            {
                if
                (
                    ADrawable_CheckState(drawable, DrawableState_Opacity) ||
                    ADrawable_CheckState(drawable->parent, DrawableState_OpacityChanged)
                )
                {
                    // blend opacity
                    drawable->blendColor->a = drawable->color->a * drawable->parent->blendColor->a;

                    // flag rgb for child
                    ADrawable_AddState(drawable, DrawableState_OpacityChanged);
                }
                else
                {
                    ADrawable_ClearState(drawable, DrawableState_OpacityChanged);
                }

                if
                (
                    ADrawable_CheckState(drawable, DrawableState_RGB) ||
                    ADrawable_CheckState(drawable->parent, DrawableState_RGBChanged)
                )
                {
                    // blend rgb
                    drawable->blendColor->r = drawable->color->r * drawable->parent->blendColor->r;
                    drawable->blendColor->g = drawable->color->g * drawable->parent->blendColor->g;
                    drawable->blendColor->b = drawable->color->b * drawable->parent->blendColor->b;

                    // flag opacity for child
                    ADrawable_AddState(drawable, DrawableState_RGBChanged);
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
                    ADrawable_AddState(drawable, DrawableState_OpacityChanged);
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
                    ADrawable_AddState(drawable, DrawableState_RGBChanged);
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

        // clear self all draw state change
        ADrawable_ClearAndAddState(drawable, DrawableState_Draw, DrawableState_DrawChanged);

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


static float GetWorldScaleX(Drawable* drawable)
{
    Drawable* parent = drawable->parent;
    int       sign   = drawable->scaleX < 0.0f ? -1 : 1;

    while (parent != NULL)
    {
        if (parent->scaleX < 0.0f)
        {
            sign = -sign;
        }

        parent = parent->parent;
    }

    // the scaleX in the x,y,z axis is the length of the matrix column vector values
    Vector3 vector[1] =
    {{
        drawable->modelMatrix->m0,
        drawable->modelMatrix->m1,
        drawable->modelMatrix->m2,
    }};

    return AVector3_Length(vector) * sign;
}


static float GetWorldScaleY(Drawable* drawable)
{
    Drawable* parent = drawable->parent;
    int       sign   = drawable->scaleY < 0.0f ? -1 : 1;

    while (parent != NULL)
    {
        if (parent->scaleY < 0.0f)
        {
            sign = -sign;
        }

        parent = parent->parent;
    }

    // the scaleY in the x,y,z axis is the length of the matrix column vector values
    Vector3 vector[1] =
    {{
        drawable->modelMatrix->m4,
        drawable->modelMatrix->m5,
        drawable->modelMatrix->m6,
    }};

    return AVector3_Length(vector) * sign;
}



static void GetWorldScaleV2(Drawable* drawable, Vector2* outScaleV2)
{
    Drawable* parent = drawable->parent;
    int       signX  = drawable->scaleX < 0.0f ? -1 : 1;
    int       signY  = drawable->scaleY < 0.0f ? -1 : 1;

    while (parent != NULL)
    {
        if (parent->scaleX < 0.0f)
        {
            signX = -signX;
        }

        if (parent->scaleY < 0.0f)
        {
            signY = -signY;
        }

        parent = parent->parent;
    }

    // the scale in the x,y,z axis is the length of the matrix column vector values

    Vector3 vectorX[1] =
    {{
         drawable->modelMatrix->m0,
         drawable->modelMatrix->m1,
         drawable->modelMatrix->m2,
     }};

    Vector3 vectorY[1] =
    {{
         drawable->modelMatrix->m4,
         drawable->modelMatrix->m5,
         drawable->modelMatrix->m6,
     }};

    outScaleV2->x = AVector3_Length(vectorX) * signX;
    outScaleV2->y = AVector3_Length(vectorY) * signY;
}


static float GetWorldPositionX(Drawable* drawable)
{
    ALog_A(drawable->parent != NULL, "ADrawable GetWorldPositionX the drawable parent cannot NULL !");
    return AMatrix->MultiplyMX(drawable->parent->modelMatrix, drawable->positionX);
}


static float GetWorldPositionY(Drawable* drawable)
{
    ALog_A(drawable->parent != NULL, "ADrawable GetWorldPositionY the drawable parent cannot NULL !");
    return AMatrix->MultiplyMX(drawable->parent->modelMatrix, drawable->positionY);
}


static void GetWorldPositionV2(Drawable* drawable, Vector2* outPositionV2)
{
    ALog_A(drawable->parent != NULL, "ADrawable GetWorldPositionV2 the drawable parent cannot NULL !");
    AMatrix->MultiplyMV2(drawable->parent->modelMatrix, drawable->positionX, drawable->positionY, outPositionV2);
}


static void GetWorldPositionV3(Drawable* drawable, Vector3* outPositionV3)
{
    ALog_A(drawable->parent != NULL, "ADrawable GetWorldPositionV3 the drawable parent cannot NULL !");
    
    AMatrix->MultiplyMV3
    (
        drawable->parent->modelMatrix,
        drawable->positionX,
        drawable->positionY,
        drawable->positionZ,
        outPositionV3
    );
}


//----------------------------------------------------------------------------------------------------------------------


static float ConvertToWorldPositionX(Drawable* localParent, float localPositionX)
{
    return AMatrix->MultiplyMX(localParent->modelMatrix, localPositionX);
}


static float ConvertToWorldPositionY(Drawable* localParent, float localPositionY)
{
    return AMatrix->MultiplyMY(localParent->modelMatrix, localPositionY);
}


static void ConvertToWorldPositionV2(Drawable* localParent, Vector2* localPositionV2, Vector2* outWorldPositionV2)
{
    AMatrix->MultiplyMV2(localParent->modelMatrix, localPositionV2->x, localPositionV2->y , outWorldPositionV2);
}


static float ConvertToLocalPositionX(Drawable* localParent, float worldPositionX)
{
    if (ADrawable_CheckState(localParent, DrawableState_UpdateInverse))
    {
        ADrawable_ClearState(localParent, DrawableState_UpdateInverse);
        AMatrix->Inverse(localParent->modelMatrix, localParent->inverseMatrix);
    }

    return AMatrix->MultiplyMX(localParent->inverseMatrix, worldPositionX);
}


static float ConvertToLocalPositionY(Drawable* localParent, float worldPositionY)
{
    if (ADrawable_CheckState(localParent, DrawableState_UpdateInverse))
    {
        ADrawable_ClearState(localParent, DrawableState_UpdateInverse);
        AMatrix->Inverse(localParent->modelMatrix, localParent->inverseMatrix);
    }

    return AMatrix->MultiplyMY(localParent->inverseMatrix, worldPositionY);
}


static void ConvertToLocalPositionV2(Drawable* localParent, Vector2* worldPositionV2, Vector2* outLocalPositionV2)
{
    if (ADrawable_CheckState(localParent, DrawableState_UpdateInverse))
    {
        ADrawable_ClearState(localParent, DrawableState_UpdateInverse);
        AMatrix->Inverse(localParent->modelMatrix, localParent->inverseMatrix);
    }

    AMatrix->MultiplyMV2(localParent->inverseMatrix, worldPositionV2->x, worldPositionV2->y, outLocalPositionV2);
}


static void ConvertToParent(Drawable* drawable, Drawable* parent)
{
    Vector2 worldPositionV2[1];
    float    worldRotationZ = GetWorldRotationZ(drawable);

    Vector2 worldScaleV2[1];
    GetWorldScaleV2(drawable, worldScaleV2);

    if (drawable->parent != NULL)
    {
        AMatrix->MultiplyMV2(drawable->parent->modelMatrix, drawable->positionX, drawable->positionY , worldPositionV2);
    }
    else
    {
        AMatrix->MultiplyMV2((Matrix4[]) MATRIX4_IDENTITY, drawable->positionX, drawable->positionY , worldPositionV2);
    }

    float rotationZ;

    if (parent != NULL)
    {
        Vector2 localPositionV2[1];
        ConvertToLocalPositionV2(parent,   worldPositionV2,    localPositionV2);
        ADrawable_SetPosition2  (drawable, localPositionV2->x, localPositionV2->y);

        Vector2 parentScaleV2[1];
        GetWorldScaleV2(parent, parentScaleV2);

        ALog_A(parentScaleV2->x != 0.0f, "ADrawable ConvertToParent failed, parent world scale x cannot 0.0f");
        ALog_A(parentScaleV2->y != 0.0f, "ADrawable ConvertToParent failed, parent world scale y cannot 0.0f");

        ADrawable_SetScale2(drawable, worldScaleV2->x / parentScaleV2->x, worldScaleV2->y / parentScaleV2->y);

        // if parent flipped Convert world rotationZ to parent flipped coordinate

        float parentRotationZ = GetWorldRotationZ(parent);

        if (parentScaleV2->x < 0.0f)
        {
            worldRotationZ  = 180.0f - worldRotationZ;
            parentRotationZ = 180.0f - parentRotationZ;
        }

        if (parentScaleV2->y < 0.0f)
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

        ADrawable_SetPosition2(drawable, worldPositionV2->x, worldPositionV2->y);
        ADrawable_SetScale2   (drawable, worldScaleV2->x,    worldScaleV2->y);
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


static float ConvertBetweenLocalPositionX(Drawable* parentA, float localPositionX, Drawable* parentB)
{
    ALog_A(parentA != NULL, "ADrawable ConvertBetweenLocalPositionX parentA cannot NULL !");
    ALog_A(parentB != NULL, "ADrawable ConvertBetweenLocalPositionX parentB cannot NULL !");

    return ConvertToLocalPositionX(parentB, AMatrix->MultiplyMX(parentA->modelMatrix, localPositionX));
}


static float ConvertBetweenLocalPositionY(Drawable* parentA, float localPositionY, Drawable* parentB)
{
    ALog_A(parentA != NULL, "ADrawable ConvertBetweenLocalPositionY parentA not NULL");
    ALog_A(parentB != NULL, "ADrawable ConvertBetweenLocalPositionY parentB not NULL");

    return ConvertToLocalPositionY(parentB, AMatrix->MultiplyMY(parentA->modelMatrix, localPositionY));
}


static void ConvertBetweenLocalPositionV2
(
    Drawable* parentA,
    Vector2*  localPositionV2,
    Drawable* parentB,
    Vector2*  outLocalPositionV2
)
{
    ALog_A(parentA            != NULL, "ADrawable ConvertBetweenLocal parentA            not NULL");
    ALog_A(localPositionV2    != NULL, "ADrawable ConvertBetweenLocal localPositionV2    not NULL");
    ALog_A(parentB            != NULL, "ADrawable ConvertBetweenLocal parentB            not NULL");
    ALog_A(outLocalPositionV2 != NULL, "ADrawable ConvertBetweenLocal outLocalPositionV2 not NULL");

    Vector2 worldPositionV2[1];
    AMatrix->MultiplyMV2    (parentA->modelMatrix,  localPositionV2->x, localPositionV2->y, worldPositionV2);
    ConvertToLocalPositionV2(parentB, worldPositionV2, outLocalPositionV2);
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

   *outDrawable->modelMatrix   = *(Matrix4[1]) MATRIX4_IDENTITY;
    outDrawable->state         = DrawableState_Null;
    outDrawable->Draw          = NULL;
    outDrawable->Render        = NULL;

    // first born make matrix update
    // first born inverse matrix need update
    ADrawable_AddState
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

    ConvertToWorldPositionX,
    ConvertToWorldPositionY,
    ConvertToWorldPositionV2,

    ConvertToLocalPositionX,
    ConvertToLocalPositionY,
    ConvertToLocalPositionV2,

    ConvertToParent,

    ConvertBetweenLocalPositionX,
    ConvertBetweenLocalPositionY,
    ConvertBetweenLocalPositionV2,

    GetFlipRotationZ,
    GetWorldRotationZ,
    GetWorldScaleX,
    GetWorldScaleY,
    GetWorldScaleV2,
    GetWorldPositionX,
    GetWorldPositionY,
    GetWorldPositionV2,
    GetWorldPositionV3,
};
