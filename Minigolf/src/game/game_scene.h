#pragma once

#include <engine.h>

#include "game/systems/fake_3d_system.h"
#include "game/systems/golfball_system.h"
#include "game/systems/obstacle_system.h"
#include "game/course/course_manager.h"
#include "game/gui/timer_number.h"

class GameScene : public Scene
{
public:
	GameScene(Application& game, bool playBGM);
	~GameScene() override;
	void Update(float dt) override;
	void Render() override;

	void StartGame();

	Signal<int> s_EndedGame;

	void OnSunkBall(Entity ball);

private:
#pragma region Resources
	//RID m_treePNG;
	RID m_numbersPNG;
	RID m_fontRoundLabel;
	//RID m_planeOBJ;
	RID m_soundBGM;
	RID m_soundWhistle;
	RID m_soundCharge;
#pragma endregion Resources

#pragma region ECS
	EntityManager m_registry;

	PhysicsSystem m_physicsSystem;
	RenderSystem m_renderSystem;
	ParticleSystem m_particleSystem;

	Fake3DSystem m_fake3DSystem;
	GolfballSystem m_golfballSystem;
	ObstacleSystem m_obstacleSystem;
#pragma region ECS

#pragma region Game Config
	static constexpr float MESH_CULLING_HEURISTIC_DIST = 500.0f;

	static constexpr Vec3 INGAME_CAMERA_OFFSET{ 0.0f, 20.0f, -4.0f };
	static constexpr Vec3 POSTROUND_CAM_POS{ 0.0f, 40.0f, -10.0f };
	static constexpr float ROUND_TRANS_TIME = 5.0f;
	static constexpr float START_TIME = 35.0f;
	static constexpr float ROUND_ADD_TIME = 15.0f;
	static constexpr int START_PART_COUNT = 2;
	static constexpr int ADD_PART_EVERY = 3;
#pragma endregion

#pragma region Game Logic
	enum GameState
	{
		PRE_ROUND,
		MID_ROUND,
		POST_ROUND
	};
	GameState m_state;
	CourseManager m_course;

	Camera m_camera;
	Vec3 m_cameraOffset;

	//Entity m_grass;
	Entity m_player;

	float m_timeLeft;
	int m_curRound;
	int m_roundPartCount;

	void LoseGame();
	void EndRound();
	void StartRound();
	void PrepareNextRound();
#pragma endregion Game Logic

#pragma region GUI
	TimerNumber* m_timeLabel;
	Label* m_roundLabel;

	void FocusTimeLabel();
	void UnfocusTimeLabel();
#pragma endregion

	bool m_isDebugOn;
	bool m_playBGM;

};