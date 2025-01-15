#pragma once

#include "systems/system.h"
#include "core/signal/signal.h"

class TimerSystem : public System
{
public:
	Signal<Entity> fired;

	void Update(float dt);

};