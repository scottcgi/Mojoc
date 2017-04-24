/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since :  2013-1-2
 * Author: scott.cgi
 */

#ifndef drawable_h
#define drawable_h


#include <stdbool.h>
#include "Engine/Graphics/OpenGL/Platform/gl3.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"
#include "Engine/Toolkit/Head/Bitwise.h"
#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Graphics/Draw/Color.h"
#include "Engine/Toolkit/Math/Vector.h"
#include "Engine/Toolkit/Head/UserData.h"


/**
 * If contains 'is' the state can set and clear
 * else the state will automatically set and clear
 */
typedef enum
{
	drawable_state_null                = 0,

	/**
	 * Whether drawable is invisible
	 */
	drawable_state_is_invisible        = 1,

	/**
	 * Whether drawable mvp matrix need update
	 */
	drawable_state_is_update_mvp       = 1 << 1,

	/**
	 * Whether drawable calculate blendColor by parent
	 */
	drawable_state_is_blend_color      = 1 << 2,

//--------------------------------------------------------------------------------------------------

	/**
	 * Flag drawable inverse matrix need update
	 */
	drawable_state_update_inverse      = 1 << 3,

//--------------------------------------------------------------------------------------------------

	/**
	 * Flag drawable transform has changed
	 */
	drawable_state_transform_ed        = 1 << 4,

	/**
	 * Flag drawable rgb has changed
	 */
	drawable_state_rgb_ed              = 1 << 5,

	/**
	 * Flag drawable opacity has changed
	 */
	drawable_state_opacity_ed          = 1 << 6,

	/**
	 * Flag drawable has been drawn
	 */
	drawable_state_draw_ed             = 1 << 7,

	/**
	 * Flag drawable color has changed
	 */
	drawable_state_color_ed            = drawable_state_rgb_ed  | drawable_state_opacity_ed,

//--------------------------------------------------------------------------------------------------

	/**
	 * Flag drawable parent change
	 */
	drawable_state_parent              = 1 << 8,

//--------------------------------------------------------------------------------------------------

	/**
	 * Flag drawable position x change
	 */
	drawable_state_position_x          = 1 << 9,

	/**
	 * Flag drawable position y change
	 */
	drawable_state_position_y          = 1 << 10,

	/**
	 * Flag drawable position z change
	 */
	drawable_state_position_z          = 1 << 11,

	/**
	 * Flag drawable position x and y change
	 */
	drawable_state_position2           = drawable_state_position_x | drawable_state_position_y,

//--------------------------------------------------------------------------------------------------

	/**
	 * Flag drawable scale x change
	 */
	drawable_state_scale_x             = 1 << 12,

	/**
	 * Flag drawable scale y change
	 */
	drawable_state_scale_y             = 1 << 13,

	/**
	 * Flag drawable scale z change
	 */
	drawable_state_scale_z             = 1 << 14,

	/**
	 * Flag drawable scale x and y change
	 */
	drawable_state_scale2              = drawable_state_scale_x | drawable_state_scale_y,

//--------------------------------------------------------------------------------------------------

	/**
	 * Flag drawable rotation x change
	 */
	drawable_state_rotation_x          = 1 << 15,

	/**
	 * Flag drawable rotation x change
	 */
	drawable_state_rotation_y          = 1 << 16,

	/**
	 * Flag drawable rotation x change
	 */
	drawable_state_rotation_z          = 1 << 17,

//--------------------------------------------------------------------------------------------------

	/**
	 * Flag drawable rgb change
	 */
	drawable_state_rgb                 = 1 << 18,

	/**
	 * Flag drawable opacity change
	 */
	drawable_state_opacity             = 1 << 19,

	/**
	 * Flag drawable color change
	 */
	drawable_state_color               = drawable_state_rgb | drawable_state_opacity,

//--------------------------------------------------------------------------------------------------


	/**
	 * Flag drawable position scale rotate translate parent change
	 */
	drawable_state_transform           = drawable_state_parent     |
			  	  	  	  	  	  	  	 drawable_state_position_x |
										 drawable_state_position_y |
										 drawable_state_position_z |
										 drawable_state_scale_x    |
										 drawable_state_scale_y    |
										 drawable_state_scale_z    |
										 drawable_state_rotation_x |
										 drawable_state_rotation_y |
										 drawable_state_rotation_z,

	/**
	 * Flag drawable state change
	 */
	drawable_state_change              = drawable_state_transform |
								         drawable_state_color,
}
DrawableState;



typedef struct Drawable Drawable;
struct  Drawable
{
     UserData  userData[1];

	 /**
	  * Default 0.0f, use openGL coordinate
	  */
	 float     width;
	 float     height;

//--------------------------------------------------------------------------------------------------

	 /**
	  * If has parent will render use parent modelMatrix
	  * parent invisible child will stop rendering
	  */
	 Drawable* parent;

	 /**
	  * Default 0.0f
	  */
	 float     positionX;
	 float     positionY;
	 float     positionZ;

	 /**
	  * Default 1.0f
	  */
	 float     scaleX;
	 float     scaleY;
	 float     scaleZ;

	 /**
	  * Default0.0f Clockwise [0.0f - 360.0f]
	  */
	 float     rotationZ;

	 /**
	  * Each [0.0f, 1.0f], default (1.0f, 1.0f, 1.0f, 1.0f)
	  */
	 Color     color        [1];

	 /**
	  * If set drawable_state_is_blend_color
	  * 	set (color) * (parent blendColor)
	  * else
	  *     equal color
	  *
	  * default (1.0f, 1.0f, 1.0f, 1.0f)
	  */
	 Color     blendColor   [1];

//--------------------------------------------------------------------------------------------------

	 /**
	  * Cached MVP matrix when property changed will update
	  */
	 Matrix4   mvpMatrix    [1];

	 /**
	  * Cached model matrix when property changed will update
	  */
	 Matrix4   modelMatrix  [1];

	 /**
	  * Cached inverse model matrix
	  */
	 Matrix4   inverseMatrix[1];

	 /**
	  * Identifier property has changed
	  */
	 int       state;

//--------------------------------------------------------------------------------------------------

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
    void      (*Draw)                  (Drawable* drawable);

    /**
     * Rendering Drawable queue and clear it wait for next frame
     */
    void      (*RenderQueue)           ();

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
	 * Convert localPoint in parent coordinate to world coordinate
	 */
	void      (*ConvertToWorldPoint)   (Drawable* localParent, Vector2* localPoint, Vector2* outWorldPoint);

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
	 * Convert worldPoint to localPoint in parent coordinate
	 */
	void      (*ConvertToLocalPoint)   (Drawable* localParent, Vector2* worldPoint, Vector2* outLocalPoint);

	/**
	 * Convert drawable transform to parent, will change parent and scale position rotationZ for parent coordinate
	 * if parent NULL will convert to world coordinate
	 */
	void      (*ConvertToParent)       (Drawable* drawable, Drawable* parent);


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
	 * Convert localPointA in parentA to localPointB in parentB
	 */
	void      (*ConvertBetweenLocal)   (Drawable* parentA, Vector2* localPointA, Drawable* parentB, Vector2* outLocalPointB);


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
};

extern struct ADrawable ADrawable[1];


//--------------------------------------------------------------------------------------------------


static inline bool ADrawableCheckState(Drawable* drawable, DrawableState checkState)
{
	return ABitwiseCheck(drawable->state, checkState);
}


static inline void ADrawableSetState(Drawable* drawable, DrawableState setState)
{
	ABitwiseSet(drawable->state, setState);
}


static inline void ADrawableSetOnlyState(Drawable* drawable, DrawableState setOnlyState)
{
	ABitwiseSetOnly(drawable->state, setOnlyState);
}


static inline void ADrawableClearState(Drawable* drawable, DrawableState clearState)
{
	ABitwiseClear(drawable->state, clearState);
}


static inline void ADrawableClearAndSetState(Drawable* drawable, DrawableState clearState, DrawableState setState)
{
	ABitwiseClearAndSet(drawable->state, clearState, setState);
}


//--------------------------------------------------------------------------------------------------


static inline void ADrawableSetVisible(Drawable* drawable)
{
	ADrawableClearState(drawable, drawable_state_is_invisible);
}


static inline void ADrawableSetInVisible(Drawable* drawable)
{
	ADrawableSetState(drawable, drawable_state_is_invisible);
}


static inline bool ADrawableCheckVisible(Drawable* drawable)
{
	return ADrawableCheckState(drawable, drawable_state_is_invisible) == false;
}


//--------------------------------------------------------------------------------------------------


static inline void ADrawableSetParent(Drawable* drawable, Drawable* parent)
{
	drawable->parent = parent;
	ADrawableSetState(drawable, drawable_state_parent);
}


//--------------------------------------------------------------------------------------------------


static inline void ADrawableSetPositionX(Drawable* drawable, float positionX)
{
	drawable->positionX = positionX;
	ADrawableSetState(drawable, drawable_state_position_x);
}


static inline void ADrawableSetPositionY(Drawable* drawable, float positionY)
{
	drawable->positionY = positionY;
	ADrawableSetState(drawable, drawable_state_position_y);
}


static inline void ADrawableSetPosition2(Drawable* drawable, float positionX, float positionY)
{
	drawable->positionX = positionX;
	drawable->positionY = positionY;
	ADrawableSetState(drawable, drawable_state_position2);
}


static inline void ADrawableSetPositionSame2(Drawable* drawable, float position)
{
    drawable->positionX = position;
    drawable->positionY = position;
    ADrawableSetState(drawable, drawable_state_position2);
}


/*
----------------------------------------------------------------------------------------------------
    All property set just make dirty flag
    so the model matrix will apply changes until nearest frame draw called
----------------------------------------------------------------------------------------------------
*/


static inline void ADrawableSetScaleX(Drawable* drawable, float scaleX)
{
	drawable->scaleX = scaleX;
	ADrawableSetState(drawable, drawable_state_scale_x);
}


static inline void ADrawableSetScaleY(Drawable* drawable, float scaleY)
{
	drawable->scaleY = scaleY;
	ADrawableSetState(drawable, drawable_state_scale_y);
}


static inline void ADrawableSetScale2(Drawable* drawable, float scaleX, float scaleY)
{
	drawable->scaleX = scaleX;
	drawable->scaleY = scaleY;
	ADrawableSetState(drawable, drawable_state_scale2);
}


static inline void ADrawableSetScaleSame2(Drawable* drawable, float scale)
{
	drawable->scaleX = scale;
	drawable->scaleY = scale;
	ADrawableSetState(drawable, drawable_state_scale2);
}


//--------------------------------------------------------------------------------------------------


static inline void ADrawableSetRotationZ(Drawable* drawable, float rotationZ)
{
	drawable->rotationZ = rotationZ;
	ADrawableSetState(drawable, drawable_state_rotation_z);
}


//--------------------------------------------------------------------------------------------------


static inline void ADrawableSetRGB(Drawable* drawable, float red, float green, float blue)
{
	drawable->color->r = red;
	drawable->color->g = green;
	drawable->color->b = blue;
	ADrawableSetState(drawable, drawable_state_rgb);
}


static inline void ADrawableSetRGBSame(Drawable* drawable, float rgb)
{
    drawable->color->r = rgb;
    drawable->color->g = rgb;
    drawable->color->b = rgb;
    ADrawableSetState(drawable, drawable_state_rgb);
}


static inline void ADrawableSetOpacity(Drawable* drawable, float opacity)
{
	drawable->color->a = opacity;
	ADrawableSetState(drawable, drawable_state_opacity);
}


static inline void ADrawableSetRGBA(Drawable* drawable, float red, float green, float blue, float opacity)
{
	drawable->color->r = red;
	drawable->color->g = green;
	drawable->color->b = blue;
	drawable->color->a = opacity;
	ADrawableSetState(drawable, drawable_state_color);
}


static inline void ADrawableSetRGBASame(Drawable* drawable, float rgba)
{
    drawable->color->r = rgba;
    drawable->color->g = rgba;
    drawable->color->b = rgba;
    drawable->color->a = rgba;
    ADrawableSetState(drawable, drawable_state_color);
}


static inline void ADrawableSetColor(Drawable* drawable, Color* color)
{
	*drawable->color = *color;
	ADrawableSetState(drawable, drawable_state_color);
}


static inline void ADrawableSetBlendColor(Drawable* drawable)
{
	ADrawableSetState(drawable, drawable_state_is_blend_color);
}


#endif
