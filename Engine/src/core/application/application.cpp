#include "pch.h"
#include "application.h"

#include "core/input/input.h"

void Application::__Init__()
{
	PreInit();
	Init();
}

void Application::__Shutdown__()
{
	Shutdown();
}

void Application::__Update__(float dt)
{
	// Convert delta time to seconds
	// TODO: add delta time smoothing
	dt *= 0.001f;

	PreUpdate(dt);
	Update(dt);
}

void Application::__Render__()
{
	Render();
}

void Application::PreInit()
{
	Logger::Init();
	
	m_input = new Input();
	m_resourceMgr = new ResourceManager();
	m_renderer = new Renderer(*m_resourceMgr);
	m_tweenMgr = new TweenManager();
}

void Application::PreUpdate(float dt)
{
	m_input->Update(dt);
	m_tweenMgr->Update(dt);
}

void Application::PostShutdown()
{
	delete m_input;
	delete m_resourceMgr;
	delete m_renderer;
	delete m_tweenMgr;
}

