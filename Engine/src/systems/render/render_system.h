#pragma once

#include "systems/system.h"
#include "entity/entity_manager/entity_manager.h"
#include "graphics/renderer/renderer.h"

class RenderSystem : public System
{
public:
	RenderSystem(EntityManager& registry, Renderer& renderer);

	void RegisterAllRequiredComponents(size_t reserve) override;
	void Register2DComponents(size_t reserve);
	void Register3DComponents(size_t reserve);

	void Render2DEntities();
	void Render3DEntities();

private:
	Renderer& m_renderer;

};