#pragma once

#include "physics/collision_result.h"
#include "physics/collision_data.h"

struct CircleCollider;
struct Ray2DCollider;

class CollisionSolver
{
public:
	static CollisionResult2D Solve(const CircleCollider& c1, const CircleCollider& c2, const CollisionData2D& data);
};