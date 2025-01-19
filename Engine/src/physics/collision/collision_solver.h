#pragma once

#include "physics/collision/collision_result.h"
#include "physics/collision/collision_data.h"

struct CircleCollider;
struct AABB2DCollider;
struct Ray2D;

class CollisionSolver
{
public:
	// 2D: Circle vs. Circle
	static CollisionResult2D Solve(const CircleCollider& c1, const CircleCollider& c2, const CollisionData2D& data);

	// 2D: Circle vs. AABB
	static CollisionResult2D Solve(const CircleCollider& c, const AABB2DCollider& b, const CollisionData2D& data);
	static CollisionResult2D Solve(const AABB2DCollider& b, const CircleCollider& c, const CollisionData2D& data);

	// 2D: AABB vs. AABB
	static CollisionResult2D Solve(const AABB2DCollider& b1, const AABB2DCollider& b2, const CollisionData2D& data);

	//static RayCast2D Solve(const CircleCollider& c, const Ray2D& ray, const CollisionData2D& data);
};