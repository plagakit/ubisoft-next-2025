#pragma once

#include "systems/system.h"
#include "core/signal/signal.h"
#include "components/2d/transform_2d.h"
#include "components/3d/transform_3d.h"
#include "components/2d/physics_2d.h"
#include "components/3d/physics_3d.h"
#include "math/shape/ray_2d.h"
#include "math/shape/ray_3d.h"
#include "core/resource/resource_manager.h"
#include "graphics/renderer/renderer.h"

class PhysicsSystem : public System
{
public:
	PhysicsSystem(EntityManager& registry, ResourceManager& resourceMgr, Renderer& renderer);
	void RegisterAllRequiredComponents(size_t reserve) override;
	void Register2DMovement(size_t reserve);
	void Register3DMovement(size_t reserve);
	void Register2DPhysics(size_t reserve);
	void Register3DPhysics(size_t reserve);

	Signal<Entity, Entity, CollisionResult2D> s_Collided;
	Signal<Entity, Entity, CollisionResult2D> s_Triggered;

	void Update2DMovement(float dt);
	void Update3DMovement(float dt);

	void Process2DCollision(Entity id1, Entity id2);
	void Process2DCollisions(const std::vector<Entity>& group1, const std::vector<Entity> group2);
	void ProcessAll2DCollisions(float dt);
	RayCast2D QueryAll2D(const Ray2D& ray);

	void RenderAllCollisionShapes();

private:
	ResourceManager& m_resourceMgr;
	Renderer& m_renderer;

	void Process2DCollision(Entity id1, Entity id2, Transform2D& tf1, Physics2D& ph1, Transform2D& tf2, Physics2D& ph2);

};