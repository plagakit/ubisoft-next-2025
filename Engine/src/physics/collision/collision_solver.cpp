#include "pch.h"
#include "collision_solver.h"

#include "math/math_utils.h"
#include "components/2d/transform_2d.h"
#include "physics/collider/circle_collider.h"
#include "physics/collider/aabb_2d_collider.h"

// CIRCLE x CIRCLE
CollisionResult2D CollisionSolver::Solve(const CircleCollider& c1, const CircleCollider& c2, const CollisionData2D& data)
{
	Vec2 c1p = data.tf1.position + c1.offset;
	Vec2 c2p = data.tf2.position + c2.offset;
	float radius = c1.radius + c2.radius;
	float distSq = c1p.DistanceSq(c2p);

	if (distSq < radius * radius)
	{
		Vec2 normal = (c2p - c1p).Normalized();
		float restitution = sqrtf(distSq) - radius;
		return { 
			true, 
			normal,
			normal * restitution * 0.5f
		};
	}

	return { false };
}

// CIRCLE x AABB
CollisionResult2D CollisionSolver::Solve(const CircleCollider& c, const AABB2DCollider& b, const CollisionData2D& data)
{
	// Get box extents
	Vec2 half = Vec2(b.width, b.height) * 0.5f;
	Vec2 min = data.tf2.position + b.offset - half;
	Vec2 max = data.tf2.position + b.offset + half;
	Vec2 circ = data.tf1.position + c.offset;

	// Calc closest point from circ center to box
	Vec2 closest;
	closest.x = std::min(std::max(circ.x, min.x), max.x);
	closest.y = std::min(std::max(circ.y, min.y), max.y);

	Vec2 diff = circ - closest;
	float distSq = diff.LengthSq();
	if (distSq <= c.radius * c.radius)
	{
		CollisionResult2D result;
		result.hit = true;

		float dist = sqrtf(distSq);
		if (dist < EPSILON)	result.contactNormal = Vec2(1, 0);
		else				result.contactNormal = diff / dist;
			
		result.restitution = result.contactNormal * (c.radius - dist);
		ASSERT_ERROR(!std::isnan(result.restitution.x), "");
		return result;
	}

	return { false };

	//Vec2 dist = (data.tf2.position + b.offset) - (data.tf1.position + c.offset);
	//dist.x = std::abs(dist.x);
	//dist.y = std::abs(dist.y);

	//float halfW = b.width * 0.5f;
	//float halfH = b.height * 0.5f;

	//// If circle is not intersecting sides of box
	//if (dist.x > (halfW + c.radius) || dist.y > (halfH + c.radius))
	//	return { false };
	//
	//// If circle intersects corner
	//bool doesObviouslyIntersect = dist.x < halfW || dist.y < halfH;
	//if (!doesObviouslyIntersect)
	//{
	//	float cornerDistSq = (dist.x - halfW) * (dist.x - halfW)
	//		+ (dist.y - halfH) * (dist.y - halfH);
	//	if (cornerDistSq >= c.radius * c.radius)
	//		return { false };
	//}

	//CollisionResult2D result;
	//result.hit = true;

	//// Find overlap and project onto x & y axis
	//Vector2 overlap = Vector2(
	//	halfW + c.radius - std::abs(dist.x),
	//	halfH + c.radius - std::abs(dist.y)
	//);

	//// Determine which axis to move based on greater overlap
	//if (overlap.x < overlap.y)
	//{
	//	int dir = Math::Sign(dist.x);
	//	result.contactNormal = Vec2(dir, 0.0f);
	//	result.restitution = overlap * Vec2(-1.0f * dir, 0);
	//}
	//else
	//{
	//	int dir = Math::Sign(dist.y);
	//	result.contactNormal = Vec2(0.0f, dir);
	//	result.restitution = overlap * Vec2(0, -1.0f * dir);
	//}

	//return result;
}

CollisionResult2D CollisionSolver::Solve(const AABB2DCollider& b, const CircleCollider& c, const CollisionData2D& data)
{
	return Solve(c, b, data);
}

// AABB x AABB
CollisionResult2D CollisionSolver::Solve(const AABB2DCollider& b1, const AABB2DCollider& b2, const CollisionData2D& data)
{
	Vec2 p1 = data.tf1.position + b1.offset;
	Vec2 p2 = data.tf2.position + b2.offset;

	bool isColliding = 
		p1.x + b1.width / 2 > p2.x - b2.width / 2 &&	// 1's right edge past 2's left
		p1.x - b1.width / 2 < p2.x + b2.width / 2 &&	// 1's left edge past 2's right
		p1.y + b1.height / 2 > p2.y - b2.height / 2 &&  // 1's top edge past 2's bottom
		p1.y - b1.height / 2 < p2.y + b2.height / 2;    // 1's bottom edge past 2's top

	if (isColliding)
	{
		CollisionResult2D result;
		result.hit = true;

		// Find overlap and project onto x & y axis
		Vec2 dist = p2 - p1;
		Vec2 overlap = Vec2(
			(b1.width + b2.width) * 0.5f - std::abs(dist.x),
			(b1.height + b2.height) * 0.5f - std::abs(dist.y)
		);

		// Determine which axis to move based on greater overlap
		if (overlap.x < overlap.y)
		{
			float dir = Math::Sign(dist.x);
			result.contactNormal = Vec2(dir, 0.0f);
			result.restitution = overlap * Vec2(-1.0f * dir, 0);
		}
		else
		{
			float dir = Math::Sign(dist.y);
			result.contactNormal = Vec2(0.0f, dir);
			result.restitution = overlap * Vec2(0, -1.0f * dir);
		}
	
		return result;
	}

	return { false };
}

//RayCast2D CollisionSolver::Solve(const CircleCollider& c, const Ray2D& r, const CollisionData2D& data)
//{
//    // Analytical soln (uses quadratic eqn)
//
//    Vec2 circCenter = c.offset + data.tf1.position;
//    Vec2 rayOrig = r.origin + data.tf2.position;
//    Vec2 rayDir = r.direction.Rotated(data.tf2.rotation);
//
//    Vec2 oc = rayOrig - circCenter;
//
//    float a = 1.0f; // rayDir.Dot(rayDir);
//    float b = 2.0f * oc.Dot(rayDir);
//    float c_sq = oc.Dot(oc) - (c.radius * c.radius);
//
//    float discriminant = (b * b) - (4.0f * a * c_sq);
//    if (discriminant >= 0.0f)
//    {
//        float sqrtDiscriminant = sqrt(discriminant);
//        float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
//        float t2 = (-b + sqrtDiscriminant) / (2.0f * a);
//
//        // Smallest non-negative t is one closest to ray origin
//        float t = -1.0f;
//        if (t1 >= 0.0f && t2 >= 0.0f)   t = std::min(t1, t2);
//        else if (t1 >= 0.0f)            t = t1;
//        else if (t2 >= 0.0f)            t = t2;
//
//        if (t >= 0.0f)
//        {
//            Vec2 hitPos = rayOrig + rayDir * t;
//            Vec2 hitNorm = (hitPos - circCenter) / c.radius;
//
//            return {
//                true,
//                hitPos,
//                hitNorm,
//                t,
//                c.GetRID(),
//                NULL_ENTITY
//            };
//        }
//    }
//    return { false };
//}
//
