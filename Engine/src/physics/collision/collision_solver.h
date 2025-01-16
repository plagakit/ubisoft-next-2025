#pragma once

#include "physics/collision/collision_result.h"
#include "physics/collision/collision_data.h"

struct CircleCollider;
struct Ray2D;

class CollisionSolver
{
public:
	static CollisionResult2D Solve(const CircleCollider& c1, const CircleCollider& c2, const CollisionData2D& data);

	//static RayCast2D Solve(const CircleCollider& c, const Ray2D& ray, const CollisionData2D& data);
};