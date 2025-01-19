#include "fake_3d_system.h"

#include "game/components/fake_3d.h"

Fake3DSystem::Fake3DSystem(EntityManager& registry) :
	System(registry)
{}

void Fake3DSystem::RegisterAllRequiredComponents(size_t n)
{
	m_registry.RegisterComponentType<Transform2D>(n);
	m_registry.RegisterComponentType<Transform3D>(n);
	m_registry.RegisterComponentType<Fake3D>(n);
}

void Fake3DSystem::Update(float dt)
{
	for (auto [id, f, tf2D, tf3D] : m_registry.AllWith<Fake3D, Transform2D, Transform3D>())
	{
		tf3D.position = Vec3(
			tf2D.position.x,
			f.y,
			tf2D.position.y
		);
	}
}
