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
	Input::InitDefaultActions();

	m_rm = std::make_unique<ResourceManager>();
	m_renderer = std::make_unique<Renderer>(*m_rm.get());
}

void Application::PreUpdate(float dt)
{
	Input::Update();
}

