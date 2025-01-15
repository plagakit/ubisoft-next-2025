#include <engine.h>

#include <iostream>

int asd = 0;
int two(int a, int b) { asd++; return a + b; }

class DemoGame : public Application 
{
private:
	float x, y;
	float count = 0.0f;
	RID f1, f2;
	RID suzanne, cube;
	RID fishSprite;
	Camera camera;
	

public:

	void TimerFireTest(Entity id)
	{
		Logger::Info("Timer w/ ID %d fired!", id);
	}
	
	void Init()
	{
		m_renderer->SetViewMatrix(Mat4::IDENTITY);
		m_renderer->SetProjectionMatrix(camera.GetProjection());
		//m_renderer->SetClearColor(Color::BLACK);

		f1 = m_rm->Load<Font>("MONOSPACE_8x13");
		f2 = m_rm->Load<Font>("MONOSPACE_9x15");
		fishSprite = m_rm->Load<Texture>("./res/sprites/fish.png");
		suzanne = m_rm->Load<Mesh>("res/models/suzanne.obj");
		cube = m_rm->Load<Mesh>("res/models/cube.obj");

		int a = 10000;
		m_registry.ReserveEntities(a);
		m_registry.RegisterComponentType<Transform2D>(a);
		m_registry.RegisterComponentType<Transform3D>(a);
		m_registry.RegisterComponentType<Sprite>(a);
		m_registry.RegisterComponentType<Particle>(a);
		m_registry.RegisterComponentType<MeshInstance>(a);
		m_registry.RegisterComponentType<Timer>();

		//const Mesh& suzanneMesh = m_rm->Get<Mesh>(suzanne);
		//Logger::Info("%d %d %d",
		//	suzanneMesh.GetVertexBuffer().size(),
		//	suzanneMesh.GetNormalBuffer().size(),
		//	suzanneMesh.GetIndexBuffer().size());

		//Entity c = m_registry.CreateEntity();
		//Transform3D tf; tf.position = Vec3::FORWARD * 5; tf.angVelocity = Quat::FromAxisAngle(Vec3::UP, 0.02f);
		//MeshInstance mi; mi.meshHandle = suzanne; mi.mode = MeshInstance::Mode::FILLED;
		//m_registry.Add<Transform3D>(c, tf);
		//m_registry.Add<MeshInstance>(c, mi);

		for (int i = 0; i < 0; i++)
		{
			Entity id = m_registry.CreateEntity();

			//Transform3D tf; 
			//tf.position = Vec3::FORWARD * 10;
			//tf.position.x = (i % 10 - 5) * 3.0f;
			////tf.position.y = (i / 10 - 5) * 3.0f;
			//tf.angVelocity = Quat::FromAxisAngle(Vec3::UP, FRAND_RANGE(-0.1f, 0.1f));
			//tf.scale = Vec3::ONE * 2.0f;

			Transform3D tf;
			tf.position = Vec3::FORWARD * 10.0f + Vec3::DOWN * 2.0f;
			tf.velocity = Vec3::UP * 100.0f;

			//MeshInstance mi = { suzanne, Color::GREEN, ShadingMode::FILLED };

			m_registry.Add<Transform3D>(id, tf);
			//m_registry.Add<MeshInstance>(id, mi);
			m_registry.Add<Sprite>(id, { fishSprite });
		}

	}

	void Shutdown()
	{
		
	}

	void Update(float dt)
	{
		// Normally you don't define 
		MovementSystem m_sysMovement{ m_registry };
		ParticleSystem m_sysParticle{ m_registry };
		TimerSystem m_sysTimer{ m_registry };

		auto& camtf = camera.GetTransform();
		camtf.position += Vec3::FORWARD * Input::GetAxis("down", "up") * dt;
		m_renderer->SetViewMatrix(camera.GetView());

		for (int i = 0; i < 0; i++)
		{
			Entity id = m_registry.CreateEntity();

			Transform2D tf;
			tf.position = Vec2(APP_VIRTUAL_WIDTH / 2.0f, APP_VIRTUAL_HEIGHT * 0.25f);
			Vec2 dir = Math::RandDirection(); dir.y = dir.y < 0 ? -dir.y : dir.y; dir.y *= 3.0f;
			tf.velocity = dir * 200.0f;
			tf.acceleration.y = -200.0f;
			tf.angVelocity = FRAND_RANGE(-PI, PI) * 2.0f;

			Sprite spr;
			spr.textureHandle = fishSprite;

			Particle p; p.lifetime = FRAND_RANGE(5.0f, 6.0f);

			m_registry.Add<Transform2D>(id, tf);
			m_registry.Add<Sprite>(id, spr);
			m_registry.Add<Particle>(id, p);
		}

		for (int i = 0; i < 10; i++)
		{
			Entity id = m_registry.CreateEntity();

			Transform3D tf;
			tf.position = Vec3::FORWARD * 10.0f + Vec3::DOWN * 2.0f;
			tf.scale = { 2.0f, 2.0f, 2.0f };
			Vec2 dir = Math::RandDirection(); dir *= 10.0f;
			tf.velocity = Vec3(dir.x, 30.0f, dir.y);
			tf.acceleration.y = -98.0f;
			//tf.angVelocity = FRAND_RANGE(-PI, PI) * 2.0f;

			Sprite spr;
			spr.textureHandle = fishSprite;

			Particle p; p.lifetime = FRAND_RANGE(5.0f, 6.0f);

			m_registry.Add<Transform3D>(id, tf);
			m_registry.Add<Sprite>(id, spr);
			m_registry.Add<Particle>(id, p);
		}

		m_sysMovement.Update(dt);
		m_sysParticle.Update(dt);
		m_sysTimer.Update(dt);

		m_registry.FlushDeleteQueue();
	}

	void Render()
	{
		RenderSystem m_sysRender{ m_registry, *m_renderer };

		//m_renderer->DrawFilledRect(0.0f, 0.0f, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT);
		//m_renderer->DrawLine(0, 0, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT, Color::BLUE);

		//static float a = 0.0f;
		//const float r = 1.0f;
		//if (Input::IsPressed("mouse-left"))
		//	a += 0.1f;
		//for (int i = 0; i < 20; i++)
		//{

		//	const float sx = x + 200 + sinf(a + i * 0.1f) * 60.0f;
		//	const float sy = y + 200 + cosf(a + i * 0.1f) * 60.0f;
		//	const float ex = x + 700 - sinf(a + i * 0.1f) * 60.0f;
		//	const float ey = y + 700 - cosf(a + i * 0.1f) * 60.0f;
		//	m_renderer->DrawLine(sx, sy, ex, ey, Color::RED);
		//}

		//m_renderer->DrawTextLine(100, 50, "Default Font", Color::BLACK);
		//m_renderer->DrawTextLine(100, 100, "Monospace 8x13", Color::BLACK, f1);
		//m_renderer->DrawTextLine(100, 100 + m_rm->Get<Font>(f2).GetFontHeight(), "Monospace 9x15", Color::BLACK, f2);

		//auto testStr1 = "Hello World!";
		//auto testStr2 = "Goodbye World!";
		//m_renderer->DrawTextLine(200, 50, testStr1, Color::RED, f2);
		//m_renderer->DrawTextLine(200 + m_rm->Get<Font>(f2).GetFontLength(testStr1), 50, testStr2, Color::BLUE, f2);

		//int count = 0;
		//for (auto [id, sp, tf] : m_registry.AllWith<Sprite, Transform2D>())
		//{
		//	count++;
		//	m_renderer->DrawTexture(tf, sp.textureHandle);
		//}

		//m_renderer->DrawTextLine(50, 50, std::to_string(count).c_str(), Color::BLUE);

		m_renderer->ClearTextureRasterizer();
		m_sysRender.Render3DEntities();
		//m_sysRender.Render2DEntities();
		m_renderer->Flush3DTextures();
	}
};

REGISTER_GAME(DemoGame)
