#pragma once

#include "systems/system.h"
#include "core/signal/signal.h"
#include "components/2d/transform_2d.h"
#include "components/3d/transform_3d.h"
#include "components/2d/physics_2d.h"
#include "math/shape/ray_2d.h"
#include "math/shape/ray_3d.h"
#include "core/resource/resource_manager.h"
#include "graphics/renderer/renderer.h"

class PhysicsSystem : public System
{
public:
	PhysicsSystem(EntityManager& registry, ResourceManager& resourceMgr, Renderer& renderer);

	Signal<Entity, Entity> s_Collided;
	Signal<Entity, Entity> s_Triggered; // first entity is the trigger

	void UpdateMovement(float dt);

	void ProcessAllCollisions(float dt);
	RayCast2D QueryAll(const Ray2D& ray);

	void RenderAllCollisionShapes();

private:
	ResourceManager& m_resourceMgr;
	Renderer& m_renderer;

	void Process2DCollision(Entity id1, Entity id2, Transform2D& tf1, Physics2D& ph1, Transform2D& tf2, Physics2D& ph2);

};