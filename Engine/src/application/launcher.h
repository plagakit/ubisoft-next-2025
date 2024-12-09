#pragma once

#include "application/application.h"

extern API::Application* API::CreateApplication();

int main(int argc, char** argv)
{
	auto game = API::CreateApplication();
	game->Update();
	game->Render();
	delete game;
}