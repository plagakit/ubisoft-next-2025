#pragma once

#include "math/vector/vector2.h"
#include "math/vector/vector3.h"
#include "math/vector/vector4.h"
#include "math/quaternion/quat.h"
#include "core/app_settings.h"

//constexpr inline float PI = 3.141592653589793f;
constexpr inline float TWO_PI{ 2 * PI };
constexpr inline float HALF_PI{ PI * 0.5f };
constexpr inline float DEG2RAD{ PI / 180.0f };
constexpr inline float RAD2DEG{ 180.0f / PI };
constexpr inline float EPSILON{ 0.00001f };

namespace Math
{
	template <typename T>
	T Lerp(T min, T max, float t)
	{
		return min + (max - min) * t;
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

	template <typename T>
	T Clamp(T min, T max, T val)
	{
		return val <= min ? min
			: val >= max ? max
			: val;
	}

	template<typename T>
	Vector2<T> Min(const Vector2<T>& a, const Vector2<T>& b)
	{
		return a.x < b.x || a.y < b.y ? a : b;
	}

	template<typename T>
	Vector2<T> Max(const Vector2<T>& a, const Vector2<T>& b)
	{
		return a.x < b.x || a.y < b.y ? b : a;
	}

	int Sign(float t);

	Quat Slerp(Quat start, Quat end, float t);

	float RandAngle();
	Vec2 RandDirection();
	int RandInt(int min, int max);

}