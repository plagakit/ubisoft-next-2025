#pragma once

#include "math/vector/vector2.h"

struct Transform2D
{
	Vec2 position		= Vec2::ZERO;
	float rotation		= 0.0f;
	float scale			= 1.0f;

	Vec2 velocity		= Vec2::ZERO;
	float angVelocity	= 0.0f;

	Vec2 acceleration	= Vec2::ZERO;
};