/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-1-2
 * Author: scott.cgi
 */

#ifndef DRAWABLE_H
#define  DRAWABLE_H


#include <stdbool.h>
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/HeaderUtils/Bitwise.h"
#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Graphics/Draw/Color.h"
#include "Engine/Toolkit/Math/Vector.h"
#include "Engine/Toolkit/HeaderUtils/UserData.h"


/**
 * If contains 'Is' the state can set(add) and clear
 * else the state will automatically set(add) and clear
 */
typedef enum
{
    /**
     * Whether drawable is invisible
     */
    DrawableState_IsInvisible       = 1,

    /**
     * Whether drawable mvp matrix need update
     */
    DrawableState_IsUpdateMVP       = 1 << 1,

    /**
     * Whether drawable calculate blendColor by parent
     */
    DrawableState_IsBlendColor      = 1 << 2,

    /**
     * Flag drawable inverse matrix need update
     */
    DrawableState_UpdateInverse      = 1 << 3,

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Flag drawable transform has changed
     */
    DrawableState_TransformChanged   = 1 << 4,

    /**
     * Flag drawable rgb has changed
     */
    DrawableState_RGBChanged         = 1 << 5,

    /**
     * Flag drawable opacity has changed
     */
    DrawableState_OpacityChanged     = 1 << 6,

    /**
     * Flag drawable has been drawn
     */
    DrawableState_DrawChanged        = 1 << 7,

    /**
     * Flag drawable color has changed
     */
    DrawableState_ColorChanged       = DrawableState_RGBChanged  | DrawableState_OpacityChanged,

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Flag drawable parent change
     */
    DrawableState_Parent             = 1 << 8,

    /**
     * Flag drawable position x change
     */
    DrawableState_PositionX          = 1 << 9,

    /**
     * Flag drawable position y change
     */
    DrawableState_PositionY          = 1 << 10,

    /**
     * Flag drawable position z change
     */
    DrawableState_PositionZ          = 1 << 11,

    /**
     * Flag drawable position x and y change
     */
    DrawableState_Position2          = DrawableState_PositionX | DrawableState_PositionY,

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Flag drawable scale x change
     */
    DrawableState_ScaleX             = 1 << 12,

    /**
     * Flag drawable scale y change
     */
    DrawableState_ScaleY             = 1 << 13,

    /**
     * Flag drawable scale z change
     */
    DrawableState_ScaleZ             = 1 << 14,

    /**
     * Flag drawable scale x and y change
     */
    DrawableState_Scale2             = DrawableState_ScaleX | DrawableState_ScaleY,

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Flag drawable rotation x change
     */
    DrawableState_RotationX          = 1 << 15,

    /**
     * Flag drawable rotation x change
     */
    DrawableState_RotationY          = 1 << 16,

    /**
     * Flag drawable rotation x change
     */
    DrawableState_RotationZ          = 1 << 17,

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Flag drawable rgb change
     */
    DrawableState_RGB                 = 1 << 18,

    /**
     * Flag drawable opacity change
     */
    DrawableState_Opacity             = 1 << 19,

    /**
     * Flag drawable color change
     */
    DrawableState_Color               = DrawableState_RGB | DrawableState_Opacity,

//----------------------------------------------------------------------------------------------------------------------


    /**
     * Flag drawable position scale rotate translate parent change
     */
    DrawableState_Transform           = DrawableState_Parent    |
                                        DrawableState_PositionX |
                                        DrawableState_PositionY |
                                        DrawableState_PositionZ |
                                        DrawableState_ScaleX    |
                                        DrawableState_ScaleY    |
                                        DrawableState_ScaleZ    |
                                        DrawableState_RotationX |
                                        DrawableState_RotationY |
                                        DrawableState_RotationZ,

    /**
     * Flag drawable transform and color change
     */
    DrawableState_Draw                = DrawableState_Transform |
                                        DrawableState_Color,
}
DrawableState;


typedef struct Drawable Drawable;
struct  Drawable
{
     UserData      userData[1];

     /**
      * Default 0.0f, use openGL coordinate
      */
     float          width;
     float          height;

//----------------------------------------------------------------------------------------------------------------------

     /**
      * If has parent will render use parent modelMatrix
      * parent invisible child will stop rendering
      */
     Drawable*     parent;

     /**
      * Local position, default 0.0f
      */
     float          positionX;
     float          positionY;
     float          positionZ;

     /**
      * Local scale, default 1.0f
      */
     float          scaleX;
     float          scaleY;
     float          scaleZ;

     /**
      * Local rotationZ, default 0.0f clockwise [0.0f - 360.0f]
      */
     float          rotationZ;

     /**
      * Each [0.0f, 1.0f], default (1.0f, 1.0f, 1.0f, 1.0f)
      */
     Color         color        [1];

     /**
      * If set DrawableState_IsBlendColor
      *     set (color) * (parent blendColor)
      * else
      *     equal color
      *
      * default (1.0f, 1.0f, 1.0f, 1.0f)
      */
     Color         blendColor   [1];

//----------------------------------------------------------------------------------------------------------------------

     /**
      * Cached MVP matrix when property changed will update
      */
     Matrix4       mvpMatrix    [1];

     /**
      * Cached model matrix when property changed will update
      */
     Matrix4       modelMatrix  [1];

     /**
      * Cached inverse model matrix
      */
     Matrix4       inverseMatrix[1];

     /**
      * Flag the property changes
      */
     DrawableState state;

//----------------------------------------------------------------------------------------------------------------------

    /**
      * Custom draw called by ADrawable's Draw, not use any openGL command
      */
     void (*Draw)  (Drawable* drawable);

     /**
      * Render with openGL command
      */
     void (*Render)(Drawable* drawable);
};


struct ADrawable
{
    Drawable* (*Create)                ();
    void      (*Init)                  (Drawable* outDrawable);

     /**
      * Transform model matrix and calculate mvp matrix if need
      * Calculate blend color if need
      * Call custom drawable's Draw method
      *
      * if render method implemented will push into render queue for flush openGL command
      */
    void     (*Draw)                  (Drawable* drawable);

    /**
     * Rendering Drawable queue and clear it wait for next frame
     */
    void     (*RenderQueue)           ();

    /**
     * Convert local x in parent coordinate to world x
     * return world x
     */
    float     (*ConvertToWorldX)       (Drawable* localParent, float localX);

    /**
     * Convert local y in parent coordinate to world y
     * return world y
     */
    float     (*ConvertToWorldY)       (Drawable* localParent, float localY);

    /**
     * Convert localV2 in parent coordinate to world coordinate
     */
    void     (*ConvertToWorldV2)      (Drawable* localParent, Vector2* localV2, Vector2* outWorldV2);

    /**
     * Convert world x to local x in parent coordinate
     * return local x
     */
    float     (*ConvertToLocalX)       (Drawable* localParent, float worldX);

    /**
     * Convert world y to local y in parent coordinate
     * return local y
     */
    float     (*ConvertToLocalY)       (Drawable* localParent, float worldY);

    /**
     * Convert worldV2 to local coordinate in parent
     */
    void     (*ConvertToLocalV2)      (Drawable* localParent, Vector2* worldV2, Vector2* outLocalV2);

    /**
     * Convert drawable transform to parent, will change parent and scale position rotationZ for parent coordinate
     * if parent NULL will convert to world coordinate
     */
    void     (*ConvertToParent)       (Drawable* drawable, Drawable* parent);


    /**
     * Convert local x in parentA to parentB
     * return local x in parentB
     */
    float     (*ConvertBetweenLocalX)  (Drawable* parentA, float localXA, Drawable* parentB);

    /**
     * Convert local y in parentA to ParentB
     * return local y in parentB
     */
    float     (*ConvertBetweenLocalY)  (Drawable* parentA, float localYA, Drawable* parentB);


    /**
     * Convert localV2A in parentA to outLocalV2B in parentB
     */
    void     (*ConvertBetweenLocalV2) (Drawable* parentA, Vector2* localV2A, Drawable* parentB, Vector2* outLocalV2B);

    /**
     * If Drawable has flip will transform rotationZ to flipped value
     */
    float     (*GetFlipRotationZ)      (Drawable* drawable, float rotationZ);

    /**
     * Get drawable rotationZ in world
     */
    float     (*GetWorldRotationZ)     (Drawable* drawable);

    /**
     * Get drawable scaleX in world
     */
    float     (*GetWorldScaleX)        (Drawable* drawable);

    /**
     * Get drawable scaleY in world
     */
    float     (*GetWorldScaleY)        (Drawable* drawable);

    /**
     * Get drawable scale vector2 in world
     */
    void     (*GetWorldScale2)        (Drawable* drawable, Vector2* outScale2);
};


extern struct ADrawable ADrawable[1];


//----------------------------------------------------------------------------------------------------------------------


static inline bool ADrawable_CheckState(Drawable* drawable, DrawableState checkState)
{
    return ABitwise_Check(drawable->state, checkState);
}


static inline void ADrawable_AddState(Drawable* drawable, DrawableState addState)
{
    ABitwise_Add(drawable->state, addState);
}


static inline void ADrawable_SetState(Drawable* drawable, DrawableState setState)
{
    ABitwise_Set(drawable->state, setState);
}


static inline void ADrawable_ClearState(Drawable* drawable, DrawableState clearState)
{
    ABitwise_Clear(drawable->state, clearState);
}


static inline void ADrawable_ClearAndAddState(Drawable* drawable, DrawableState clearState, DrawableState addState)
{
    ABitwise_ClearAndAdd(drawable->state, clearState, addState);
}


//----------------------------------------------------------------------------------------------------------------------


static inline void ADrawable_SetVisible(Drawable* drawable)
{
    ADrawable_ClearState(drawable, DrawableState_IsInvisible);
}


static inline void ADrawable_SetInVisible(Drawable* drawable)
{
    ADrawable_AddState(drawable, DrawableState_IsInvisible);
}


static inline bool ADrawable_CheckVisible(Drawable* drawable)
{
    return ADrawable_CheckState(drawable, DrawableState_IsInvisible) == false;
}


//----------------------------------------------------------------------------------------------------------------------


static inline void ADrawable_SetParent(Drawable* drawable, Drawable* parent)
{
    drawable->parent = parent;
    ADrawable_AddState(drawable, DrawableState_Parent);
}


//----------------------------------------------------------------------------------------------------------------------


static inline void ADrawable_SetPositionX(Drawable* drawable, float positionX)
{
    drawable->positionX = positionX;
    ADrawable_AddState(drawable, DrawableState_PositionX);
}


static inline void ADrawable_SetPositionY(Drawable* drawable, float positionY)
{
    drawable->positionY = positionY;
    ADrawable_AddState(drawable, DrawableState_PositionY);
}


static inline void ADrawable_SetPosition2(Drawable* drawable, float positionX, float positionY)
{
    drawable->positionX = positionX;
    drawable->positionY = positionY;
    ADrawable_AddState(drawable, DrawableState_Position2);
}


static inline void ADrawable_SetPositionSame2(Drawable* drawable, float position)
{
    drawable->positionX = position;
    drawable->positionY = position;
    ADrawable_AddState(drawable, DrawableState_Position2);
}


//----------------------------------------------------------------------------------------------------------------------


static inline void ADrawable_SetScaleX(Drawable* drawable, float scaleX)
{
    drawable->scaleX = scaleX;
    ADrawable_AddState(drawable, DrawableState_ScaleX);
}


static inline void ADrawable_SetScaleY(Drawable* drawable, float scaleY)
{
    drawable->scaleY = scaleY;
    ADrawable_AddState(drawable, DrawableState_ScaleY);
}


static inline void ADrawable_SetScale2(Drawable* drawable, float scaleX, float scaleY)
{
    drawable->scaleX = scaleX;
    drawable->scaleY = scaleY;
    ADrawable_AddState(drawable, DrawableState_Scale2);
}


static inline void ADrawable_SetScaleSame2(Drawable* drawable, float scale)
{
    drawable->scaleX = scale;
    drawable->scaleY = scale;
    ADrawable_AddState(drawable, DrawableState_Scale2);
}


//----------------------------------------------------------------------------------------------------------------------


static inline void ADrawable_SetRotationZ(Drawable* drawable, float rotationZ)
{
    drawable->rotationZ = rotationZ;
    ADrawable_AddState(drawable, DrawableState_RotationZ);
}


//----------------------------------------------------------------------------------------------------------------------


static inline void ADrawable_SetRGB(Drawable* drawable, float red, float green, float blue)
{
    drawable->color->r = red;
    drawable->color->g = green;
    drawable->color->b = blue;
    ADrawable_AddState(drawable, DrawableState_RGB);
}


static inline void ADrawable_SetRGBSame(Drawable* drawable, float rgb)
{
    drawable->color->r = rgb;
    drawable->color->g = rgb;
    drawable->color->b = rgb;
    ADrawable_AddState(drawable, DrawableState_RGB);
}


static inline void ADrawable_SetOpacity(Drawable* drawable, float opacity)
{
    drawable->color->a = opacity;
    ADrawable_AddState(drawable, DrawableState_Opacity);
}


static inline void ADrawable_SetRGBA(Drawable* drawable, float red, float green, float blue, float opacity)
{
    drawable->color->r = red;
    drawable->color->g = green;
    drawable->color->b = blue;
    drawable->color->a = opacity;
    ADrawable_AddState(drawable, DrawableState_Color);
}


static inline void ADrawable_SetRGBASame(Drawable* drawable, float rgba)
{
    drawable->color->r = rgba;
    drawable->color->g = rgba;
    drawable->color->b = rgba;
    drawable->color->a = rgba;
    ADrawable_AddState(drawable, DrawableState_Color);
}


static inline void ADrawable_SetColor(Drawable* drawable, Color* color)
{
    *drawable->color = *color;
    ADrawable_AddState(drawable, DrawableState_Color);
}


static inline void ADrawable_SetBlendColor(Drawable* drawable)
{
    ADrawable_AddState(drawable, DrawableState_IsBlendColor);
}


#endif
