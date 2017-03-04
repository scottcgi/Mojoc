/*
 *
 *  Created on: 2013-11-5
 *      Author: scott.cgi
 */

#include "Engine/Toolkit/Math/TweenEase.h"
#include "Engine/Toolkit/Math/Math.h"



#define value from + (to - from)


static float Linear(float from, float to, float time)
{
	return value * time;
}


/** Quadratic */
static float QuadIn(float from, float to, float time)
{
	return value * time * time;
}

static float QuadOut(float from, float to, float time)
{
	return value * time * (2.0f - time);
}

static float QuadBoth(float from, float to, float time)
{
	if (time < 0.5f)
	{
		return value * time * time * 2.0f;
	}
	else
	{
		return value * (2.0f * time * (2.0f - time) - 1.0f);
	}
}


/** Cubic */
static float CubicIn(float from, float to, float time)
{
	return value * time * time * time;
}

static float CubicOut(float from, float to, float time)
{
	time -= 1.0f;
	return value * (time * time * time + 1.0f);
}

static float CubicBoth(float from, float to, float time)
{
	if (time < 0.5f)
	{
		return value * 4.0f * time * time * time;
	}
	else
	{
		time -= 1.0f;
		return value * (4.0f * time * time * time + 1.0f);
	}
}



/** Quartic */
static float QuartIn(float from, float to, float time)
{
	return value * time * time * time * time;
}

static float QuartOut(float from, float to, float time)
{
	time -= 1.0f;
	return value * (time * time * time * (-time) + 1.0f);
}

static float QuartBoth(float from, float to, float time)
{
	if (time < 0.5f)
	{
		return value * 8.0f * time * time * time * time;
	}
	else
	{
		time -= 1.0f;
		return value * (-8.0f * time * time * time * time + 1.0f);
	}
}



/** Quintic */
static float QuintIn(float from, float to, float time)
{
	return value * time * time * time * time * time;
}

static float QuintOut(float from, float to, float time)
{
	time -= 1.0f;
	return value * (time * time * time * time * time + 1.0f);
}

static float QuintBoth(float from, float to, float time)
{
	if (time < 0.5f)
	{
		return value * 16.0f * time * time * time * time * time;
	}
	else
	{
		time -=  1.0f;
		return value * (16.0f * time * time * time * time * time + 1.0f);
	}
}



/** Sine */
static float SineIn(float from, float to, float time)
{
	return value * (1.0f - cosf(time * math_pi2));
}

static float SineOut(float from, float to, float time)
{
	return value * sinf(time * math_pi2);
}

static float SineBoth(float from, float to, float time)
{
	return value * 0.5f * (1.0f - cosf(time * math_pi));
}


/* exponential */
static float ExpoIn(float from, float to, float time)
{
	if (time == 0.0f)
	{
		return from;
	}
	else
	{
		return value * powf(2.0f, 10.0f * (time - 1.0f));
	}
}

static float ExpoOut(float from, float to, float time)
{
	if (time == 1.0f)
	{
		return to;
	}
	else
	{
		return value * (1.0f - powf(2.0f, -10.0f * time));
	}
}

static float ExpoBoth(float from, float to, float time)
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
		return value * 0.5f * powf(2.0f, 20.0f * time - 10.0f);
	}
	else
	{
		return value * 0.5f * (2.0f - powf(2.0f, -20.0f * time + 10.0f));
	}
}


/** Circular */
static float CircIn(float from, float to, float time)
{
	return value * (1.0f - sqrtf(1.0f - time * time));
}

static float CircOut(float from, float to, float time)
{
	return value * sqrtf((2.0f - time) * time);
}

static float CircBoth(float from, float to, float time)
{
	if (time < 0.5f)
	{
		return value * 0.5f * (1.0f - sqrtf(1.0f - 4.0 * time * time));
	}
	else
	{
		time = time * 2.0f - 2.0f;
		return value * 0.5f * (sqrtf(1.0f - time * time) + 1);
	}
}

/** Elastic */
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

	return value * -powf(2.0f, 10.0f * time - 10.0f) * sinf((3.33f * time - 3.58f) * math_2pi);
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

	return value * (powf(2.0f, -10.0f * time) * sinf((3.33f * time - 0.25f) * math_2pi) + 1.0f);
}

static float ElasticBoth(float from, float to, float time)
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
		return value * -0.5f * powf(2.0f, 20.0f * time - 10.0f) * sinf((4.45f * time - 2.475f) * math_2pi);
	}
	else
	{
		return value * (powf(2.0f, -20.0f * time + 10.0f) * sinf((4.45f * time - 2.475f) * math_2pi) * 0.5f + 1.0f);
	}
}



/** Back */
static float BackIn(float from, float to, float time)
{
	return value * time * time * (2.70158f * time - 1.70158f);
}

static float BackOut(float from, float to, float time)
{
	time -= 1.0f;
	return value * (time * time * (2.70158f * time + 1.70158f) + 1.0f);
}

static float BackBoth(float from, float to, float time)
{
	if (time < 0.5f)
	{
		return value *  time * time * (14.379636f * time - 5.189818f);
	}
	else
	{
		time -= 1.0f;
		return value * (time * time * (14.379636f * time + 5.189818f) + 1.0f);
	}
}

/** Bounce */
static float BounceOut(float from, float to, float time)
{
	if (time < 0.363636f)
	{
		return value * 7.5625f * time * time;
	}
	else if (time < 0.72727f)
	{
		time -= 0.545454f;
		return value * (7.5625f * time * time + 0.75f);
	}
	else if (time < 0.909091f)
	{
		time -= 0.818182f;
		return value * (7.5625f * time * time + 0.9375f);
	}
	else
	{
		time -= 0.954545f;
		return value * (7.5625f * time * time + 0.984375f);
	}
}

static float BounceIn(float from, float to, float time)
{
	if (time > 0.636364f)
	{
		time = 1.0f - time;
		return value * (1.0f - 7.5625f * time * time);
	}
	else if (time > 0.27273f)
	{
		time = 0.454546f - time;
		return value * (0.25f - 7.5625f * time * time);
	}
	else if (time > 0.090909f)
	{
		time = 0.181818f - time;
		return value * (0.0625f - 7.5625f * time * time);
	}
	else
	{
		time = 0.045455f - time;
		return value * (0.015625f - 7.5625f * time * time);
	}
}

static float BounceBoth(float from, float to, float time)
{
	if (time < 0.5f)
	{
		// bounce in
		if (time > 0.318182f)
		{
			time = 1.0f - time * 2.0f;
			return value * (0.5f - 3.78125f * time * time);
		}
		else if (time > 0.136365f)
		{
			time = 0.454546f - time * 2.0f;
			return value * (0.125f - 3.78125f * time * time);
		}
		else if (time > 0.045455f)
		{
			time = 0.181818f - time * 2.0f;
			return value * (0.03125f - 3.78125f * time * time);
		}
		else
		{
			time = 0.045455f - time * 2.0f;
			return value * (0.007813f - 3.78125f * time * time);
		}
	}

	// bounce out
	if (time < 0.681818f)
	{
		time = time * 2.0f - 1.0f;
		return value * (3.78125f * time * time + 0.5f);
	}
	else if (time < 0.863635f)
	{
		time = time * 2.0f - 1.545454f;
		return value * (3.78125f * time * time + 0.875f);
	}
	else if (time < 0.954546f)
	{
		time = time * 2.0f - 1.818182f;
		return value * (3.78125f * time * time + 0.96875f);
	}
	else
	{
		time = time * 2.0f - 1.954545f;
		return value * (3.78125f * time * time + 0.992188f);
	}
}

#undef value



struct ATweenEase ATweenEase[1] =
{
	Linear,

	QuadIn,
	QuadOut,
	QuadBoth,

	CubicIn,
	CubicOut,
	CubicBoth,

	QuartIn,
	QuartOut,
	QuartBoth,

	QuintIn,
	QuintOut,
	QuintBoth,

	SineIn,
	SineOut,
	SineBoth,

	ExpoIn,
	ExpoOut,
	ExpoBoth,

	CircIn,
	CircOut,
	CircBoth,

	ElasticIn,
	ElasticOut,
	ElasticBoth,

	BackIn,
	BackOut,
	BackBoth,

	BounceIn,
	BounceOut,
	BounceBoth,
};
