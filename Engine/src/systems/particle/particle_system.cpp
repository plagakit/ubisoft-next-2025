#include "pch.h"
#include "particle_system.h"

#include "components/particle/particle.h"

ParticleSystem::ParticleSystem(EntityManager& registry) :
	System(registry)
{}

void ParticleSystem::RegisterAllRequiredComponents(size_t reserve)
{
	m_registry.RegisterComponentType<Particle>(reserve);
}

void ParticleSystem::Update(float dt)
{
	for (auto& [id, pt] : m_registry.AllWith<Particle>())
	{
		pt.lifetime -= dt;
		if (pt.lifetime < 0)
			m_registry.QueueDelete(id);
	}
}
