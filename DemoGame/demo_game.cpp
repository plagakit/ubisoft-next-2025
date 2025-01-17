#include <engine.h>

#include <iostream>

int asd = 0;
int two(int a, int b) { asd++; return a + b; }

class DemoGame : public Application 
{
private:
	float t = 0.0f;
	float x, y;
	float count = 0.0f;
	RID f1, f2;
	RID suzanne, cube;
	RID fishSprite;
	Camera camera;
	
	Entity circle1, circle2;
	RID circleCol;

	RID fishAudio;

	GUIRoot guiRoot;
	Frame* f;

public:

	void CollisionSignalTest(Entity id1, Entity id2)
	{
		//Logger::Info("%d collided w/ %d!", id1, id2);
	}

	void TweenTo()
	{
		Tween<Vec2> tween1 = m_tweenMgr->CreateTween<Vec2>(
			{ 0.0f, 0.0f }, { 1.0f, 0.0f },
			f->GetPosition().relative,
			5.0f, EasingMode::ELASTIC, EasingType::OUT);
		Tween<Vec2> tween2 = m_tweenMgr->CreateTween<Vec2>(
			{ 0.5f, 0.5f }, { 0.2f, 0.75f },
			f->GetSize().relative,
			5.0f, EasingMode::ELASTIC, EasingType::OUT);
	}
	
	void Init()
	{
		auto frame = std::make_unique<Frame>();
		frame->GetPosition() = Dim2(0.25f, 0.25f, 0.0f, 50.0f);
		frame->m_anchor = Vec2(1.0f, 0.0f);
		f = frame.get();
		guiRoot.AddChild(std::move(frame));

		TweenTo();

		m_renderer->SetViewMatrix(Mat4::IDENTITY);
		m_renderer->SetProjectionMatrix(camera.GetProjection());

		f1 = m_resourceMgr->Load<Font>("MONOSPACE_8x13");
		f2 = m_resourceMgr->Load<Font>("MONOSPACE_9x15");
		fishSprite = m_resourceMgr->Load<Texture>("./res/sprites/fish.png");
		suzanne = m_resourceMgr->Load<Mesh>("res/models/suzanne.obj");
		cube = m_resourceMgr->Load<Mesh>("res/models/cube.obj");

		fishAudio = m_resourceMgr->Load<Audio>("res/audio/fish.wav");

		auto [crid, cc] = m_resourceMgr->LoadAndGet<CircleCollider>("");
		circleCol = crid;
		cc.radius = 40.0f;

		int a = 10000;
		m_registry.ReserveEntities(a);
		m_registry.RegisterComponentType<Transform2D>(a);
		m_registry.RegisterComponentType<Transform3D>(a);
		m_registry.RegisterComponentType<Sprite>(a);
		m_registry.RegisterComponentType<Particle>(a);
		m_registry.RegisterComponentType<MeshInstance>(a);
		m_registry.RegisterComponentType<Timer>();
		m_registry.RegisterComponentType<Physics2D>(100);

		//const Mesh& suzanneMesh = m_resourceMgr->Get<Mesh>(suzanne);
		//Logger::Info("%d %d %d",
		//	suzanneMesh.GetVertexBuffer().size(),
		//	suzanneMesh.GetNormalBuffer().size(),
		//	suzanneMesh.GetIndexBuffer().size());

		Entity c = m_registry.CreateEntity();
		Transform3D tf; tf.position = Vec3::FORWARD * 5; tf.angVelocity = Quat::FromAxisAngle(Vec3::UP, 0.02f);
		MeshInstance mi; mi.meshHandle = suzanne; mi.mode = ShadingMode::FILLED;
		m_registry.Add<Transform3D>(c, tf);
		m_registry.Add<MeshInstance>(c, mi);

		// 2d collision test
		circle1 = m_registry.CreateEntity();
		Transform2D tff; tff.position = { 500, 480 }; tff.velocity = Vec2::ONE * 30.0f;
		m_registry.Add<Transform2D>(circle1, tff);
		m_registry.Add<Sprite>(circle1, { fishSprite });
		m_registry.Add<Physics2D>(circle1, { circleCol });

		circle2 = m_registry.CreateEntity();
		Transform2D tff2; tff2.position = { 700, 700 }; tff2.velocity = -Vec2::ONE * 30.0f;
		m_registry.Add<Transform2D>(circle2, tff2);
		m_registry.Add<Sprite>(circle2, { fishSprite });
		m_registry.Add<Physics2D>(circle2, { circleCol });

	}

	void Shutdown()
	{
		
	}

	void Update(float dt)
	{
		t += dt;

		if (Input::IsJustPressed("mouse-left"))
		{
			Audio& audio = m_resourceMgr->Get<Audio>(fishAudio);
			audio.Play();

			Vec2 mousePos = Input::GetMousePos();
			//Logger::Info("Clicked! Mouse pos: %s", mousePos.ToString().c_str());

			Ray3D ray = camera.ProjectRay(mousePos);
			//Logger::Info("Ray: %s %s", ray.origin.ToString().c_str(), ray.direction.ToString().c_str());

			Entity e = m_registry.CreateEntity();
			Transform3D tf; tf.position = ray.origin; 
			tf.velocity = ray.direction;
			Sprite s; s.textureHandle = fishSprite;
			MeshInstance m; m.meshHandle = cube;
			Particle p; p.lifetime = 30.0f;

			m_registry.Add<Transform3D>(e, tf);
			m_registry.Add<MeshInstance>(e, m);
			m_registry.Add<Particle>(e, p);
			m_registry.Add<Sprite>(e, s);
		}

		// Normally you don't define 
		PhysicsSystem s_physics{ m_registry, *m_resourceMgr, *m_renderer };
		ParticleSystem m_sysParticle{ m_registry };
		TimerSystem m_sysTimer{ m_registry };

		s_physics.s_Collided.Connect<DemoGame, &DemoGame::CollisionSignalTest>(this);

		auto& camtf = camera.GetTransform();
		Vec3 move = Vec3::FORWARD * Input::GetAxis("down", "up")
			+ Vec3::RIGHT * Input::GetAxis("left", "right")
			+ Vec3::UP * Input::GetAxis("Q", "E");

		move = camtf.orientation * move;
		camtf.position += move * dt * 3.0f;

		camtf.orientation = Quat::FromEulerAngles(0, Input::GetAxis("J", "L") * 2.0f * dt, 0) * camtf.orientation;
		camtf.orientation *= Quat::FromEulerAngles(0, 0, Input::GetAxis("K", "I") * -2.0f * dt);

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

			Particle p; p.lifetime = FRAND_RANGE(0.5f, 1.0f);

			m_registry.Add<Transform2D>(id, tf);
			m_registry.Add<Sprite>(id, spr);
			m_registry.Add<Particle>(id, p);
		}

		for (int i = 0; i < 10; i++)
		{
			Entity id = m_registry.CreateEntity();

			Transform3D tf;
			tf.position = Vec3::FORWARD * 20.0f + Vec3::DOWN * 3.0f;
			tf.scale = { 2.0f, 2.0f, 2.0f };
			float tt = (t + FRAND_RANGE(-0.2f, 0.2f)) * 6.0f;
			tf.velocity = Vec3(cosf(tt), 2.0f, sinf(tt)) * FRAND_RANGE(20.0f, 30.0f);
			tf.acceleration.y = -98.0f;

			Sprite spr;
			spr.textureHandle = fishSprite;

			Particle p; p.lifetime = FRAND_RANGE(0.5f, 0.7f);

			m_registry.Add<Transform3D>(id, tf);
			m_registry.Add<Sprite>(id, spr);
			m_registry.Add<Particle>(id, p);
		}

		s_physics.Update2DMovement(dt);
		s_physics.Update3DMovement(dt);
		s_physics.ProcessAll2DCollisions(dt);
		m_sysParticle.Update(dt);
		m_sysTimer.Update(dt);

		m_registry.FlushDeleteQueue();

		guiRoot.UpdateGUI(dt);
	}

	void Render()
	{
		RenderSystem m_sysRender{ m_registry, *m_renderer };

		m_renderer->ClearDepthRasterizer();
		m_renderer->ClearPaintersRasterizer();

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

		//int count = 0;
		//for (auto [id, sp, tf] : m_registry.AllWith<Sprite, Transform2D>())
		//{
		//	count++;
		//	m_renderer->DrawTexture(tf, sp.textureHandle);
		//}

		//m_renderer->DrawTextLine(50, 50, std::to_string(count).c_str(), Color::BLUE);

		m_sysRender.Render3DEntities();

		for (int i = 0; i < 100; i++)
		{
			Vec3 pos = Vec3::DOWN * 5.0f;
			m_renderer->Draw3DLine(pos + Vec3::FORWARD * i, pos + Vec3::FORWARD * (i + 1), Color::DARK_GRAY);
		}
		m_renderer->DrawSphere(Vec3::FORWARD * 5.0f, 1.0f, Color::RED);
		m_renderer->DrawSphere(Vec3::FORWARD * 5.0f, 3.0f, Color::BLUE);
		m_renderer->DrawSphere(Vec3::FORWARD * 5.0f, 10.0f, Color::GREEN);
		m_renderer->Draw3DLine(Vec3::FORWARD * 5.0f, Vec3::FORWARD * 5.0 + Vec3::RIGHT, Color::WHITE);
		m_renderer->Draw3DLine(Vec3::FORWARD * 5.0f, Vec3::FORWARD * 5.0 + Vec3::UP, Color::WHITE);
		m_renderer->Draw3DLine(Vec3::FORWARD * 5.0f, Vec3::FORWARD * 5.0 + Vec3::FORWARD, Color::WHITE);

		m_renderer->FlushDepthRasterizer();
		m_renderer->FlushPaintersRasterizer();

		m_sysRender.Render2DEntities();

		/*m_renderer->DrawCircle({ 300, 300 }, 4, Color::RED);
		m_renderer->DrawCircle({ 300, 300 }, 16, Color::RED);
		m_renderer->DrawCircle({ 350, 300 }, 32, Color::RED);
		m_renderer->DrawCircle({ 350, 300 }, 100, Color::RED);
		m_renderer->DrawCircle({ 350, 300 }, 300, Color::RED);*/

		m_renderer->DrawTextLine(100, 50, "Default Font", Color::WHITE);
		m_renderer->DrawTextLine(100, 100, "Monospace 8x13", Color::WHITE, f1);
		m_renderer->DrawTextLine(100, 100 + m_resourceMgr->Get<Font>(f2).GetFontHeight(), "Monospace 9x15", Color::WHITE, f2);

		auto testStr1 = "Hello World!";
		auto testStr2 = "Goodbye World!";
		m_renderer->DrawTextLine(220, 50, testStr1, Color::RED, f2);
		m_renderer->DrawTextLine(220 + m_resourceMgr->Get<Font>(f2).GetFontLength(testStr1), 50, testStr2, Color::BLUE, f2);

		Collider2D& circle = m_resourceMgr->Get<CircleCollider>(circleCol);
		//circle.DebugDraw(*m_renderer, { 400, 300 });

		PhysicsSystem s_physics{ m_registry, *m_resourceMgr, *m_renderer };
		s_physics.RenderAllCollisionShapes();

		guiRoot.RenderGUI(*m_renderer);
	}
};

REGISTER_GAME(DemoGame)
