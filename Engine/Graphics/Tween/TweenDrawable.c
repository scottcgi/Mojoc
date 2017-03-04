/*
 * TweenDrawable.c
 *
 *  Created on: 2016-6-17
 *      Author: scott.cgi
 */

#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Toolkit/Utils/TweenTool.h"


static float MoveXOnGet(void* target)
{
	return ((Drawable*) target)->positionX;
}


static void MoveXOnSet(void* target, float value)
{
	ADrawableSetPositionX((Drawable*) target, value);
}


static void MoveYOnSet(void* target, float value)
{
	ADrawableSetPositionY((Drawable*) target, value);
}


static float MoveYOnGet(void* target)
{
	return ((Drawable*) target)->positionY;
}


static void ScaleXOnSet(void* target, float value)
{
	ADrawableSetScaleX((Drawable*) target, value);
}


static float ScaleXOnGet(void* target)
{
	return ((Drawable*) target)->scaleX;
}


static void ScaleYOnSet(void* target, float value)
{
	ADrawableSetScaleY((Drawable*) target, value);
}

static float ScaleYOnGet(void* target)
{
	return ((Drawable*) target)->scaleY;
}


static void RotateZOnSet(void* target, float value)
{
	ADrawableSetRotationZ((Drawable*) target, value);
}


static float RotateZOnGet(void* target)
{
	return ((Drawable*) target)->rotationZ;
}


static void FadeToOnSet(void* target, float value)
{
	ADrawableSetOpacity((Drawable*) target, value);
}


static float FadeToOnGet(void* target)
{
	return ((Drawable*) target)->color->a;
}


struct ATweenActionValueGetSetImpl ATweenActionValueGetSetImpl[1] =
{{
	{
		MoveXOnGet,
		MoveXOnSet,
	},

	{
		MoveYOnGet,
		MoveYOnSet,
	},

	{
		ScaleXOnGet,
		ScaleXOnSet,
	},

	{
		ScaleYOnGet,
		ScaleYOnSet,
	},

	{
		RotateZOnGet,
		RotateZOnSet,
	},

	{
		FadeToOnGet,
		FadeToOnSet,
	},
}};
