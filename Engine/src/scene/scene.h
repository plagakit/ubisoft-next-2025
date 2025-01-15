#pragma once

#include "core/application/application.h"
#include "core/resource/resource_manager.h"
#include "graphics/renderer/renderer.h"

class Scene
{
public:
	Scene() = delete;
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	Scene(ResourceManager& resourceMgr, Renderer& renderer);
	virtual ~Scene() = 0;

	
	
protected:
	Application& m_game;
	ResourceManager& m_resourceMgr;
	Renderer& m_renderer;

private:


};