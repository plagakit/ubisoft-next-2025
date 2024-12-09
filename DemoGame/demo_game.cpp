#include <engine.h>

#include <iostream>

class DemoGame : public API::Application 
{

	void Update()
	{
		std::cout << "Hello World! from dervied game" << std::endl;
	}

	void Render()
	{
		std::cout << "Render! from derived game" << std::endl;
	}


};

REGISTER_GAME(DemoGame)