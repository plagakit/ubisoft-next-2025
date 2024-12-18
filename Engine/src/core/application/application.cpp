#include "application.h"

#include "core/logger/logger.h"

#include <App/app.h>
#include <windows.h>

// Fetch Next API's main function from App/
extern int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);

Application* s_application;

int LaunchGame(Application* app, ApplicationCmdLineArgs args)
{
	Logger::Info("Launching NEXT API...");
	Logger::Warn("this is my warning");
	Logger::Error("AAAAAAAAAAAAAAAAAAAAAA");
	s_application = app;

	// Start Ubisoft Next API's main function
	// we don't need to pass in args b/c their main doesn't use any
	HINSTANCE hInstance = GetModuleHandle(NULL);
	return wWinMain(hInstance, NULL, GetCommandLineW(), SW_SHOWDEFAULT);
}

void Init()
{
	Logger::Info("Running Init...");
	s_application->Init();
}

void Update(const float deltaTime)
{
	s_application->Update(deltaTime);
}

void Render()
{
	s_application->Render();
}

void Shutdown()
{
	Logger::Info("Running Shutdown...");
	s_application->Shutdown();
	delete s_application;
}