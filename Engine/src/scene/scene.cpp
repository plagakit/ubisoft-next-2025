#include "pch.h"
#include "scene.h"

Scene::Scene(Application& game) :
	m_game(game),
	m_input(*game.m_input),
	m_resourceMgr(*game.m_resourceMgr),
	m_renderer(*game.m_renderer),
	m_tweenMgr(*game.m_tweenMgr),
	m_GUI(std::make_unique<GUIRoot>())
{}

// Destructor is pure virtual but still needs to be defined b/c required for derived destructors
Scene::~Scene() = default;
