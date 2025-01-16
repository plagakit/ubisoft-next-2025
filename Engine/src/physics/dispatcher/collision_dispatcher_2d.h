#pragma once

#include "physics/collision_data.h"
#include "physics/collision_result.h"
#include "physics/collision_solver.h"

class CollisionDispatcher2DBase 
{
public:
	virtual CollisionResult2D Collide(const CircleCollider& other, const CollisionData2D& data) const = 0;
	//virtual CollisionResult2D Collide(const Ray2DCollider& other, const CollisionData2D& data) const = 0;
};

template <typename T>
class CollisionDispatcher2D : public CollisionDispatcher2DBase {

public:
	CollisionDispatcher2D(const T& t) :
		t(t)
	{}

	CollisionResult2D Collide(const CircleCollider& other, const CollisionData2D& data) const override
	{
		return CollisionSolver::Solve(t, other, data);
	};

private:
	const T& t;

};