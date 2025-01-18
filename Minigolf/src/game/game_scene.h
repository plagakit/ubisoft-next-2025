#pragma once

#include <engine.h>
#include "game/systems/golfball_system.h"

class GameScene : public Scene
{
public:
	GameScene(Application& game);
	~GameScene() override;
	void Update(float dt) override;
	void Render() override;

private:
	RID m_treePNG;
	RID m_planeOBJ;
	RID m_bgmWAV;

	DebugCamera m_camera;

	EntityManager m_registry;

	PhysicsSystem m_physicsSystem;
	RenderSystem m_renderSystem;
	ParticleSystem m_particleSystem;
	GolfballSystem m_golfballSystem;

	Entity m_player;

};