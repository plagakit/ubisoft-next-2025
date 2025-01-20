#include "pch.h"
#include "scene.h"

Scene::Scene(Application& game) :
	m_game(game),
	m_input(game.GetInput()),
	m_resourceMgr(game.GetResourceManager()),
	m_renderer(game.GetRenderer()),
	m_tweenMgr(game.GetTweenManager()),
	m_GUI(std::make_unique<GUIRoot>())
{}

// Destructor is pure virtual but still needs to be defined b/c required for derived destructors
Scene::~Scene() = default;
