#include "pch.h"
#include "render_system.h"

#include "components/2d/sprite.h"
#include "components/2d/transform_2d.h"
#include "components/3d/mesh_instance.h"
#include "components/3d/transform_3d.h"

RenderSystem::RenderSystem(EntityManager& registry, Renderer& renderer) :
	System(registry),
	m_renderer(renderer)
{}

void RenderSystem::Render2DEntities()
{
	for (auto [id, sp, tf] : m_registry.AllWith<Sprite, Transform2D>())
	{
		m_renderer.DrawTexture(tf, sp.textureHandle);
	}
}

void RenderSystem::Render3DEntities()
{
	for (auto [id, mi, tf] : m_registry.AllWith<MeshInstance, Transform3D>())
	{
		m_renderer.DrawMesh(tf.ToMatrix(), mi);
	}
}
