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
 * Since    : 2013-1-2
 * Update   : 2019-1-24
 * Author   : scott.cgi
 */


#ifndef DRAWABLE_H
#define DRAWABLE_H


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
 * If contains 'Is' the state can set add and clear,
 * else the state will automatically set add and clear.
 */
typedef enum
{
    DrawableState_Null                = 0,
    
    /**
     * Whether drawable is invisible.
     */
    DrawableState_IsInvisible         = 1,

    /**
     * Whether drawable mvp matrix need to update.
     */
    DrawableState_IsUpdateMVPMatrix   = 1 << 1,

    /**
     * Whether drawable calculate blendColor by parent.
     */
    DrawableState_IsBlendColor        = 1 << 2,

    /**
     * Flag drawable inverse matrix need to update.
     */
    DrawableState_UpdateInverseMatrix = 1 << 3,

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Flag drawable transform has changed.
     */
    DrawableState_TransformChanged    = 1 << 4,

    /**
     * Flag drawable rgb has changed.
     */
    DrawableState_RGBChanged          = 1 << 5,

    /**
     * Flag drawable opacity has changed.
     */
    DrawableState_OpacityChanged      = 1 << 6,

    /**
     * Flag drawable has been drawn.
     */
    DrawableState_DrawChanged         = 1 << 7,

    /**
     * Flag drawable color has changed
     */
    DrawableState_ColorChanged        = DrawableState_RGBChanged |
                                        DrawableState_OpacityChanged,

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Flag drawable parent has changed.
     */
    DrawableState_Parent              = 1 << 8,

    /**
     * Flag drawable position x has changed.
     */
    DrawableState_PositionX           = 1 << 9,

    /**
     * Flag drawable position y has changed.
     */
    DrawableState_PositionY           = 1 << 10,

    /**
     * Flag drawable position z has changed.
     */
    DrawableState_PositionZ           = 1 << 11,

    /**
     * Flag drawable position x and y have changed.
     */
    DrawableState_Position2           = DrawableState_PositionX |
                                        DrawableState_PositionY,

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Flag drawable scale x has changed.
     */
    DrawableState_ScaleX             = 1 << 12, 

    /**
     * Flag drawable scale y has changed.
     */
    DrawableState_ScaleY             = 1 << 13,

    /**
     * Flag drawable scale z has changed.
     */
    DrawableState_ScaleZ             = 1 << 14,

    /**
     * Flag drawable scale x and y have changed.
     */
    DrawableState_Scale2             = DrawableState_ScaleX | DrawableState_ScaleY,

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Flag drawable rotation x has changed.
     */
    DrawableState_RotationX          = 1 << 15,

    /**
     * Flag drawable rotation x has changed.
     */
    DrawableState_RotationY          = 1 << 16,

    /**
     * Flag drawable rotation x has changed.
     */
    DrawableState_RotationZ          = 1 << 17,

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Flag drawable rgb has changed.
     */
    DrawableState_RGB                 = 1 << 18,

    /**
     * Flag drawable opacity has changed.
     */
    DrawableState_Opacity             = 1 << 19,

    /**
     * Flag drawable color has changed.
     */
    DrawableState_Color               = DrawableState_RGB | DrawableState_Opacity,

//----------------------------------------------------------------------------------------------------------------------

    /**
     * Flag drawable position scale rotate translate parent have changed.
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
     * Flag drawable transform and color have changed.
     */
    DrawableState_Draw                = DrawableState_Transform |
                                        DrawableState_Color,
}
DrawableState;


typedef struct Drawable Drawable;

/**
 * If object can be drawn then must inherit Drawable.
 * the drawable provides transform, color, state, matrix properties,
 * but render function must be implemented by subclass.
 */
struct  Drawable
{
     UserData      userData[1];

     /* Default 0.0f, use openGL coordinate. */
     float         width;
     float         height;

//----------------------------------------------------------------------------------------------------------------------

     /**
      * If has parent will render use parent modelMatrix,
      * and parent invisible that children will stop rendering.
      */
     Drawable*     parent;

     /* The local position relative parent, default 0.0f. */
     float         positionX;
     float         positionY;
     float         positionZ;

     /* The local scale relative parent, default 1.0f. */
     float         scaleX;
     float         scaleY;
     float         scaleZ;

     /**
      * The local rotation relative parent, default 0.0f and clockwise [0.0f - 360.0f].
      */
     float         rotationX;
     float         rotationY;
     float         rotationZ;

     /**
      * The color value each between [0.0f, 1.0f], default {1.0f, 1.0f, 1.0f, 1.0f}.
      */
     Color         color        [1];

     /**
      * If set DrawableState_IsBlendColor
      *     equal (color) * (parent blendColor)
      * else
      *     equal color
      *
      * default {1.0f, 1.0f, 1.0f, 1.0f}
      */
     Color         blendColor   [1];

//----------------------------------------------------------------------------------------------------------------------

     /**
      * Cached MVP matrix when property has changed will update.
      */
     Matrix4       mvpMatrix    [1];

     /**
      * Cached model matrix when property has changed will update.
      */
     Matrix4       modelMatrix  [1];

     /**
      * Cached inverse model matrix。
      */
     Matrix4       inverseMatrix[1];

     /**
      * Flag the property changes.
      */
     DrawableState state;

//----------------------------------------------------------------------------------------------------------------------

    /**
      * Custom draw for preparing rendering data.
      * called by ADrawable's Draw, do not use any openGL command,
      * and can check any state change.
      *
      * if called in Scheduler or Coroutine may have draw order problem,
      * because the draw oder common controlled by Component.
      */
     void (*Draw)  (Drawable* drawable);

     /**
      * Render with openGL command.
      */
     void (*Render)(Drawable* drawable);
};


/**
 * Control Drawable and coordinate conversion.
 */
struct ADrawable
{
    Drawable* (*Create)                       (void);
    void      (*Init)                         (Drawable* outDrawable);

     /**
      * Transform model matrix by position scale rotation,
      * and calculate mvp matrix and blend color if needed,
      * and call drawable's Draw method.
      *
      * if render method implemented will push Drawable into render queue for flush openGL command.
      */
    void      (*Draw)                         (Drawable* drawable);

    /**
     * Call Render function of Drawable in the render queue and clear it for next frame.
     * the drawable will push into render queue when it's Draw function called.
     */
    void      (*Render)                       (void);

    /**
     * Get render queue size.
     */
    int       (*GetDrawCalls)                 (void);

    /**
     * Convert localPositionX in localParent to world coordinate.
     * return world position x.
     */
    float     (*ConvertToWorldPositionX)      (Drawable* localParent, float localPositionX);

    /**
     * Convert localPositionY in localParent to world coordinate.
     * return world position y.
     */
    float     (*ConvertToWorldPositionY)      (Drawable* localParent, float localPositionY);

    /**
     * Convert localPositionV2 in localParent to world coordinate.
     */
    void      (*ConvertToWorldPositionV2)     (
                                                   Drawable* localParent,
                                                   Vector2*  localPositionV2,
                                                   Vector2*  outWorldPositionV2
                                              );

    /**
     * Convert worldPositionX to localParent coordinate.
     * return local position x
     */
    float    (*ConvertToLocalPositionX)       (Drawable* localParent, float worldPositionX);

    /**
     * Convert worldPositionY to localParent coordinate.
     * return local position y.
     */
    float    (*ConvertToLocalPositionY)       (Drawable* localParent, float worldPositionY);

    /**
     * Convert worldPositionV2 to localParent coordinate.
     */
    void     (*ConvertToLocalPositionV2)      (
                                                  Drawable* localParent,
                                                  Vector2*  worldPositionV2,
                                                  Vector2*  outLocalPositionV2
                                              );

    /**
     * Set drawable parent and keep world transform.
     * if parent NULL transform will convert to world coordinate.
     */
    void     (*ConvertToParent)               (Drawable* drawable, Drawable* parent);


    /**
     * Convert localPositionX in parentA to parentB coordinate.
     * return local position x in parentB.
     */
    float    (*ConvertBetweenLocalPositionX)  (Drawable* parentA, float localPositionX, Drawable* parentB);

    /**
     * Convert localPositionY in parentA to ParentB coordinate.
     * return local position y in parentB.
     */
    float    (*ConvertBetweenLocalPositionY)  (Drawable* parentA, float localPositionY, Drawable* parentB);


    /**
     * Convert localPositionV2 in parentA to outLocalPositionV2 in parentB.
     */
    void     (*ConvertBetweenLocalPositionV2) (
                                                  Drawable* parentA,
                                                  Vector2*  localPositionV2,
                                                  Drawable* parentB,
                                                  Vector2*  outLocalPositionV2
                                              );

    /**
     * If Drawable has flip will transform rotationZ to flipped value.
     */
    float    (*GetFlipRotationZ)              (Drawable* drawable, float rotationZ);

    /**
     * Get drawable rotationZ in world coordinate.
     */
    float    (*GetWorldRotationZ)             (Drawable* drawable);

    /**
     * Get drawable scaleX in world coordinate.
     */
    float    (*GetWorldScaleX)                (Drawable* drawable);

    /**
     * Get drawable scaleY in world coordinate.
     */
    float    (*GetWorldScaleY)                (Drawable* drawable);

    /**
     * Get drawable scale Vector2 in world coordinate.
     */
    void     (*GetWorldScaleV2)               (Drawable* drawable, Vector2* outScaleV2);

    /**
     * Get drawable position x in world coordinate.
     */
    float    (*GetWorldPositionX)             (Drawable* drawable);

    /**
     * Get drawable position x in world coordinate.
     */
    float    (*GetWorldPositionY)             (Drawable* drawable);

    /**
     * Get drawable position Vector2 in world coordinate.
     */
    void     (*GetWorldPositionV2)            (Drawable* drawable, Vector2* outPositionV2);

    /**
     * Get drawable position Vector3 in world coordinate.
     */
    void     (*GetWorldPositionV3)            (Drawable* drawable, Vector3* outPositionV3);
};


extern struct ADrawable ADrawable[1];


//----------------------------------------------------------------------------------------------------------------------


/**
 * Check drawable whether has state.
 */
static inline bool ADrawable_CheckState(Drawable* drawable, DrawableState state)
{
    return ABitwise_Check(drawable->state, state);
}


/**
 * Add state to drawable.
 */
static inline void ADrawable_AddState(Drawable* drawable, DrawableState state)
{
    ABitwise_Add(drawable->state, state);
}


/**
 * Set state to drawable.
 */
static inline void ADrawable_SetState(Drawable* drawable, DrawableState state)
{
    ABitwise_Set(drawable->state, state);
}


/**
 * Clear drawable state.
 */
static inline void ADrawable_ClearState(Drawable* drawable, DrawableState state)
{
    ABitwise_Clear(drawable->state, state);
}


/**
 * Clear drawable clearState and add addState.
 */
static inline void ADrawable_ClearAndAddState(Drawable* drawable, DrawableState clearState, DrawableState addState)
{
    ABitwise_ClearAndAdd(drawable->state, clearState, addState);
}


//----------------------------------------------------------------------------------------------------------------------


/**
 * Make drawable visible.
 */
static inline void ADrawable_SetVisible(Drawable* drawable)
{
    ADrawable_ClearState(drawable, DrawableState_IsInvisible);
}


/**
 * Make drawable invisible.
 */
static inline void ADrawable_SetInvisible(Drawable* drawable)
{
    ADrawable_AddState(drawable, DrawableState_IsInvisible);
}


/**
 * Check drawable whether is visible.
 */
static inline bool ADrawable_CheckVisible(Drawable* drawable)
{
    return ADrawable_CheckState(drawable, DrawableState_IsInvisible) == false;
}


//----------------------------------------------------------------------------------------------------------------------


/**
 * Set drawable parent, current transform will become local in new parent.
 * use ADrawable->ConvertToParent can keep world transform stay.
 */
static inline void ADrawable_SetParent(Drawable* drawable, Drawable* parent)
{
    drawable->parent = parent;
    ADrawable_AddState(drawable, DrawableState_Parent);
}


//----------------------------------------------------------------------------------------------------------------------


/**
 * Set drawable position x, and flag state for Drawable->Draw to change modelMatrix.
 */
static inline void ADrawable_SetPositionX(Drawable* drawable, float positionX)
{
    drawable->positionX = positionX;
    ADrawable_AddState(drawable, DrawableState_PositionX);
}


/**
 * Set drawable position y, and flag state for Drawable->Draw to change modelMatrix.
 */
static inline void ADrawable_SetPositionY(Drawable* drawable, float positionY)
{
    drawable->positionY = positionY;
    ADrawable_AddState(drawable, DrawableState_PositionY);
}

/**
* Set drawable position x y, and flag state for Drawable->Draw to change modelMatrix.
*/
static inline void ADrawable_SetPosition2(Drawable* drawable, float positionX, float positionY)
{
    drawable->positionX = positionX;
    drawable->positionY = positionY;
    ADrawable_AddState(drawable, DrawableState_Position2);
}


/**
* Set drawable position same x y, and flag state for Drawable->Draw to change modelMatrix.
*/
static inline void ADrawable_SetPositionSame2(Drawable* drawable, float position)
{
    drawable->positionX = position;
    drawable->positionY = position;
    ADrawable_AddState(drawable, DrawableState_Position2);
}


//----------------------------------------------------------------------------------------------------------------------


/**
 * Set drawable scale x, and flag state for Drawable->Draw to change modelMatrix.
 */
static inline void ADrawable_SetScaleX(Drawable* drawable, float scaleX)
{
    drawable->scaleX = scaleX;
    ADrawable_AddState(drawable, DrawableState_ScaleX);
}


/**
 * Set drawable scale y, and flag state for Drawable->Draw to change modelMatrix.
 */
static inline void ADrawable_SetScaleY(Drawable* drawable, float scaleY)
{
    drawable->scaleY = scaleY;
    ADrawable_AddState(drawable, DrawableState_ScaleY);
}


/**
 * Set drawable scale x y, and flag state for Drawable->Draw to change modelMatrix.
 */
static inline void ADrawable_SetScale2(Drawable* drawable, float scaleX, float scaleY)
{
    drawable->scaleX = scaleX;
    drawable->scaleY = scaleY;
    ADrawable_AddState(drawable, DrawableState_Scale2);
}


/**
 * Set drawable scale same x y, and flag state for Drawable->Draw to change modelMatrix.
 */
static inline void ADrawable_SetScaleSame2(Drawable* drawable, float scale)
{
    drawable->scaleX = scale;
    drawable->scaleY = scale;
    ADrawable_AddState(drawable, DrawableState_Scale2);
}


//----------------------------------------------------------------------------------------------------------------------


/**
 * Set drawable rotation x, and flag state for Drawable->Draw to change modelMatrix.
 */
static inline void ADrawable_SetRotationX(Drawable* drawable, float rotationX)
{
    drawable->rotationX = rotationX;
    ADrawable_AddState(drawable, DrawableState_RotationX);
}


/**
 * Set drawable rotation y, and flag state for Drawable->Draw to change modelMatrix.
 */
static inline void ADrawable_SetRotationY(Drawable* drawable, float rotationY)
{
    drawable->rotationY = rotationY;
    ADrawable_AddState(drawable, DrawableState_RotationY);
}


/**
 * Set drawable rotation z, and flag state for Drawable->Draw to change modelMatrix.
 */
static inline void ADrawable_SetRotationZ(Drawable* drawable, float rotationZ)
{
    drawable->rotationZ = rotationZ;
    ADrawable_AddState(drawable, DrawableState_RotationZ);
}


//----------------------------------------------------------------------------------------------------------------------


/**
 * Set drawable rgb, and flag state for Drawable->Draw to change blendColor.
 */
static inline void ADrawable_SetRGB(Drawable* drawable, float red, float green, float blue)
{
    drawable->color->r = red;
    drawable->color->g = green;
    drawable->color->b = blue;
    ADrawable_AddState(drawable, DrawableState_RGB);
}


/**
 * Set drawable same rgb, and flag state for Drawable->Draw to change blendColor.
 */
static inline void ADrawable_SetRGBSame(Drawable* drawable, float rgb)
{
    drawable->color->r = rgb;
    drawable->color->g = rgb;
    drawable->color->b = rgb;
    ADrawable_AddState(drawable, DrawableState_RGB);
}


/**
 * Set drawable opacity, and flag state for Drawable->Draw to change blendColor.
 */
static inline void ADrawable_SetOpacity(Drawable* drawable, float opacity)
{
    drawable->color->a = opacity;
    ADrawable_AddState(drawable, DrawableState_Opacity);
}


/**
 * Set drawable rgba, and flag state for Drawable->Draw to change blendColor.
 */
static inline void ADrawable_SetRGBA(Drawable* drawable, float red, float green, float blue, float opacity)
{
    drawable->color->r = red;
    drawable->color->g = green;
    drawable->color->b = blue;
    drawable->color->a = opacity;
    ADrawable_AddState(drawable, DrawableState_Color);
}


/**
 * Set drawable same rgba, and flag state for Drawable->Draw to change blendColor.
 */
static inline void ADrawable_SetRGBASame(Drawable* drawable, float rgba)
{
    drawable->color->r = rgba;
    drawable->color->g = rgba;
    drawable->color->b = rgba;
    drawable->color->a = rgba;
    ADrawable_AddState(drawable, DrawableState_Color);
}


/**
 * Set drawable color, and flag state for Drawable->Draw to change blendColor.
 */
static inline void ADrawable_SetColor(Drawable* drawable, Color* color)
{
    *drawable->color = *color;
    ADrawable_AddState(drawable, DrawableState_Color);
}


/**
 * Make drawable color blend parent blendColor.
 */
static inline void ADrawable_SetBlendColor(Drawable* drawable)
{
    ADrawable_AddState(drawable, DrawableState_IsBlendColor);
}


#endif
