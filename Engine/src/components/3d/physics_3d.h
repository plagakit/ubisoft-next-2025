#pragma once

#include "core/resource/resource.h"
#include "physics/collision/collision_result.h"

struct Physics3D
{
	RID colliderHandle = INVALID_RID;
	bool isImmovable = false;
	bool isTrigger = false;
	CollisionResult3D lastHit = {};
};