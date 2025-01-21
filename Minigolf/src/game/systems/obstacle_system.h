#pragma once

#include <engine.h>

/**
Controls the behaviour of various obstacles.
*/
class ObstacleSystem : public System
{
public:
	ObstacleSystem(EntityManager& registry);
	void RegisterAllRequiredComponents(size_t n) override;

	void Update(float dt);

	void OnTrigger(Entity trigger, Entity e, CollisionResult2D col);

private:
	void ApplyLipoutForce(Entity lip, Entity ball);

};