#include "game_scene.h"

#include "components/fake_3d.h"
#include "components/golfball.h"
#include "components/obstacle.h"

const Vec3 GameScene::INGAME_CAMERA_OFFSET = Vec3(0.0f, 20.0f, -4.0f);

GameScene::GameScene(Application& game) :
	Scene(game),
	m_physicsSystem(m_registry, m_resourceMgr, m_renderer),
	m_renderSystem(m_registry, m_renderer),
	m_particleSystem(m_registry),
	m_camera(m_input),

	m_course(m_registry, m_resourceMgr),
	m_fake3DSystem(m_registry),
	m_golfballSystem(m_registry, m_renderer, m_resourceMgr, m_input, m_tweenMgr),
	m_obstacleSystem(m_registry),

	m_isDebugOn(true)
{
	// Load inputs
	m_input.CreateAction("toggle-debug");
	m_input.AddEvent<InputEventVirtual>("toggle-debug", 'T');

	// Load resources
	m_treePNG = m_resourceMgr.Load<Texture>("res/sprites/tree.png");
	m_planeOBJ = m_resourceMgr.Load<Mesh>("res/models/plane.obj");

	auto [bgmRID, bgm] = m_resourceMgr.LoadAndGet<Audio>("res/audio/bgm.wav");
	m_soundBGM = bgmRID;
	//bgm.Play();
	m_soundWhistle = m_resourceMgr.Load<Audio>("res/audio/whistle.wav");
	
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

	// Create plane
	{
		m_grass = m_registry.CreateEntity();
		Transform3D tf; tf.scale = Vec3(600.0f, 1.0f, 600.0f);
		MeshInstance m = { m_planeOBJ, Color(0.15f, 0.6f, 0.0f), ShadingMode::FILLED};
		m_registry.Add<Transform3D>(m_grass, tf);
		m_registry.Add<MeshInstance>(m_grass, m);
	}

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
	m_physicsSystem.Update2DMovement(dt);
	m_particleSystem.Update(dt);
	m_fake3DSystem.Update(dt);

	// Process game logic
	if (m_state == GameState::GENERATING)
	{
		
	}
	else if (m_state == GameState::RUNNING_COURSE)
	{
		// Player input
		m_golfballSystem.Update(dt);

		//Transform2D& golfTF2D = m_registry.Get<Transform2D>(m_player);
		//constexpr float MIN_SPD = 5.0f;
		//constexpr float MAX_SPD = 20.0f;
		//float speed = Math::Clamp(MIN_SPD, MAX_SPD, golfTF2D.velocity.Length());

		const auto& golfPos = m_registry.Get<Transform3D>(m_player).position;
		Vec3 offset = INGAME_CAMERA_OFFSET;
		//offset = offset * Math::Map(MIN_SPD, MAX_SPD, 1.0f, 0.66f, speed);

		Transform3D& camTF = m_camera.GetTransform();
		camTF.position = Math::Lerp(camTF.position, golfPos + offset, 0.15f);
		camTF.orientation = Quat::LookAt(golfPos + offset, golfPos);

		// FOV effect
		//float newFOV = Math::Clamp(60.0f, 90.0f, Math::Map(MIN_SPD, MAX_SPD, 60.0f, 90.0f, speed));
		//m_camera.SetFOV(Math::Lerp(m_camera.GetFOV(), newFOV, 0.1f));
		//m_renderer.SetProjectionMatrix(m_camera.GetProjection());

		// Player-body collisions
		m_physicsSystem.Process2DCollisions({ m_player }, m_course.GetPhysicsBodies());
	}
	else if (m_state == GameState::END_ROUND)
	{
		EndRound();
	}

	if (m_isDebugOn)
	{
		m_camera.Update(dt);
	}
	m_renderer.SetViewMatrix(m_camera.GetView());
	m_registry.FlushDeleteQueue();
}

void GameScene::Render()
{
	// Render 2D/3D objects
	m_renderer.ClearDepthRasterizer();
	m_renderer.ClearPaintersRasterizer();
	
	m_renderSystem.Render3DEntities();

	m_renderer.FlushDepthRasterizer();
	m_renderer.FlushPaintersRasterizer();

	// Game logic rendering
	if (m_state == GameState::GENERATING)
	{

	}
	else if (m_state == GameState::RUNNING_COURSE)
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


	if (m_isDebugOn)
	{
		int balls = 0;
		int courseBodies = 0;
		for (const auto& pair : m_registry.AllWith<Golfball>())
			balls++;

		for (const auto& pair : m_registry.AllWith<Tile, Physics2D>())
			courseBodies++;

		m_renderer.DrawTextLine(Vec2(20, 100), std::to_string(balls), Color::WHITE);
		m_renderer.DrawTextLine(Vec2(20, 80), std::to_string(courseBodies), Color::WHITE);
	}
}

void GameScene::StartGame()
{
	m_course.LoadTemplate("res/courses/course0.txt");
	//m_resourceMgr.Get<Audio>(m_soundBGM).Play();

	m_curRound = 1;
	m_roundPartCount = START_PART_COUNT;

	NextRound();
}

void GameScene::OnSunkBall(Entity ball)
{
	Logger::Info("Sunk ball!");
	m_state = END_ROUND;
}

void GameScene::NextRound()
{
	m_player = m_golfballSystem.CreateGolfball();

	m_course.GenerateCourse(m_roundPartCount);

	Transform2D& playerTF = m_registry.Get<Transform2D>(m_player);
	playerTF.position = m_course.GetGolfStartPos();

	m_state = GameState::RUNNING_COURSE;
}

void GameScene::EndRound()
{
	m_registry.QueueDelete(m_player);
	m_course.ClearCourse();
	m_registry.FlushDeleteQueue();

	m_state = GameState::GENERATING;
}
