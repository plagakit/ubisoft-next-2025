#pragma once

#include "math/vector/vector2.h"
#include "math/vector/vector3.h"

struct CollisionResult2D
{
	bool hit			= false;
	Vec2 contactNormal	= Vec2::ZERO;
	Vec2 restitution	= Vec2::ZERO;
	Vec2 contactPoint	= Vec2::ZERO;
};

struct CollisionResult3D
{
	bool hit			= false;
	Vec3 contactNormal	= Vec3::ZERO;
	Vec3 restitution	= Vec3::ZERO;
	Vec3 contactPoint	= Vec3::ZERO;
};