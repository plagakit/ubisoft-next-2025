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

	void OnTrigger(Entity trigger, Entity e, CollisionResult2D col);

private:
	static constexpr float CENTRIPETAL_MULTIPLIER = 2.0f;

	void ApplyLipoutForce(Entity lip, Entity ball);

};