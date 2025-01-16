#include "pch.h"
#include "collision_solver.h"

#include "components/2d/transform_2d.h"
#include "physics/collider/circle_collider.h"

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
