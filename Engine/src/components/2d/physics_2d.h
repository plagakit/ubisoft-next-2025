#pragma once

#include "core/resource/resource.h"
#include "physics/collision_result.h"

struct Physics2D
{


	RID colliderHandle			= INVALID_RID;

	CollisionResult2D lastHit	= {};
};