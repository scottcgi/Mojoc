/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2016-6-17
 * Update   : 2019-1-19
 * Author   : scott.cgi
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
