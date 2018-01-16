/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-11-5
 * Author: scott.cgi
 */

#ifndef TWEEN_EASE_H
#define TWEEN_EASE_H


typedef enum
{
    TweenEaseType_Linear,

/*
------------------------------------------------------------------------------------------------------------------------
    Quadratic
------------------------------------------------------------------------------------------------------------------------
*/
    TweenEaseType_QuadraticIn,
    TweenEaseType_QuadraticOut,
    TweenEaseType_QuadraticInOut,

/*
------------------------------------------------------------------------------------------------------------------------
    Cubic
------------------------------------------------------------------------------------------------------------------------
*/
    TweenEaseType_CubicIn,
    TweenEaseType_CubicOut,
    TweenEaseType_CubicInOut,

/*
------------------------------------------------------------------------------------------------------------------------
    Quartic
------------------------------------------------------------------------------------------------------------------------
*/
    TweenEaseType_QuarticIn,
    TweenEaseType_QuarticOut,
    TweenEaseType_QuarticInOut,

/*
------------------------------------------------------------------------------------------------------------------------
    Quintic
------------------------------------------------------------------------------------------------------------------------
*/
    TweenEaseType_QuinticIn,
    TweenEaseType_QuinticOut,
    TweenEaseType_QuinticInOut,

/*
------------------------------------------------------------------------------------------------------------------------
    Sine
------------------------------------------------------------------------------------------------------------------------
*/
    TweenEaseType_SineIn,
    TweenEaseType_SineOut,
    TweenEaseType_SineInOut,

/*
------------------------------------------------------------------------------------------------------------------------
    Exponential
------------------------------------------------------------------------------------------------------------------------
*/
    TweenEaseType_ExponentialIn,
    TweenEaseType_ExponentialOut,
    TweenEaseType_ExponentialInOut,

/*
------------------------------------------------------------------------------------------------------------------------
    Circular
------------------------------------------------------------------------------------------------------------------------
*/
    TweenEaseType_CircularIn,
    TweenEaseType_CircularOut,
    TweenEaseType_CircularInOut,


/*
------------------------------------------------------------------------------------------------------------------------
    Elastic
------------------------------------------------------------------------------------------------------------------------
*/
    TweenEaseType_ElasticIn,
    TweenEaseType_ElasticOut,
    TweenEaseType_ElasticInOut,


/*
------------------------------------------------------------------------------------------------------------------------
    Back
------------------------------------------------------------------------------------------------------------------------
*/
    TweenEaseType_BackIn,
    TweenEaseType_BackOut,
    TweenEaseType_BackInOut,

/*
------------------------------------------------------------------------------------------------------------------------
    Bounce
------------------------------------------------------------------------------------------------------------------------
*/
    TweenEaseType_BounceIn,
    TweenEaseType_BounceOut,
    TweenEaseType_BounceInOut,

    TweenEaseType_Length,
}
TweenEaseType;


struct ATweenEase
{
    /**
     * Ease interpolation algorithm function
     * value between [from, to] control by time between [0.0f, 1.0f]
     */
    float (*Interpolates[TweenEaseType_Length])(float from, float to, float time);
};


extern struct ATweenEase ATweenEase[1];


#endif
