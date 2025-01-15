#include "pch.h"
#include "timer_system.h"

#include "components/timer.h"

void TimerSystem::Update(float dt)
{
	for (auto [id, tm] : m_registry.AllWith<Timer>())
	{
		if (tm.isRunning)
		{
			tm.elapsedTime += dt;
			if (tm.elapsedTime >= tm.duration)
			{
				fired.Emit(id);
				if (!tm.firesOnce)
				{
					tm.elapsedTime = 0.0f;
					tm.isDone = false;
				}
				else
				{
					tm.isDone = true;
				}
			}
		}
	}
}
