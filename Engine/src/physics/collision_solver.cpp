#include "pch.h"
#include "collision_solver.h"

#include "components/2d/transform_2d.h"
#include "physics/collider/circle_collider.h"

CollisionResult2D CollisionSolver::Solve(const CircleCollider& c1, const CircleCollider& c2, const CollisionData2D& data)
{
	Vec2 c1p = data.tf1.position + c1.offset;
	Vec2 c2p = data.tf2.position + c2.offset;
	float distSq = c1p.DistanceSq(c2p);
	float colDist = (c1.radius + c2.radius) * (c1.radius + c2.radius);

	if (distSq < colDist)
	{
		return { true, (c2p - c1p).Normalized()} ;
	}

	return { false };
}
