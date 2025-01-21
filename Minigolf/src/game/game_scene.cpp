#include "game_scene.h"

#include "components/fake_3d.h"
#include "components/golfball.h"
#include "components/obstacle.h"

GameScene::GameScene(Application& game) :
	Scene(game),
	m_physicsSystem(m_registry, m_resourceMgr, m_renderer),
	m_renderSystem(m_registry, m_renderer),
	m_particleSystem(m_registry),

	m_fake3DSystem(m_registry),
	m_golfballSystem(m_registry, m_renderer, m_resourceMgr, m_input, m_tweenMgr),
	m_obstacleSystem(m_registry),

	m_course(m_registry, m_resourceMgr),

	m_isDebugOn(false)
{
	// Load inputs
	m_input.CreateAction("toggle-debug");
	m_input.AddEvent<InputEventVirtual>("toggle-debug", 'T');

	// Load resources
	//m_treePNG = m_resourceMgr.Load<Texture>("res/sprites/tree.png");
	//m_planeOBJ = m_resourceMgr.Load<Mesh>("res/models/plane.obj");
	m_fontRoundLabel = m_resourceMgr.Load<Font>("HELVETICA_18");
	auto [numsRID, nums] = m_resourceMgr.LoadAndGet<Texture>("res/sprites/numbers.png");
	m_numbersPNG = numsRID;
	nums.SetSheetDimensions(1, 11);

	auto [bgmRID, bgm] = m_resourceMgr.LoadAndGet<Audio>("res/audio/bgm.wav");
	m_soundBGM = bgmRID;
	bgm.SetLooping(true);
	m_soundWhistle = m_resourceMgr.Load<Audio>("res/audio/whistle.wav");
	m_soundCharge = m_resourceMgr.Load<Audio>("res/audio/charge.wav");
	
	// Initialize renderer
	m_renderer.SetProjectionMatrix(m_camera.GetProjection());
	m_renderer.SetClearColor(Color(0.765f, 0.925f, 1.0f));

	m_cameraOffset = Vec3(0.0f, 20.0f, -2.0f);
	m_camera.GetTransform().position = m_cameraOffset;
	m_camera.GetTransform().orientation = Quat::LookAt(m_cameraOffset, Vec3::ZERO);

	// Initialize ECS
	m_registry.ReserveEntities(1000);
	m_physicsSystem.Register2DMovement(1000);
	m_physicsSystem.Register3DMovement(1000);
	m_physicsSystem.Register2DPhysics(1000);
	m_renderSystem.Register3DComponents(1000);
	m_particleSystem.RegisterAllRequiredComponents(1000);
	m_golfballSystem.RegisterAllRequiredComponents(2);
	m_fake3DSystem.RegisterAllRequiredComponents(1000);
	m_obstacleSystem.RegisterAllRequiredComponents(500);

	// Create plane
	// I took the plane out at the last minute because it takes
	// too long to rasterize - there's probably a soln but its
	// 7 PM on Monday :')
	m_renderer.SetClearColor(Color(0.15f, 0.6f, 0.0f));
	//{
	//	m_grass = m_registry.CreateEntity();
	//	Transform3D tf; tf.scale = Vec3(600.0f, 1.0f, 600.0f);
	//	MeshInstance m = { m_planeOBJ, Color(0.15f, 0.6f, 0.0f), ShadingMode::FILLED};
	//	m_registry.Add<Transform3D>(m_grass, tf);
	//	m_registry.Add<MeshInstance>(m_grass, m);
	//}

	// Create trees
	//for (int i = 0; i < 0; i++)
	//{
	//	float x = FRAND_RANGE(-100.0f, 100.0f);
	//	float z = FRAND_RANGE(-100.0f, 100.0f);
	//	Entity tree = m_registry.CreateEntity();
	//	Transform3D tf; 
	//	tf.position = Vec3(x, 5.0f, z);
	//	tf.scale = Vec3(1.0f, 10.0f, 1.0f);
	//	Sprite s; s.textureHandle = m_treePNG;
	//	m_registry.Add<Transform3D>(tree, tf);
	//	m_registry.Add<Sprite>(tree, s);
	//}

	// Connect signals
	m_physicsSystem.s_Collided.Connect<GolfballSystem, &GolfballSystem::OnCollision>(&m_golfballSystem);
	m_physicsSystem.s_Triggered.Connect<GolfballSystem, &GolfballSystem::OnTrigger>(&m_golfballSystem);
	m_physicsSystem.s_Triggered.Connect<ObstacleSystem, &ObstacleSystem::OnTrigger>(&m_obstacleSystem);

	m_golfballSystem.s_SunkBall.Connect<GameScene, &GameScene::OnSunkBall>(this);

	// Initialize GUI
	std::unique_ptr<TimerNumber> timeLabel = std::make_unique<TimerNumber>(m_resourceMgr, m_numbersPNG, 2);
	m_timeLabel = timeLabel.get();
	m_timeLabel->SetTime(10.537f);
	m_timeLabel->GetPosition() = Dim2(0.5f, 0.1f, 0.0f, 0.0f);

	std::unique_ptr<Label> roundLabel = std::make_unique<Label>("ROUND", m_fontRoundLabel, Color::WHITE);
	m_roundLabel = roundLabel.get();
	m_roundLabel->GetPosition() = Dim2(0.0f, 1.0f, -25.0f, -35.0f);

	m_timeLabel->AddChild(std::move(roundLabel));
	m_GUI->AddChild(std::move(timeLabel));

	StartGame();
}

GameScene::~GameScene()
{
	m_resourceMgr.Unload(m_soundBGM);
}

void GameScene::Update(float dt)
{
	if (m_input.IsJustPressed("toggle-debug"))
		m_isDebugOn = !m_isDebugOn;

	// Process movement
	m_obstacleSystem.Update(dt);
	m_physicsSystem.Update2DMovement(dt);
	m_particleSystem.Update(dt);
	m_fake3DSystem.Update(dt);

	// Process game logic
	if (m_state == GameState::PRE_ROUND)
	{
		
	}
	else if (m_state == GameState::MID_ROUND)
	{
		// Tick down current time
		m_timeLeft -= dt;
		if (m_timeLeft < 0.0f)
		{
			Logger::Info("Lost game!");
			LoseGame();
			return;
		}

		// Player input
		m_golfballSystem.Update(dt);

		// make camera hover over player
		const auto& golfPos = m_registry.Get<Transform3D>(m_player).position;
		Vec3 offset = INGAME_CAMERA_OFFSET;
		Transform3D& camTF = m_camera.GetTransform();
		camTF.position = Math::Lerp(camTF.position, golfPos + offset, 0.15f);
		camTF.orientation = Quat::LookAt(golfPos + offset, golfPos);

		// Player-body collisions
		m_physicsSystem.Process2DCollisions({ m_player }, m_course.GetPhysicsBodies());
	}
	else if (m_state == GameState::POST_ROUND)
	{
		EndRound();
	}

	// Update GUI
	m_timeLabel->SetTime(m_timeLeft);
	m_GUI->UpdateGUI(dt);

	m_renderer.SetViewMatrix(m_camera.GetView());
	m_registry.FlushDeleteQueue();
}

void GameScene::Render()
{
	// Render 2D/3D objects
	m_renderer.ClearDepthRasterizer();
	m_renderer.ClearPaintersRasterizer();
	
	//m_renderSystem.Render3DEntities();

	// Custom rendering - I want to cull a lot of the walls
	// that probably aren't in view when the camera is locked
	// to the golf ball
	if (m_state == GameState::MID_ROUND)
	{
		// Draw all sprites
		for (auto [id, sp, tf] : m_registry.AllWith<Sprite, Transform3D>())
			m_renderer.Draw3DTexture(tf.position, tf.scale.LengthSq(), sp.textureHandle);

		// Draw the plane
		//const Transform3D& planeTF = m_registry.Get<Transform3D>(m_grass);
		//const MeshInstance& planeMI = m_registry.Get<MeshInstance>(m_grass);
		//m_renderer.DrawMesh(planeTF.ToMatrix(), planeMI);

		// Draw mesh instances w/ fake 3D, cull if far enough from golf ball
		const Transform2D golfTF = m_registry.Get<Transform2D>(m_player);
		for (auto [id, mi, tf3D, tf2D, f] : m_registry.AllWith<MeshInstance, Transform3D, Transform2D, Fake3D>())
		{
			float dist = tf2D.position.DistanceSq(golfTF.position);
			if (dist < MESH_CULLING_HEURISTIC_DIST)
			{
				m_renderer.DrawMesh(tf3D.ToMatrix(), mi);
			}
		}
	}
	else
		m_renderSystem.Render3DEntities();


	m_renderer.FlushDepthRasterizer();
	m_renderer.FlushPaintersRasterizer();

	// Game logic rendering
	if (m_state == GameState::MID_ROUND)
	{
		// Render UI
		m_golfballSystem.RenderSpeedParticles();
		m_golfballSystem.RenderSwooshParticles(m_player);
		m_golfballSystem.RenderArrow();


		if (m_isDebugOn)
		{
			const Vec2& golfPos = m_registry.Get<Transform2D>(m_player).position;

			Transform2D debugTf;
			debugTf.scale = 40.0f;
			debugTf.position = Vec2(APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT) / 2
				- golfPos * debugTf.scale;

			m_renderer.Set2DView(debugTf);
			m_physicsSystem.RenderAllCollisionShapes();
			m_renderer.Set2DView({});

			m_course.RenderGrid(m_renderer, m_camera);
		}
	}

	// GUI rendering
	m_GUI->RenderGUI(m_renderer);

	if (m_isDebugOn)
	{
		int balls = 0;
		int courseBodies = 0;
		for (const auto& pair : m_registry.AllWith<Golfball>())
			balls++;

		for (const auto& pair : m_registry.AllWith<Tile, Physics2D>())
			courseBodies++;

		m_renderer.DrawTextLine(Vec2(20, 100), "Golfball Entities: " + std::to_string(balls), Color::WHITE);
		m_renderer.DrawTextLine(Vec2(20, 80), "Physics Bodies: " + std::to_string(courseBodies), Color::WHITE);
		m_renderer.DrawTextLine(Vec2(20, 60), "Total Entities: " + std::to_string(m_registry.Count()), Color::WHITE);
		
		m_renderer.DrawTextLine(Vec2(20, 160), "Rendered Tris: " + std::to_string(m_renderer.GetMetrics().renderedMeshes), Color::WHITE);
		m_renderer.DrawTextLine(Vec2(20, 140), "Clipped Tris: " + std::to_string(m_renderer.GetMetrics().clippedTriangles), Color::WHITE);
		m_renderer.DrawTextLine(Vec2(20, 120), "Rasterized Tris: " + std::to_string(m_renderer.GetMetrics().rasterizedTriangles), Color::WHITE);
		m_renderer.ResetMetrics();

		m_renderer.DrawTextLine(Vec2(20, 500), "Time left: " + std::to_string(m_timeLeft), Color::WHITE);
		m_renderer.DrawTextLine(Vec2(20, 480), "Current round: " + std::to_string(m_curRound), Color::WHITE);
		m_renderer.DrawTextLine(Vec2(20, 460), "Room parts: " + std::to_string(m_roundPartCount), Color::WHITE);
	}
}

void GameScene::StartGame()
{
	m_course.LoadTemplate("res/courses/course0.txt");
	m_resourceMgr.Get<Audio>(m_soundBGM).Play();

	m_curRound = 1;
	m_roundPartCount = START_PART_COUNT;
	m_timeLeft = START_TIME;

	StartRound();
}

void GameScene::OnSunkBall(Entity ball)
{
	Logger::Info("Sunk ball!");
	m_state = GameState::POST_ROUND;
}

void GameScene::LoseGame()
{
	s_EndedGame.Emit(m_curRound);
}

void GameScene::EndRound()
{
	// Below code creates a transition animation
	// that starts the next round once done
	Callback<void> nextRoundCallback;
	nextRoundCallback.Bind<GameScene, &GameScene::PrepareNextRound>(this);

	// Tween camera pos, start next round once done
	m_tweenMgr.CreateTween(
		m_camera.GetTransform().position,
		POSTROUND_CAM_POS,
		m_camera.GetTransform().position,
		ROUND_TRANS_TIME,
		EasingMode::CUBIC, EasingType::OUT,
		nextRoundCallback
	);

	// Tween camera rotation to be looking at the end of the course
	Vec3 endPoint = m_camera.GetTransform().position - INGAME_CAMERA_OFFSET;
	m_tweenMgr.CreateTween(
		m_camera.GetTransform().orientation,
		Quat::LookAt(POSTROUND_CAM_POS, endPoint),
		m_camera.GetTransform().orientation,
		ROUND_TRANS_TIME,
		EasingMode::CUBIC, EasingType::OUT
	);

	// Add time to the current time left
	float newTimeLeft = std::ceil(m_timeLeft + ROUND_ADD_TIME);
	m_tweenMgr.CreateTween(
		m_timeLeft,
		newTimeLeft,
		m_timeLeft,
		3.0f,
		EasingMode::CUBIC, EasingType::IN
	);
	FocusTimeLabel();
	m_resourceMgr.Get<Audio>(m_soundCharge).Play();

	m_state = GameState::PRE_ROUND;
}

void GameScene::StartRound()
{
	m_player = m_golfballSystem.CreateGolfball();

	m_course.GenerateCourse(m_roundPartCount);

	Transform2D& playerTF = m_registry.Get<Transform2D>(m_player);
	playerTF.position = m_course.GetGolfStartPos();

	m_resourceMgr.Get<Audio>(m_soundWhistle).Play();
	m_roundLabel->GetText() = "ROUND " + std::to_string(m_curRound);

	m_state = GameState::MID_ROUND;
}

void GameScene::PrepareNextRound()
{
	m_registry.QueueDelete(m_player);
	m_course.ClearCourse();
	m_registry.FlushDeleteQueue();

	m_curRound++;
	if (m_curRound % ADD_PART_EVERY == 0)
		m_roundPartCount++;

	StartRound();
}

void GameScene::FocusTimeLabel()
{
	Callback<void> unfocusCB;
	unfocusCB.Bind<GameScene, &GameScene::UnfocusTimeLabel>(this);

	m_tweenMgr.CreateTween(
		Vec2(0.5f, 0.1f), Vec2(0.5f, 0.5f),
		m_timeLabel->GetPosition().relative,
		3.5f, EasingMode::ELASTIC, EasingType::OUT,
		unfocusCB
	);
}

void GameScene::UnfocusTimeLabel()
{
	m_tweenMgr.CreateTween(
		Vec2(0.5f, 0.5f), Vec2(0.5f, 0.1f),
		m_timeLabel->GetPosition().relative,
		0.5f, EasingMode::CUBIC, EasingType::OUT
	);
}
