#pragma once

#include "common.h"

#define REGISTER_GAME(GAME) \
    API::Application* API::CreateApplication() \
    { \
        return new GAME(); \
    }


namespace API {

	class ENGINE_API Application 
	{
	public:
		Application();
		~Application();

		virtual void Update() = 0;
		virtual void Render() = 0;
	};

	Application* CreateApplication();

}