#include "pch.h"
#include "application.h"

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
	PreUpdate(dt);
	Update(dt);
}

void Application::__Render__()
{
	Render();
}

void Application::PreInit()
{
	input.InitDefaultActions();
}

void Application::PreUpdate(float dt)
{
	input.Update();
}

