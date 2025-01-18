#include "game_scene.h"

GameScene::GameScene(Application& game) :
	Scene(game),
	m_physicsSystem(m_registry, m_resourceMgr, m_renderer),
	m_renderSystem(m_registry, m_renderer),
	m_particleSystem(m_registry),
	m_camera(m_input),
	m_golfballSystem(m_registry, m_renderer, m_resourceMgr, m_input)
{
	// Load resources
	m_treePNG = m_resourceMgr.Load<Texture>("res/sprites/tree.png");
	m_planeOBJ = m_resourceMgr.Load<Mesh>("res/models/plane.obj");

	auto [bgmRID, bgm] = m_resourceMgr.LoadAndGet<Audio>("res/audio/bgm.wav");
	m_bgmWAV = bgmRID;
	//bgm.Play();
	
	// Initialize renderer
	m_renderer.SetProjectionMatrix(m_camera.GetProjection());
	m_renderer.SetClearColor(Color(0.765f, 0.925f, 1.0f));
	m_camera.GetTransform().position = Vec3(0.0f, 3.0f, -2.0f);

	//m_camera.GetTransform().orientation = Quat::LookAt()

	// Initialize ECS
	m_registry.ReserveEntities(1000);
	m_physicsSystem.Register2DMovement(1000);
	m_physicsSystem.Register3DMovement(1000);
	m_physicsSystem.Register2DPhysics(100);
	m_renderSystem.Register3DComponents(1000);
	m_particleSystem.RegisterAllRequiredComponents(1000);
	m_golfballSystem.RegisterAllRequiredComponents(1);

	// Create golfball
	m_player = m_golfballSystem.CreatePlayer();

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

}

GameScene::~GameScene()
{
	m_resourceMgr.Unload(m_bgmWAV);
}

void GameScene::Update(float dt)
{
	m_physicsSystem.Update3DMovement(dt);
	m_particleSystem.Update(dt);

	m_golfballSystem.Update(dt);

	if (m_input.IsJustPressed("space"))
	{
		Vec3& golfPos = m_registry.Get<Transform3D>(1).position;
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

	// Fake 3D effect
	for (auto [id, tf3D, tf2D] : m_registry.AllWith<Transform3D, Transform2D>())
	{
		tf3D.position = Vec3(
			tf2D.position.x,
			0.0f,
			tf2D.position.y
		);
	}

	m_camera.Update(dt);
	m_renderer.SetViewMatrix(m_camera.GetView());
}

void GameScene::Render()
{
	// Render 2D/3D objects
	m_renderer.ClearDepthRasterizer();
	m_renderer.ClearPaintersRasterizer();
	
	m_renderSystem.Render3DEntities();

	// Draw grid
	{
		Color GRID_COLOR = Color(0.1f, 0.4f, 0.0f);
		constexpr int GRIDLINE_COUNT = 40;
		constexpr float GRIDLINE_SPACING = 5.0f;
		Vec3 camPos = m_camera.GetTransform().position;
		float xOffset = camPos.x - fmodf(camPos.x, GRIDLINE_SPACING);
		float zOffset = camPos.z - fmodf(camPos.z, GRIDLINE_SPACING);

		for (int i = 0; i < GRIDLINE_COUNT; i++)
		{
			for (int j = 0; j < GRIDLINE_COUNT; j++)
			{
				float x = (i - GRIDLINE_COUNT / 2) * GRIDLINE_SPACING + xOffset;
				float z = (j - GRIDLINE_COUNT / 2) * GRIDLINE_SPACING + zOffset;

				m_renderer.Draw3DLine(Vec3(x, 0.01f, zOffset), Vec3(x, 0.01f, z), GRID_COLOR);
				m_renderer.Draw3DLine(Vec3(xOffset, 0.01f, z), Vec3(x, 0.01f, z), GRID_COLOR);
			}
		}
	}

	m_renderer.FlushDepthRasterizer();
	m_renderer.FlushPaintersRasterizer();

	// Render UI
	m_golfballSystem.Render();
}
