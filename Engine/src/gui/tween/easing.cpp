#include "pch.h"
#include "easing.h"

#include <cmath>
#include "math/math_utils.h"

// All of the math for these functions
// have been gotten from https://easings.net/

float Easing::Discrete(float t)
{
	return std::floor(t);
}

float Easing::Linear(float t)
{
	return t;
}

float Easing::CubicIn(float t)
{
	return t * t * t;
}

float Easing::CubicOut(float t)
{
	float it = 1.0f - t;
	return 1.0f - it * it * it;
}

float Easing::CubicInOut(float t)
{
	return t < 0.5
		? 4.0f * t * t * t
		: 1.0f - std::powf(-2.0f * t + 2.0f, 3) * 0.5f;
}

float Easing::ElasticIn(float t)
{
	constexpr float c4 = TWO_PI / 3.0f;
	return std::abs(t) < EPSILON ? 0.0f // == 0
		: std::abs(t - 1.0f) < EPSILON ? 1.0f // == 1
		: -std::powf(2.0f, 10.0f * t - 10.0f) * sinf((t * 10.0f - 10.75f) * c4);
}

float Easing::ElasticOut(float t)
{
	constexpr float c4 = TWO_PI / 3.0f;
	return std::abs(t) < EPSILON ? 0.0f 
		: std::abs(t - 1.0f) < EPSILON ? 1.0f
		: -std::powf(2.0f, -10.0f * t) * sinf((t * 10.0f - 0.75f) * c4) + 1.0f;
}

float Easing::ElasticInOut(float t)
{
	constexpr float c5 = TWO_PI / 4.5f;
	return std::abs(t) < EPSILON ? 0.0f
		: std::abs(t - 1.0f) < EPSILON ? 1.0f
		: t < 0.5f
		? -(std::powf(2, 20 * t - 10) * sinf((20 * t - 11.125f) * c5)) * 0.5f
		: (std::powf(2, -20 * t + 10) * sinf((20 * t - 11.125f) * c5)) * 0.5f + 1;
}
