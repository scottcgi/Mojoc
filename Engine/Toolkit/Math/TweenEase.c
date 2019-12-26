/*
 * The original algorithms of easing functions come from Robert Penner.
 * The open source licensed under the MIT License and the BSD License.
 *
 * Introduce: http://robertpenner.com/easing
 * License  : http://robertpenner.com/easing_terms_of_use.html
 *
 *----------------------------------------------------------------------------------------------------------------------
 * 
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2013-11-5
 * Update   : 2019-1-17
 * Author   : scott.cgi
 */


#include "Engine/Toolkit/Math/TweenEase.h"
#include "Engine/Toolkit/Math/Math.h"


static float Linear(float time)
{
    return time;
}


//----------------------------------------------------------------------------------------------------------------------


static float QuadraticIn(float time)
{
    return time * time;
}


static float QuadraticOut(float time)
{
    return time * (2.0f - time);
}


static float QuadraticInOut(float time)
{
    if (time < 0.5f)
    {
        return time * time * 2.0f;
    }

    return 2.0f * time * (2.0f - time) - 1.0f;
}


//----------------------------------------------------------------------------------------------------------------------


static float CubicIn(float time)
{
    return time * time * time;
}


static float CubicOut(float time)
{
    time -= 1.0f;
    return time * time * time + 1.0f;
}


static float CubicInOut(float time)
{
    if (time < 0.5f)
    {
        return 4.0f * time * time * time;
    }

    time -= 1.0f;
    return 4.0f * time * time * time + 1.0f;
}


//----------------------------------------------------------------------------------------------------------------------


static float QuarticIn(float time)
{
    return time * time * time * time;
}


static float QuarticOut(float time)
{
    time -= 1.0f;
    return 1.0f - time * time * time * time;
}


static float QuarticInOut(float time)
{
    if (time < 0.5f)
    {
        return 8.0f * time * time * time * time;
    }

    time -= 1.0f;
    return 1.0f - 8.0f * time * time * time * time;
}


//----------------------------------------------------------------------------------------------------------------------


static float QuinticIn(float time)
{
    return time * time * time * time * time;
}


static float QuinticOut(float time)
{
    time -= 1.0f;
    return time * time * time * time * time + 1.0f;
}


static float QuinticInOut(float time)
{
    if (time < 0.5f)
    {
        return 16.0f * time * time * time * time * time;
    }

    time -= 1.0f;
    return 16.0f * time * time * time * time * time + 1.0f;
}


//----------------------------------------------------------------------------------------------------------------------


static float ExponentialIn(float time)
{
    if (time <= 0.0f)
    {
        return time;
    }

    return powf(2.0f, 10.0f * (time - 1.0f));
}


static float ExponentialOut(float time)
{
    if (time >= 1.0f)
    {
        return time;
    }

    return 1.0f - powf(2.0f, -10.0f * time);
}


static float ExponentialInOut(float time)
{
    if (time <= 0.0f || time >= 1.0f)
    {
        return time;
    }

    if (time < 0.5f)
    {
        return 0.5f * powf(2.0f, 20.0f * time - 10.0f);
    }

    return 0.5f * (2.0f - powf(2.0f, -20.0f * time + 10.0f));
}


//----------------------------------------------------------------------------------------------------------------------


static float SineIn(float time)
{
    return 1.0f - cosf(time * MATH_PI2);
}


static float SineOut(float time)
{
    return sinf(time * MATH_PI2);
}


static float SineInOut(float time)
{
    return 0.5f * (1.0f - cosf(time * MATH_PI));
}


//----------------------------------------------------------------------------------------------------------------------


static float CircularIn(float time)
{
    return 1.0f - AMath_Sqrtf(1.0f - time * time);
}


static float CircularOut(float time)
{
    return AMath_Sqrtf((2.0f - time) * time);
}


static float CircularInOut(float time)
{
    if (time < 0.5f)
    {
        return 0.5f * (1.0f - AMath_Sqrtf(1.0f - 4.0f * time * time));
    }

    time = time * 2.0f - 2.0f;
    return 0.5f * (AMath_Sqrtf(1.0f - time * time) + 1.0f);
}


//----------------------------------------------------------------------------------------------------------------------


static float ElasticIn(float time)
{
    if (time <= 0.0f || time >= 1.0f)
    {
        return time;
    }

    return -powf(2.0f, 10.0f * time - 10.0f) * sinf(20.923007f * time - 22.493803f);
}


static float ElasticOut(float time)
{
    if (time <= 0.0f || time >= 1.0f)
    {
        return time;
    }
    
    return powf(2.0f, -10.0f * time) * sinf(20.923007f * time - 1.570796f) + 1.0f;
}


static float ElasticInOut(float time)
{
    if (time <= 0.0f || time >= 1.0f)
    {
        return time;
    }

    if (time < 0.5f)
    {
        return -0.5f * powf(2.0f, 20.0f * time - 10.0f) * sinf(27.960175f * time - 15.550884f);
    }

    return powf(2.0f, -20.0f * time + 10.0f) * sinf(27.960175f * time - 15.550884f) * 0.5f + 1.0f;
}


//----------------------------------------------------------------------------------------------------------------------


static float BackIn(float time)
{
    return time * time * (2.70158f * time - 1.70158f);
}


static float BackOut(float time)
{
    time -= 1.0f;
    return time * time * (2.70158f * time + 1.70158f) + 1.0f;
}


static float BackInOut(float time)
{
    if (time < 0.5f)
    {
        return time * time * (14.379636f * time - 5.189818f);
    }

    time -= 1.0f;
    return time * time * (14.379636f * time + 5.189818f) + 1.0f;
}


static float BackInExponentialOut(float time)
{
    if (time < 0.5f)
    {
        return time * time * (14.379636f * time - 5.189818f);
    }

    return 0.5f * (2.0f - powf(2.0f, -20.0f * time + 10.0f));
}


static float BackInElasticOut(float time)
{
    if (time < 0.5f)
    {
        return time * time * (14.379636f * time - 5.189818f);
    }

    return powf(2.0f, -20.0f * time + 10.0f) * sinf(27.960175f * time - 15.550884f) * 0.5f + 1.0f;
}


//----------------------------------------------------------------------------------------------------------------------


static float BounceIn(float time)
{
    if (time > 0.636364f)
    {
        time = 1.0f - time;
        return 1.0f - 7.5625f * time * time;
    }

    if (time > 0.27273f)
    {
        time = 0.454546f - time;
        return 0.25f - 7.5625f * time * time;
    }

    if (time > 0.090909f)
    {
        time = 0.181818f - time;
        return 0.0625f - 7.5625f * time * time;
    }

    if (time >= 1.0f)
    {
        return time;
    }

    time = 0.045455f - time;
    return 0.015625f - 7.5625f * time * time;
}


static float BounceOut(float time)
{
    if (time < 0.363636f)
    {
        return 7.5625f * time * time;
    }

    if (time < 0.72727f)
    {
        time -= 0.545454f;
        return 7.5625f * time * time + 0.75f;
    }

    if (time < 0.909091f)
    {
        time -= 0.818182f;
        return 7.5625f * time * time + 0.9375f;
    }

    if (time >= 1.0f)
    {
        return time;
    }
    
    time -= 0.954545f;
    return 7.5625f * time * time + 0.984375f;
}


static float BounceInOut(float time)
{
    if (time < 0.5f)
    {
        // bounce in
        if (time > 0.318182f)
        {
            time = 1.0f - time * 2.0f;
            return 0.5f - 3.78125f * time * time;
        }

        if (time > 0.136365f)
        {
            time = 0.454546f - time * 2.0f;
            return 0.125f - 3.78125f * time * time;
        }

        if (time > 0.045455f)
        {
            time = 0.181818f - time * 2.0f;
            return 0.03125f - 3.78125f * time * time;
        }

        time = 0.045455f - time * 2.0f;
        return 0.007813f - 3.78125f * time * time;
    }

    // bounce out
    if (time < 0.681818f)
    {
        time = time * 2.0f - 1.0f;
        return 3.78125f * time * time + 0.5f;
    }

    if (time < 0.863635f)
    {
        time = time * 2.0f - 1.545454f;
        return 3.78125f * time * time + 0.875f;
    }

    if (time < 0.954546f)
    {
        time = time * 2.0f - 1.818182f;
        return 3.78125f * time * time + 0.96875f;
    }

    if (time >= 1.0f)
    {
        return time;
    }

    time = time * 2.0f - 1.954545f;
    return 3.78125f * time * time + 0.992188f;
}


struct ATweenEase ATweenEase[1] =
{{
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

    ExponentialIn,
    ExponentialOut,
    ExponentialInOut,

    SineIn,
    SineOut,
    SineInOut,

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
}};
