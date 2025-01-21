#pragma once

#include "math/shape/plane.h"

struct Frustum
{
	Plane top;
	Plane bottom;
	Plane right;
	Plane left;
	Plane near;
	Plane far;
};