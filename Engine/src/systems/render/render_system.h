#pragma once

#include "systems/system.h"
#include "entity/entity_manager/entity_manager.h"
#include "graphics/renderer/renderer.h"

class RenderSystem : public System
{
public:
	RenderSystem(EntityManager& registry, Renderer& renderer);

	void Render2DEntities();
	void Render3DEntities();

private:
	Renderer& m_renderer;

};