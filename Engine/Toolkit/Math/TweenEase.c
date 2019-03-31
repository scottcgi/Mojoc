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
 * Since : 2013-11-5
 * Update: 2019-1-17
 * Author: scott.cgi
 */


#include "Engine/Toolkit/Math/TweenEase.h"
#include "Engine/Toolkit/Math/Math.h"


#define VALUE from + (to - from)  // NOLINT


static float Linear(float from, float to, float time)
{
    return VALUE * time;
}


//----------------------------------------------------------------------------------------------------------------------


static float QuadraticIn(float from, float to, float time)
{
    return VALUE * time * time;
}


static float QuadraticOut(float from, float to, float time)
{
    return VALUE * time * (2.0f - time);
}


static float QuadraticInOut(float from, float to, float time)
{
    if (time < 0.5f)
    {
        return VALUE * time * time * 2.0f;
    }
    else
    {
        return VALUE * (2.0f * time * (2.0f - time) - 1.0f);
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float CubicIn(float from, float to, float time)
{
    return VALUE * time * time * time;
}


static float CubicOut(float from, float to, float time)
{
    time -= 1.0f;
    return VALUE * (time * time * time + 1.0f);
}


static float CubicInOut(float from, float to, float time)
{
    if (time < 0.5f)
    {
        return VALUE * 4.0f * time * time * time;
    }
    else
    {
        time -= 1.0f;
        return VALUE * (4.0f * time * time * time + 1.0f);
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float QuarticIn(float from, float to, float time)
{
    return VALUE * time * time * time * time;
}


static float QuarticOut(float from, float to, float time)
{
    time -= 1.0f;
    return VALUE * (time * time * time * (-time) + 1.0f);
}


static float QuarticInOut(float from, float to, float time)
{
    if (time < 0.5f)
    {
        return VALUE * 8.0f * time * time * time * time;
    }
    else
    {
        time -= 1.0f;
        return VALUE * (-8.0f * time * time * time * time + 1.0f);
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float QuinticIn(float from, float to, float time)
{
    return VALUE * time * time * time * time * time;
}


static float QuinticOut(float from, float to, float time)
{
    time -= 1.0f;
    return VALUE * (time * time * time * time * time + 1.0f);
}


static float QuinticInOut(float from, float to, float time)
{
    if (time < 0.5f)
    {
        return VALUE * 16.0f * time * time * time * time * time;
    }
    else
    {
        time -=  1.0f;
        return VALUE * (16.0f * time * time * time * time * time + 1.0f);
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float SineIn(float from, float to, float time)
{
    return VALUE * (1.0f - cosf(time * MATH_PI2));
}


static float SineOut(float from, float to, float time)
{
    return VALUE * sinf(time * MATH_PI2);
}


static float SineInOut(float from, float to, float time)
{
    return VALUE * 0.5f * (1.0f - cosf(time * MATH_PI));
}


//----------------------------------------------------------------------------------------------------------------------


static float ExponentialIn(float from, float to, float time)
{
    if (time == 0.0f)
    {
        return from;
    }
    else
    {
        return VALUE * powf(2.0f, 10.0f * (time - 1.0f));
    }
}


static float ExponentialOut(float from, float to, float time)
{
    if (time == 1.0f)
    {
        return to;
    }
    else
    {
        return VALUE * (1.0f - powf(2.0f, -10.0f * time));
    }
}


static float ExponentialInOut(float from, float to, float time)
{
    if (time == 0.0f)
    {
        return from;
    }

    if (time == 1.0f)
    {
        return to;
    }

    if (time < 0.5f)
    {
        return VALUE * 0.5f * powf(2.0f, 20.0f * time - 10.0f);
    }
    else
    {
        return VALUE * 0.5f * (2.0f - powf(2.0f, -20.0f * time + 10.0f));
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float CircularIn(float from, float to, float time)
{
    return VALUE * (1.0f - AMath_Sqrtf(1.0f - time * time));
}


static float CircularOut(float from, float to, float time)
{
    return VALUE * AMath_Sqrtf((2.0f - time) * time);
}


static float CircularInOut(float from, float to, float time)
{
    if (time < 0.5f)
    {
        return VALUE * 0.5f * (1.0f - AMath_Sqrtf(1.0f - 4.0f * time * time));
    }
    else
    {
        time = time * 2.0f - 2.0f;
        return VALUE * 0.5f * (AMath_Sqrtf(1.0f - time * time) + 1.0f);
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float ElasticIn(float from, float to, float time)
{
    if (time == 0.0f)
    {
        return from;
    }

    if (time == 1.0f)
    {
        return to;
    }

    return VALUE * -powf(2.0f, 10.0f * time - 10.0f) * sinf((3.33f * time - 3.58f) * MATH_2PI);
}


static float ElasticOut(float from, float to, float time)
{
    if (time == 0.0f)
    {
        return from;
    }

    if (time == 1.0f)
    {
        return to;
    }

    return VALUE * (powf(2.0f, -10.0f * time) * sinf((3.33f * time - 0.25f) * MATH_2PI) + 1.0f);
}


static float ElasticInOut(float from, float to, float time)
{
    if (time == 0.0f)
    {
        return from;
    }

    if (time == 1.0f)
    {
        return to;
    }

    if (time < 0.5f)
    {
        return VALUE * -0.5f * powf(2.0f, 20.0f * time - 10.0f) * sinf((4.45f * time - 2.475f) * MATH_2PI);
    }
    else
    {
        return VALUE * (powf(2.0f, -20.0f * time + 10.0f) * sinf((4.45f * time - 2.475f) * MATH_2PI) * 0.5f + 1.0f);
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float BackIn(float from, float to, float time)
{
    return VALUE * time * time * (2.70158f * time - 1.70158f);
}


static float BackOut(float from, float to, float time)
{
    time -= 1.0f;
    return VALUE * (time * time * (2.70158f * time + 1.70158f) + 1.0f);
}


static float BackInOut(float from, float to, float time)
{
    if (time < 0.5f)
    {
        return VALUE * time * time * (14.379636f * time - 5.189818f);
    }
    else
    {
        time -= 1.0f;
        return VALUE * (time * time * (14.379636f * time + 5.189818f) + 1.0f);
    }
}


static float BackInExponentialOut(float from, float to, float time)
{
    if (time < 0.5f)
    {
        return VALUE * time * time * (14.379636f * time - 5.189818f);
    }
    else
    {
        return VALUE * 0.5f * (2.0f - powf(2.0f, -20.0f * time + 10.0f));
    }
}


static float BackInElasticOut(float from, float to, float time)
{
    if (time < 0.5f)
    {
        return VALUE * time * time * (14.379636f * time - 5.189818f);
    }
    else
    {
        return VALUE * (powf(2.0f, -20.0f * time + 10.0f) * sinf((4.45f * time - 2.475f) * MATH_2PI) * 0.5f + 1.0f);
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float BounceOut(float from, float to, float time)
{
    if (time < 0.363636f)
    {
        return VALUE * 7.5625f * time * time;
    }
    else if (time < 0.72727f)
    {
        time -= 0.545454f;
        return VALUE * (7.5625f * time * time + 0.75f);
    }
    else if (time < 0.909091f)
    {
        time -= 0.818182f;
        return VALUE * (7.5625f * time * time + 0.9375f);
    }
    else
    {
        time -= 0.954545f;
        return VALUE * (7.5625f * time * time + 0.984375f);
    }
}


static float BounceIn(float from, float to, float time)
{
    if (time > 0.636364f)
    {
        time = 1.0f - time;
        return VALUE * (1.0f - 7.5625f * time * time);
    }
    else if (time > 0.27273f)
    {
        time = 0.454546f - time;
        return VALUE * (0.25f - 7.5625f * time * time);
    }
    else if (time > 0.090909f)
    {
        time = 0.181818f - time;
        return VALUE * (0.0625f - 7.5625f * time * time);
    }
    else
    {
        time = 0.045455f - time;
        return VALUE * (0.015625f - 7.5625f * time * time);
    }
}


static float BounceInOut(float from, float to, float time)
{
    if (time < 0.5f)
    {
        // bounce in
        if (time > 0.318182f)
        {
            time = 1.0f - time * 2.0f;
            return VALUE * (0.5f - 3.78125f * time * time);
        }
        else if (time > 0.136365f)
        {
            time = 0.454546f - time * 2.0f;
            return VALUE * (0.125f - 3.78125f * time * time);
        }
        else if (time > 0.045455f)
        {
            time = 0.181818f - time * 2.0f;
            return VALUE * (0.03125f - 3.78125f * time * time);
        }
        else
        {
            time = 0.045455f - time * 2.0f;
            return VALUE * (0.007813f - 3.78125f * time * time);
        }
    }

    // bounce out
    if (time < 0.681818f)
    {
        time = time * 2.0f - 1.0f;
        return VALUE * (3.78125f * time * time + 0.5f);
    }
    else if (time < 0.863635f)
    {
        time = time * 2.0f - 1.545454f;
        return VALUE * (3.78125f * time * time + 0.875f);
    }
    else if (time < 0.954546f)
    {
        time = time * 2.0f - 1.818182f;
        return VALUE * (3.78125f * time * time + 0.96875f);
    }
    else
    {
        time = time * 2.0f - 1.954545f;
        return VALUE * (3.78125f * time * time + 0.992188f);
    }
}


#undef VALUE


struct ATweenEase ATweenEase[1] =
{
    Linear,

    QuadraticIn,
    QuadraticOut,
    QuadraticInOut,

    CubicIn,
    CubicOut,
    CubicInOut,

    QuarticIn,
    QuarticOut,
    QuarticInOut,

    QuinticIn,
    QuinticOut,
    QuinticInOut,

    SineIn,
    SineOut,
    SineInOut,

    ExponentialIn,
    ExponentialOut,
    ExponentialInOut,

    CircularIn,
    CircularOut,
    CircularInOut,

    ElasticIn,
    ElasticOut,
    ElasticInOut,

    BackIn,
    BackOut,
    BackInOut,
    BackInExponentialOut,
    BackInElasticOut,

    BounceIn,
    BounceOut,
    BounceInOut,
};
