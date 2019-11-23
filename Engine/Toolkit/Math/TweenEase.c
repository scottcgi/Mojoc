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
 * Since    : 2013-11-5
 * Update   : 2019-1-17
 * Author   : scott.cgi
 */


#include "Engine/Toolkit/Math/TweenEase.h"
#include "Engine/Toolkit/Math/Math.h"


#define VALUE from + (to - from)


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


//----------------------------------------------------------------------------------------------------------------------


static float LinearTime(float time)
{
    return time;
}


//----------------------------------------------------------------------------------------------------------------------


static float QuadraticInTime(float time)
{
    return time * time;
}


static float QuadraticOutTime(float time)
{
    return time * (2.0f - time);
}


static float QuadraticInOutTime(float time)
{
    if (time < 0.5f)
    {
        return time * time * 2.0f;
    }
    else
    {
        return 2.0f * time * (2.0f - time) - 1.0f;
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float CubicInTime(float time)
{
    return time * time * time;
}


static float CubicOutTime(float time)
{
    time -= 1.0f;
    return time * time * time + 1.0f;
}


static float CubicInOutTime(float time)
{
    if (time < 0.5f)
    {
        return 4.0f * time * time * time;
    }
    else
    {
        time -= 1.0f;
        return 4.0f * time * time * time + 1.0f;
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float QuarticInTime(float time)
{
    return time * time * time * time;
}


static float QuarticOutTime(float time)
{
    time -= 1.0f;
    return time * time * time * (-time) + 1.0f;
}


static float QuarticInOutTime(float time)
{
    if (time < 0.5f)
    {
        return 8.0f * time * time * time * time;
    }
    else
    {
        time -= 1.0f;
        return -8.0f * time * time * time * time + 1.0f;
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float QuinticInTime(float time)
{
    return time * time * time * time * time;
}


static float QuinticOutTime(float time)
{
    time -= 1.0f;
    return time * time * time * time * time + 1.0f;
}


static float QuinticInOutTime(float time)
{
    if (time < 0.5f)
    {
        return 16.0f * time * time * time * time * time;
    }
    else
    {
        time -=  1.0f;
        return 16.0f * time * time * time * time * time + 1.0f;
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float ExponentialInTime(float time)
{
    if (time == 0.0f)
    {
        return time;
    }
    else
    {
        return powf(2.0f, 10.0f * (time - 1.0f));
    }
}


static float ExponentialOutTime(float time)
{
    if (time == 1.0f)
    {
        return time;
    }
    else
    {
        return 1.0f - powf(2.0f, -10.0f * time);
    }
}


static float ExponentialInOutTime(float time)
{
    if (time == 0.0f || time == 1.0f)
    {
        return time;
    }

    if (time < 0.5f)
    {
        return 0.5f * powf(2.0f, 20.0f * time - 10.0f);
    }
    else
    {
        return 0.5f * (2.0f - powf(2.0f, -20.0f * time + 10.0f));
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float SineInTime(float time)
{
    return 1.0f - cosf(time * MATH_PI2);
}


static float SineOutTime(float time)
{
    return sinf(time * MATH_PI2);
}


static float SineInOutTime(float time)
{
    return 0.5f * (1.0f - cosf(time * MATH_PI));
}


//----------------------------------------------------------------------------------------------------------------------


static float CircularInTime(float time)
{
    return 1.0f - AMath_Sqrtf(1.0f - time * time);
}


static float CircularOutTime(float time)
{
    return AMath_Sqrtf((2.0f - time) * time);
}


static float CircularInOutTime(float time)
{
    if (time < 0.5f)
    {
        return 0.5f * (1.0f - AMath_Sqrtf(1.0f - 4.0f * time * time));
    }
    else
    {
        time = time * 2.0f - 2.0f;
        return 0.5f * (AMath_Sqrtf(1.0f - time * time) + 1.0f);
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float ElasticInTime(float time)
{
    if (time == 0.0f || time == 1.0f)
    {
        return time;
    }

    return -powf(2.0f, 10.0f * time - 10.0f) * sinf((3.33f * time - 3.58f) * MATH_2PI);
}


static float ElasticOutTime(float time)
{
    if (time == 0.0f || time == 1.0f)
    {
        return time;
    }
    
    return powf(2.0f, -10.0f * time) * sinf((3.33f * time - 0.25f) * MATH_2PI) + 1.0f;
}


static float ElasticInOutTime(float time)
{
    if (time == 0.0f || time == 1.0f)
    {
        return time;
    }

    if (time < 0.5f)
    {
        return -0.5f * powf(2.0f, 20.0f * time - 10.0f) * sinf((4.45f * time - 2.475f) * MATH_2PI);
    }
    else
    {
        return powf(2.0f, -20.0f * time + 10.0f) * sinf((4.45f * time - 2.475f) * MATH_2PI) * 0.5f + 1.0f;
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float BackInTime(float time)
{
    return time * time * (2.70158f * time - 1.70158f);
}


static float BackOutTime(float time)
{
    time -= 1.0f;
    return time * time * (2.70158f * time + 1.70158f) + 1.0f;
}


static float BackInOutTime(float time)
{
    if (time < 0.5f)
    {
        return time * time * (14.379636f * time - 5.189818f);
    }
    else
    {
        time -= 1.0f;
        return time * time * (14.379636f * time + 5.189818f) + 1.0f;
    }
}


static float BackInExponentialOutTime(float time)
{
    if (time < 0.5f)
    {
        return time * time * (14.379636f * time - 5.189818f);
    }
    else
    {
        return 0.5f * (2.0f - powf(2.0f, -20.0f * time + 10.0f));
    }
}


static float BackInElasticOutTime(float time)
{
    if (time < 0.5f)
    {
        return time * time * (14.379636f * time - 5.189818f);
    }
    else
    {
        return powf(2.0f, -20.0f * time + 10.0f) * sinf((4.45f * time - 2.475f) * MATH_2PI) * 0.5f + 1.0f;
    }
}


//----------------------------------------------------------------------------------------------------------------------


static float BounceInTime(float time)
{
    if (time > 0.636364f)
    {
        time = 1.0f - time;
        return 1.0f - 7.5625f * time * time;
    }
    else if (time > 0.27273f)
    {
        time = 0.454546f - time;
        return 0.25f - 7.5625f * time * time;
    }
    else if (time > 0.090909f)
    {
        time = 0.181818f - time;
        return 0.0625f - 7.5625f * time * time;
    }
    else
    {
        time = 0.045455f - time;
        return 0.015625f - 7.5625f * time * time;
    }
}


static float BounceOutTime(float time)
{
    if (time < 0.363636f)
    {
        return 7.5625f * time * time;
    }
    else if (time < 0.72727f)
    {
        time -= 0.545454f;
        return 7.5625f * time * time + 0.75f;
    }
    else if (time < 0.909091f)
    {
        time -= 0.818182f;
        return 7.5625f * time * time + 0.9375f;
    }
    else
    {
        time -= 0.954545f;
        return 7.5625f * time * time + 0.984375f;
    }
}


static float BounceInOutTime(float time)
{
    if (time < 0.5f)
    {
        // bounce in
        if (time > 0.318182f)
        {
            time = 1.0f - time * 2.0f;
            return 0.5f - 3.78125f * time * time;
        }
        else if (time > 0.136365f)
        {
            time = 0.454546f - time * 2.0f;
            return 0.125f - 3.78125f * time * time;
        }
        else if (time > 0.045455f)
        {
            time = 0.181818f - time * 2.0f;
            return 0.03125f - 3.78125f * time * time;
        }
        else
        {
            time = 0.045455f - time * 2.0f;
            return 0.007813f - 3.78125f * time * time;
        }
    }

    // bounce out
    if (time < 0.681818f)
    {
        time = time * 2.0f - 1.0f;
        return 3.78125f * time * time + 0.5f;
    }
    else if (time < 0.863635f)
    {
        time = time * 2.0f - 1.545454f;
        return 3.78125f * time * time + 0.875f;
    }
    else if (time < 0.954546f)
    {
        time = time * 2.0f - 1.818182f;
        return 3.78125f * time * time + 0.96875f;
    }
    else
    {
        time = time * 2.0f - 1.954545f;
        return 3.78125f * time * time + 0.992188f;
    }
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

//----------------------------------------------------------------------------------------------------------------------

     LinearTime,

     QuadraticInTime,
     QuadraticOutTime,
     QuadraticInOutTime,

     CubicInTime,
     CubicOutTime,
     CubicInOutTime,

     QuarticInTime,
     QuarticOutTime,
     QuarticInOutTime,

     QuinticInTime,
     QuinticOutTime,
     QuinticInOutTime,

     ExponentialInTime,
     ExponentialOutTime,
     ExponentialInOutTime,

     SineInTime,
     SineOutTime,
     SineInOutTime,

     CircularInTime,
     CircularOutTime,
     CircularInOutTime,

     ElasticInTime,
     ElasticOutTime,
     ElasticInOutTime,

     BackInTime,
     BackOutTime,
     BackInOutTime,
     BackInExponentialOutTime,
     BackInElasticOutTime,

     BounceInTime,
     BounceOutTime,
     BounceInOutTime,
}};
