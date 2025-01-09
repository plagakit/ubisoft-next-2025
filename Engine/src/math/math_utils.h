#pragma once

#include "math/vector/vector2.h"
#include "math/vector/vector3.h"
#include "math/vector/vector4.h"

//constexpr float PI = 3.141592653589793f;
//constexpr float TWO_PI = 2 * PI;
//constexpr float HALF_PI = PI * 0.5f;

namespace Math
{
	template <typename T>
	T Lerp(T min, T max, float t)
	{
		return min + t * (max - min);
	}

	template <typename T>
	float InvLerp(T min, T max, T val)
	{
		return (val - min) / (max - min);
	}

	template <typename T>
	T Map(T min0, T max0, T min1, T max1, T val)
	{
		float t = InvLerp(min0, max0, val);
		return Lerp(min1, max1, t);
	}

	float RandAngle();
	Vec2 RandDirection();

}