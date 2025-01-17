#pragma once

#include "systems/system.h"
#include "core/signal/signal.h"

class TimerSystem : public System
{
public:
	TimerSystem(EntityManager& registry);
	void RegisterAllRequiredComponents(size_t reserve) override;

	Signal<Entity> fired;

	void Update(float dt);

};