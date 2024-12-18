#pragma once

#include "common.h"

// if the game is in debug mode, we make the entrypoint main() and
// the subsystem a console so that we can see std::cout. if we are
// in release, we can make the entrypoint the Windows one so that
// we don't spawn a console.
#ifdef _DEBUG

#define REGISTER_GAME(TYPE) \
	int main(int argc, char** argv) \
	{ \
		TYPE* app = new TYPE(); \
		LaunchGame(app, { argc, argv }); \
	}

#else

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define REGISTER_GAME(TYPE) \
	int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) \
	{ \
		TYPE* app = new TYPE(); \
		LaunchGame(app, { 0, NULL }); \
	}

#endif

struct ApplicationCmdLineArgs 
{
	int m_argc = 0;
	char** m_argv = NULL;

};

class Application 
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
};

int LaunchGame(Application* app, ApplicationCmdLineArgs args);

// External functions defined by Next API
void Init();
void Update(const float deltaTime);
void Render();
void Shutdown();