#include "pch.h"
#include "launcher.h"

#include "core/application/application.h"
#include "core/debug/logger.h"

#ifdef PLATFORM_WINDOWS
#include <App/app.h>
#include <windows.h>
#include <filesystem>


// Fetch Next API's main function from app.h
extern int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
#endif

Application* s_application;

int __LaunchGame__(Application* app, ApplicationCmdLineArgs args)
{
	Logger::Info("Launching NEXT API...");
	s_application = app;

	// Start Ubisoft Next API's main function
	// we don't need to pass in args b/c their main doesn't use any
	HINSTANCE hInstance = GetModuleHandle(NULL);
	return wWinMain(hInstance, NULL, GetCommandLineW(), SW_SHOWDEFAULT);
}

void Init()
{
	Logger::Info("Running Init...");
	s_application->__Init__();
}

void Update(const float deltaTime)
{
	s_application->__Update__(deltaTime);
}

void Render()
{
	s_application->__Render__();
}

void Shutdown()
{
	Logger::Info("Running Shutdown...");
	s_application->__Shutdown__();
	delete s_application;
}