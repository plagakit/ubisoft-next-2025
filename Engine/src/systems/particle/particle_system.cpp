#include "pch.h"
#include "particle_system.h"

#include "components/particle/particle.h"

void ParticleSystem::Update(float dt)
{
	for (auto& [id, pt] : m_registry.AllWith<Particle>())
	{
		pt.lifetime -= dt;
		if (pt.lifetime < 0)
			m_registry.QueueDelete(id);
	}
}
