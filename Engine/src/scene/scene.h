#pragma once

#include "core/application/application.h"
#include "core/resource/resource_manager.h"
#include "graphics/renderer/renderer.h"
#include "gui/tween/tween_manager.h"
#include "gui/objects/root/gui_root.h"
#include <memory>

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
	TweenManager& m_tweenMgr;

	std::unique_ptr<GUIRoot> m_GUI;

private:


};