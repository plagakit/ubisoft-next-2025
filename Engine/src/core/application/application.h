#pragma once

#include "core/input/input.h"
#include "core/resource/resource_manager.h"
#include "graphics/renderer/renderer.h"
#include "gui/tween/tween_manager.h"
#include "entity/entity_manager/entity_manager.h"

#include <memory>

struct ApplicationCmdLineArgs 
{
	int m_argc = 0;
	char** m_argv = nullptr;
};

class Application 
{
public:
	void __Init__();
	void __Shutdown__();
	void __Update__(float dt);
	void __Render__();

	Input& GetInput();
	ResourceManager& GetResourceManager();
	Renderer& GetRenderer();
	TweenManager& GetTweenManager();

protected:
	Input* m_input;
	ResourceManager* m_resourceMgr;
	Renderer* m_renderer;
	TweenManager* m_tweenMgr;

	virtual void Init() {};
	virtual void Shutdown() {};
	virtual void Update(float dt) {};
	virtual void Render() {};

private:
	void PreInit();
	void PreUpdate(float dt);

	void PostShutdown();
};