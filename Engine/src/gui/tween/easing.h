#pragma once

#include <array>

using EasingFunction = float (*)(float);

constexpr int EASING_MODES = 4;
enum EasingMode
{
	DISCRETE,
	LINEAR,
	CUBIC,
	ELASTIC
};

constexpr int EASING_TYPES = 3;
enum EasingType
{
	IN,
	OUT,
	IN_OUT
};

// Here we make a compile type map from mode-type
// pairs to their corresponding functions
namespace Easing
{
	float Discrete(float t);
	float Linear(float t);

	float CubicIn(float t);
	float CubicOut(float t);
	float CubicInOut(float t);

	float ElasticIn(float t);
	float ElasticOut(float t);
	float ElasticInOut(float t);

	constexpr int PAIRS = EASING_TYPES * EASING_MODES;
	constexpr std::array<EasingFunction, PAIRS> g_easingMap =
	{
		&Discrete, &Discrete, &Discrete,
		&Linear, &Linear, &Linear,
		&CubicIn, &CubicOut, &CubicInOut,
		&ElasticIn, &ElasticOut, &ElasticInOut
	};

	constexpr EasingFunction GetFunc(EasingMode mode, EasingType type)
	{
		return g_easingMap[static_cast<int>(mode) * 3 + static_cast<int>(type)];
	}
}