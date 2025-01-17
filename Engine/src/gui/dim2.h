#pragma once

#include "math/vector/vector2.h"

struct Dim2
{
	Vec2 relative; // Relative to parent GUI, usually ranges [0, 1]
	Vec2 offset;   // Absolute pixel offset

	Dim2(float rx, float ry, float ox, float oy) :
		relative(rx, ry), offset(ox, oy)
	{};
};