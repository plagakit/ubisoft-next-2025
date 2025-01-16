#pragma once

#include "math/vector/vector2.h"

struct CollisionResult2D
{
	bool hit			= false;
	Vec2 contactNormal	= Vec2::ZERO;
	Vec2 contactPoint	= Vec2::ZERO;
};