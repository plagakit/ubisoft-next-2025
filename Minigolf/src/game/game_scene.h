#pragma once

#include <engine.h>

#include "game/course_manager/course_manager.h"
#include "game/systems/fake_3d_system.h"
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
	Vec3 m_cameraOffset;

	EntityManager m_registry;

	PhysicsSystem m_physicsSystem;
	RenderSystem m_renderSystem;
	ParticleSystem m_particleSystem;

	Fake3DSystem m_fake3DSystem;
	GolfballSystem m_golfballSystem;

	CourseManager m_course;

	Entity m_player;
	std::vector<Entity> m_walls;

	bool m_isDebugOn;

	void DrawGrid();

};