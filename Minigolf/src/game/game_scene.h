#pragma once

#include <engine.h>

#include "game/systems/fake_3d_system.h"
#include "game/systems/golfball_system.h"
#include "game/systems/obstacle_system.h"
#include "game/course/course_manager.h"

class GameScene : public Scene
{
public:
	GameScene(Application& game);
	~GameScene() override;
	void Update(float dt) override;
	void Render() override;

	void StartGame();

	void OnSunkBall(Entity ball);

private:
	RID m_treePNG;
	RID m_planeOBJ;
	RID m_soundBGM;
	RID m_soundWhistle;

	EntityManager m_registry;

	PhysicsSystem m_physicsSystem;
	RenderSystem m_renderSystem;
	ParticleSystem m_particleSystem;

	Fake3DSystem m_fake3DSystem;
	GolfballSystem m_golfballSystem;
	ObstacleSystem m_obstacleSystem;

	enum GameState
	{
		GENERATING,
		RUNNING_COURSE,
		ENDING_ROUND
	};
	GameState m_state;
	CourseManager m_course;

	DebugCamera m_camera;
	Vec3 m_cameraOffset;
	static const Vec3 INGAME_CAMERA_OFFSET;

	Entity m_grass;
	Entity m_player;

	static constexpr float START_TIME = 20.0f;
	float m_timeLeft;

	static constexpr int START_PART_COUNT = 2;
	int m_curRound;
	int m_roundPartCount;

	bool m_isDebugOn;

	void NextRound();
	void EndRound();

};