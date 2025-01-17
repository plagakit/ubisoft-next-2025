#include "pch.h"
#include "physics_system.h"

#include "physics/collider/collider_2d.h"
#include "core/debug/logger.h"

PhysicsSystem::PhysicsSystem(EntityManager& registry, ResourceManager& resourceMgr, Renderer& renderer) :
	System(registry),
	m_resourceMgr(resourceMgr),
	m_renderer(renderer)
{}

void PhysicsSystem::RegisterAllRequiredComponents(size_t reserve)
{
	Register2DMovement(reserve);
	Register3DMovement(reserve);
	Register2DPhysics(reserve);
	Register3DPhysics(reserve);
}

void PhysicsSystem::Register2DMovement(size_t reserve)
{
	m_registry.RegisterComponentType<Transform2D>(reserve);
}

void PhysicsSystem::Register3DMovement(size_t reserve)
{
	m_registry.RegisterComponentType<Transform3D>(reserve);
}

void PhysicsSystem::Register2DPhysics(size_t reserve)
{
	m_registry.RegisterComponentType<Physics2D>(reserve);
}

void PhysicsSystem::Register3DPhysics(size_t reserve)
{
	m_registry.RegisterComponentType<Physics3D>(reserve);
}

void PhysicsSystem::Update2DMovement(float dt)
{
	for (auto [id, tf] : m_registry.AllWith<Transform2D>())
	{
		tf.velocity += tf.acceleration * dt;
		tf.position += tf.velocity * dt;
		tf.rotation += tf.angVelocity * dt;
	}
}

void PhysicsSystem::Update3DMovement(float dt)
{
	for (auto [id, tf] : m_registry.AllWith<Transform3D>())
	{
		tf.velocity += tf.acceleration * dt;
		tf.position += tf.velocity * dt;
		tf.orientation *= tf.angVelocity * dt;
	}
}

void PhysicsSystem::ProcessAll2DCollisions(float dt)
{
	// We skip out on the syntactic sugar here for efficiency
	EntityView<Physics2D, Transform2D>&& entityView{ m_registry, 0 };
	auto itBegin = entityView.begin();
	auto itEnd = entityView.end();

	for (auto it1 = itBegin; it1 != itEnd; ++it1)
	{
		auto [id1, ph1, tf1] = *it1;
		auto it2 = it1; ++it2; // next entity right after current one

		for (; it2 != itEnd; ++it2)
		{
			auto [id2, ph2, tf2] = *it2;
			Process2DCollision(id1, id2, tf1, ph1, tf2, ph2);
		}
	}
}

RayCast2D PhysicsSystem::QueryAll2D(const Ray2D& ray)
{
	for (auto [id, ph, tf] : m_registry.AllWith<Physics2D, Transform2D>())
	{

	}

	return { false };
}

void PhysicsSystem::RenderAllCollisionShapes()
{
	for (auto [id, ph, tf] : m_registry.AllWith<Physics2D, Transform2D>())
	{
		Collider2D& collider = m_resourceMgr.Get<Collider2D>(ph.colliderHandle);
		collider.DebugDraw(m_renderer, tf.position);
	}
}

void PhysicsSystem::Process2DCollision(Entity id1, Entity id2, Transform2D& tf1, Physics2D& ph1, Transform2D& tf2, Physics2D& ph2)
{
	Collider2D& c1 = m_resourceMgr.Get<Collider2D>(ph1.colliderHandle);
	Collider2D& c2 = m_resourceMgr.Get<Collider2D>(ph2.colliderHandle);

	CollisionData2D data = { tf1, tf2 };
	CollisionResult2D result = c1.Collide(c2, data);

	if (result.hit)
	{
		ph1.lastHit = result;
		ph2.lastHit = result;

		if (ph1.isTrigger || ph2.isTrigger)	
			s_Triggered.Emit(id1, id2);
		else
		{
			s_Collided.Emit(id1, id2);

			if (ph1.isImmovable)
				tf2.position -= result.restitution * 2.0f;
			else if (ph2.isImmovable)
				tf1.position += result.restitution * 2.0f;
			else
			{
				tf1.position += result.restitution;
				tf2.position -= result.restitution;
			}
		}
	}
}
