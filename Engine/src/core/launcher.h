#pragma once

class Application;
struct ApplicationCmdLineArgs;

/**
* Internal function to start the application by calling NextAPI's main 
* function. Don't use this to start your game, just use the REGSITER_GAME 
* macro defined in launcher.h.
*/
int __LaunchGame__(Application* app, ApplicationCmdLineArgs args);

/**
* A macro to register the game so that it can be launched by the
* Engine's entry point. Pass in the type of your Application subclass
* here to run it.
* 
* If we are in debug mode, we make the entrypoint main() and the 
* subsystem a console so that we can see the standard output. If
* we are in release mode, we make the subsystem Windows to prevent
* creating a console, but we manually set the entrypoint to 
* mainCRTstartup so that we can use the main function below.
*/
#define REGISTER_GAME(TYPE) \
	int main(int argc, char** argv) \
	{ \
		TYPE* app = new TYPE(); \
		__LaunchGame__(app, { argc, argv }); \
	}

// External functions defined by Next API

void Init();
void Update(const float deltaTime);
void Render();
void Shutdown();