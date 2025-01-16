#pragma once

#include "math/vector/vector2.h"
#include "core/resource/resource.h"
#include "entity/entity.h"

struct Ray2D
{
	Vec2 origin		= Vec2::ZERO;
	Vec2 direction	= Vec2::ZERO;
};

struct RayCast2D
{
	bool hit		= false;
	Vec2 hitPos		= Vec2::ZERO;
	Vec2 hitNorm	= Vec2::ZERO;
	float t			= 0.0f; // progress along line
	RID hitCollider	= INVALID_RID;
	Entity hitID	= NULL_ENTITY;
};