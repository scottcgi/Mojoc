/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The project Mojoc is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2016-6-17
 * Update: 2019-1-19
 * Author: scott.cgi
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
