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
 * Update   : 2019-12-19
 * Author   : scott.cgi
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

    TweenEaseType_ExponentialIn,
    TweenEaseType_ExponentialOut,
    TweenEaseType_ExponentialInOut,

    TweenEaseType_SineIn,
    TweenEaseType_SineOut,
    TweenEaseType_SineInOut,

    TweenEaseType_CircularIn,
    TweenEaseType_CircularOut,
    TweenEaseType_CircularInOut,

    TweenEaseType_ElasticIn,
    TweenEaseType_ElasticOut,
    TweenEaseType_ElasticInOut,

    TweenEaseType_BackIn,
    TweenEaseType_BackOut,
    TweenEaseType_BackInOut,
    TweenEaseType_BackInExponentialOut,
    TweenEaseType_BackInElasticOut,

    TweenEaseType_BounceIn,
    TweenEaseType_BounceOut,
    TweenEaseType_BounceInOut,

    TweenEaseType_Length,
}
TweenEaseType;


struct ATweenEase
{

    /**
     * Tween easing time functions.
     * the return value between [0.0f, 1.0f], which is the time between [0.0f, 1.0f] after easing.
     */
    float (*Easing[TweenEaseType_Length])(float time);
};


extern struct ATweenEase ATweenEase[1];


#endif
