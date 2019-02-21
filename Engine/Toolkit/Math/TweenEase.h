/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-11-5
 * Author: scott.cgi
 */

#ifndef TWEEN_EASE_H
#define TWEEN_EASE_H


typedef enum
{
    TweenEaseType_Linear,

    TweenEaseType_QuadraticIn,
    TweenEaseType_QuadraticOut,
    TweenEaseType_QuadraticInOut,

    TweenEaseType_CubicIn,
    TweenEaseType_CubicOut,
    TweenEaseType_CubicInOut,

    TweenEaseType_QuarticIn,
    TweenEaseType_QuarticOut,
    TweenEaseType_QuarticInOut,

    TweenEaseType_QuinticIn,
    TweenEaseType_QuinticOut,
    TweenEaseType_QuinticInOut,

    TweenEaseType_SineIn,
    TweenEaseType_SineOut,
    TweenEaseType_SineInOut,

    TweenEaseType_ExponentialIn,
    TweenEaseType_ExponentialOut,
    TweenEaseType_ExponentialInOut,

    TweenEaseType_CircularIn,
    TweenEaseType_CircularOut,
    TweenEaseType_CircularInOut,

    TweenEaseType_ElasticIn,
    TweenEaseType_ElasticOut,
    TweenEaseType_ElasticInOut,

    TweenEaseType_BackIn,
    TweenEaseType_BackOut,
    TweenEaseType_BackInOut,

    TweenEaseType_BounceIn,
    TweenEaseType_BounceOut,
    TweenEaseType_BounceInOut,

    TweenEaseType_Length,
}
TweenEaseType;


struct ATweenEase
{
    /**
     * Ease interpolation algorithm function.
     * value between [from, to] control by time between [0.0f, 1.0f].
     */
    float (*Interpolates[TweenEaseType_Length])(float from, float to, float time);
};


extern struct ATweenEase ATweenEase[1];


#endif
