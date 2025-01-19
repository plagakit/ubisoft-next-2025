#include "game_scene.h"

#include "components/fake_3d.h"
#include "components/obstacle.h"

GameScene::GameScene(Application& game) :
	Scene(game),
	m_physicsSystem(m_registry, m_resourceMgr, m_renderer),
	m_renderSystem(m_registry, m_renderer),
	m_particleSystem(m_registry),
	m_camera(m_input),

	m_course(m_registry, m_resourceMgr),
	m_fake3DSystem(m_registry),
	m_golfballSystem(m_registry, m_renderer, m_resourceMgr, m_input),

	m_isDebugOn(true)
{
	// Load inputs
	m_input.CreateAction("toggle-debug");
	m_input.AddEvent<InputEventVirtual>("toggle-debug", 'T');

	// Load resources
	m_treePNG = m_resourceMgr.Load<Texture>("res/sprites/tree.png");
	m_planeOBJ = m_resourceMgr.Load<Mesh>("res/models/plane.obj");

	auto [bgmRID, bgm] = m_resourceMgr.LoadAndGet<Audio>("res/audio/bgm.wav");
	m_bgmWAV = bgmRID;
	//bgm.Play();
	
	// Initialize renderer
	m_renderer.SetProjectionMatrix(m_camera.GetProjection());
	m_renderer.SetClearColor(Color(0.765f, 0.925f, 1.0f));

	m_cameraOffset = Vec3(0.0f, 20.0f, -2.0f);
	m_camera.GetTransform().position = m_cameraOffset;
	m_camera.GetTransform().orientation = Quat::LookAt(m_cameraOffset, Vec3::ZERO);

	//m_camera.GetTransform().orientation = Quat::LookAt()

	// Initialize ECS
	m_registry.ReserveEntities(1000);
	m_physicsSystem.Register2DMovement(1000);
	m_physicsSystem.Register3DMovement(1000);
	m_physicsSystem.Register2DPhysics(100);
	m_renderSystem.Register3DComponents(1000);
	m_particleSystem.RegisterAllRequiredComponents(1000);
	m_golfballSystem.RegisterAllRequiredComponents(1);
	m_fake3DSystem.RegisterAllRequiredComponents(100);

	// Create golfball
	m_player = m_golfballSystem.CreateGolfball();

	// Create walls
	m_course.CreateWall(0, 0);
	m_course.LoadCourse("res/courses/course0.txt");

	// Create plane
	{
		Entity p = m_registry.CreateEntity();
		Transform3D tf; tf.scale = Vec3(100.0f, 1.0f, 100.0f);
		MeshInstance m = { m_planeOBJ, Color(0.15f, 0.6f, 0.0f), ShadingMode::FILLED};
		m_registry.Add<Transform3D>(p, tf);
		m_registry.Add<MeshInstance>(p, m);
	}

	// Create trees
	for (int i = 0; i < 100; i++)
	{
		float x = FRAND_RANGE(-100.0f, 100.0f);
		float z = FRAND_RANGE(-100.0f, 100.0f);
		Entity tree = m_registry.CreateEntity();
		Transform3D tf; 
		tf.position = Vec3(x, 5.0f, z);
		tf.scale = Vec3(1.0f, 10.0f, 1.0f);
		Sprite s; s.textureHandle = m_treePNG;
		m_registry.Add<Transform3D>(tree, tf);
		m_registry.Add<Sprite>(tree, s);
	}

	// Connect signals
	m_physicsSystem.s_Collided.Connect<GolfballSystem, &GolfballSystem::OnCollision>(&m_golfballSystem);
}


GameScene::~GameScene()
{
	m_resourceMgr.Unload(m_bgmWAV);
}

void GameScene::Update(float dt)
{
	if (m_input.IsJustPressed("toggle-debug"))
		m_isDebugOn = !m_isDebugOn;

	m_physicsSystem.Update2DMovement(dt);
	m_physicsSystem.Process2DCollisions({ m_player }, m_walls);

	m_particleSystem.Update(dt);

	m_golfballSystem.Update(dt);

	if (m_input.IsJustPressed("space"))
	{
		Vec3& golfPos = m_registry.Get<Transform3D>(m_player).position;
		Vec3& pos = m_camera.GetTransform().position;
		Quat& orn = m_camera.GetTransform().orientation;

		m_tweenMgr.CreateTween<Quat>(
			orn, Quat::LookAt(pos, golfPos),
			orn, 0.3f, EasingMode::CUBIC, EasingType::IN_OUT
		);
		/*m_tweenMgr.CreateTween<Vec3>(
			pos, golfPos + Vec3(0.0f, 3.0f, -3.0f),
			pos, 0.3f, EasingMode::CUBIC, EasingType::IN_OUT
		);*/
	}

	m_fake3DSystem.Update(dt);

	if (m_isDebugOn)
	{
		m_camera.Update(dt);
	}
	else
	{
		const auto& golfPos = m_registry.Get<Transform3D>(m_player).position;
		m_camera.GetTransform().position = Math::Lerp(
			m_camera.GetTransform().position,
			golfPos + m_cameraOffset,
			0.15f
		);
	}

	const auto& golfPos = m_registry.Get<Transform2D>(m_player).position;
	//Logger::Info("%s", golfPos.ToString().c_str());


	m_renderer.SetViewMatrix(m_camera.GetView());
}

void GameScene::Render()
{
	// Render 2D/3D objects
	m_renderer.ClearDepthRasterizer();
	m_renderer.ClearPaintersRasterizer();
	
	m_renderSystem.Render3DEntities();

	if (m_isDebugOn)
		DrawGrid();

	m_renderer.FlushDepthRasterizer();
	m_renderer.FlushPaintersRasterizer();

	if (m_isDebugOn)
	{
		const Vec2& golfPos = m_registry.Get<Transform2D>(m_player).position;

		Transform2D debugTf;
		debugTf.scale = 40.0f;
		debugTf.position = Vec2(APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT) / 2
					- golfPos * debugTf.scale;

		m_renderer.Set2DView(debugTf);
		m_physicsSystem.RenderAllCollisionShapes();
		m_renderer.Set2DView({});;
	}

	// Render UI
	m_golfballSystem.Render();
}



void GameScene::DrawGrid()
{
	Color GRID_COLOR = Color(0.1f, 0.4f, 0.0f);
	constexpr int GRIDLINE_COUNT = 40;
	constexpr float GRIDLINE_SPACING = 4.0f;
	Vec3 camPos = m_camera.GetTransform().position;
	float xOffset = camPos.x - fmodf(camPos.x, GRIDLINE_SPACING);
	float zOffset = camPos.z - fmodf(camPos.z, GRIDLINE_SPACING);

	for (int i = 0; i < GRIDLINE_COUNT; i++)
	{
		for (int j = 0; j < GRIDLINE_COUNT; j++)
		{
			float x = (i - GRIDLINE_COUNT / 2) * GRIDLINE_SPACING + xOffset + GRIDLINE_SPACING * 0.5f;
			float z = (j - GRIDLINE_COUNT / 2) * GRIDLINE_SPACING + zOffset + GRIDLINE_SPACING * 0.5f;

			m_renderer.Draw3DLine(Vec3(x, 0.01f, zOffset), Vec3(x, 0.01f, z), GRID_COLOR);
			m_renderer.Draw3DLine(Vec3(xOffset, 0.01f, z), Vec3(x, 0.01f, z), GRID_COLOR);
		}
	}
}
