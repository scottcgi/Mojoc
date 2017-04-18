/*
 * Copyright (C) scott.cgi All Rights Reserved.
 *
 * Since  : 2013-11-5
 * Author : scott.cgi
 */

#ifndef tween_ease_h
#define tween_ease_h


typedef enum
{
	tween_ease_linear,

	/* quadratic */
	tween_ease_quad_in,
	tween_ease_quad_out,
	tween_ease_quad_both,

	/* cubic */
	tween_ease_cubic_in,
	tween_ease_cubic_out,
	tween_ease_cubic_both,

	/* quartic */
	tween_ease_quart_in,
	tween_ease_quart_out,
	tween_ease_quart_both,

	/* quintic */
	tween_ease_quint_in,
	tween_ease_quint_out,
	tween_ease_quint_both,

	/* sine */
	tween_ease_sine_in,
	tween_ease_sine_out,
	tween_ease_sine_both,

	/* exponential */
	tween_ease_expo_in,
	tween_ease_expo_out,
	tween_ease_expo_both,

	/* circular */
	tween_ease_circ_in,
	tween_ease_circ_out,
	tween_ease_circ_both,

	/* elastic */
	tween_ease_elastic_in,
	tween_ease_elastic_out,
	tween_ease_elastic_both,

	/* back */
	tween_ease_back_in,
	tween_ease_back_out,
	tween_ease_back_both,

	/* bounce */
	tween_ease_bounce_in,
	tween_ease_bounce_out,
	tween_ease_bounce_both,

	tween_ease_type_length,
}
TweenEaseType;


struct ATweenEase
{
	/**
	 * Ease interpolation algorithm function
	 * value between [from, to] control by time between [0.0f, 1.0f]
	 */
	float (*Interpolates[tween_ease_type_length])(float from, float to, float time);
};


extern struct ATweenEase ATweenEase[1];


#endif
