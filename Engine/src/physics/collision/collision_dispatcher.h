#pragma once

#include "physics/collision/collision_data.h"
#include "physics/collision/collision_result.h"
#include "physics/collision/collision_solver.h"

/*
Implementation of Visitor design pattern so that we can just
do Collider2D.Collide(Collider2D).

Resources:
https://refactoring.guru/design-patterns/visitor/cpp/example
*/

struct CircleCollider;
struct AABB2DCollider;

class CollisionDispatcher2DBase 
{
public:
	virtual CollisionResult2D Collide(const CircleCollider& other, const CollisionData2D& data) const = 0;
	virtual CollisionResult2D Collide(const AABB2DCollider& other, const CollisionData2D& data) const = 0;
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

	CollisionResult2D Collide(const AABB2DCollider& other, const CollisionData2D& data) const override
	{
		return CollisionSolver::Solve(t, other, data);
	}

private:
	const T& t;

};