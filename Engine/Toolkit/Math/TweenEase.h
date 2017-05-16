/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-11-5
 * Author : scott.cgi
 * Version: 0.1
 */


#ifndef TWEEN_EASE_H
#define TWEEN_EASE_H


typedef enum
{
	TweenEaseType_linear,

/*
----------------------------------------------------------------------------------------------------
    Quadratic
----------------------------------------------------------------------------------------------------
*/
	TweenEaseType_QuadIn,
	TweenEaseType_QuadOut,
	TweenEaseType_QuadBoth,

/*
----------------------------------------------------------------------------------------------------
    Cubic
----------------------------------------------------------------------------------------------------
*/
	TweenEaseType_CubicIn,
	TweenEaseType_CubicOut,
	TweenEaseType_CubicBoth,

/*
----------------------------------------------------------------------------------------------------
    Quartic
----------------------------------------------------------------------------------------------------
*/
	TweenEaseType_QuartIn,
	TweenEaseType_QuartOut,
	TweenEaseType_QuartBoth,

/*
----------------------------------------------------------------------------------------------------
    Quintic
----------------------------------------------------------------------------------------------------
*/
	TweenEaseType_QuintIn,
	TweenEaseType_QuintOut,
	TweenEaseType_QuintBoth,

/*
----------------------------------------------------------------------------------------------------
    Sine
----------------------------------------------------------------------------------------------------
*/
	TweenEaseType_SineIn,
	TweenEaseType_SineOut,
	TweenEaseType_SineBoth,

/*
----------------------------------------------------------------------------------------------------
    Exponential
----------------------------------------------------------------------------------------------------
*/
	TweenEaseType_ExpoIn,
	TweenEaseType_ExpoOut,
	TweenEaseType_ExpoBoth,

/*
----------------------------------------------------------------------------------------------------
    circular
----------------------------------------------------------------------------------------------------
*/
	TweenEaseType_CircIn,
	TweenEaseType_CircOut,
	TweenEaseType_CircBoth,


/*
----------------------------------------------------------------------------------------------------
    Elastic
----------------------------------------------------------------------------------------------------
*/
	TweenEaseType_ElasticIn,
	TweenEaseType_ElasticOut,
	TweenEaseType_ElasticBoth,


/*
----------------------------------------------------------------------------------------------------
    Back
----------------------------------------------------------------------------------------------------
*/
	TweenEaseType_BackIn,
	TweenEaseType_BackOut,
	TweenEaseType_BackBoth,

/*
----------------------------------------------------------------------------------------------------
    Bounce
----------------------------------------------------------------------------------------------------
*/
	TweenEaseType_BounceIn,
	TweenEaseType_BounceOut,
	TweenEaseType_BounceBoth,

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
