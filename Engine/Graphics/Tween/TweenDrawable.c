/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2016-6-17
 * Author : scott.cgi
 * Version: 0.1
 */


#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Toolkit/Utils/TweenTool.h"


static float MoveXOnGet(void* target)
{
    return ((Drawable*) target)->positionX;
}


static void MoveXOnSet(void* target, float value)
{
    ADrawable_SetPositionX((Drawable*) target, value);
}


static void MoveYOnSet(void* target, float value)
{
    ADrawable_SetPositionY((Drawable*) target, value);
}


static float MoveYOnGet(void* target)
{
    return ((Drawable*) target)->positionY;
}


static void ScaleXOnSet(void* target, float value)
{
    ADrawable_SetScaleX((Drawable*) target, value);
}


static float ScaleXOnGet(void* target)
{
    return ((Drawable*) target)->scaleX;
}


static void ScaleYOnSet(void* target, float value)
{
    ADrawable_SetScaleY((Drawable*) target, value);
}


static float ScaleYOnGet(void* target)
{
    return ((Drawable*) target)->scaleY;
}


static void RotateZOnSet(void* target, float value)
{
    ADrawable_SetRotationZ((Drawable*) target, value);
}


static float RotateZOnGet(void* target)
{
    return ((Drawable*) target)->rotationZ;
}


static void FadeToOnSet(void* target, float value)
{
    ADrawable_SetOpacity((Drawable*) target, value);
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
