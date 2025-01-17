#pragma once

#include "systems/system.h"

class ParticleSystem : public System
{
public:
	ParticleSystem(EntityManager& registry);
	void RegisterAllRequiredComponents(size_t reserve) override;

	void Update(float dt);
};