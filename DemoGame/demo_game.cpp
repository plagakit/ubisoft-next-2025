#include <engine.h>

#include <iostream>

class DemoGame : public Application 
{
private:
	float x, y;
	float count = 0.0f;
	RID f1, f2;
	RID fishSprite;

	MovementSystem m_sysMovement{ m_registry };
	ParticleSystem m_sysParticle{ m_registry };

public:
	void Init()
	{
		f1 = m_rm->Load<Font>("MONOSPACE_8x13");
		f2 = m_rm->Load<Font>("MONOSPACE_9x15");
		fishSprite = m_rm->Load<Texture>("./res/sprites/fish.png");

		int a = 1000000;
		m_registry.ReserveEntities(a);
		m_registry.RegisterComponentType<Transform2D>(a);
		m_registry.RegisterComponentType<Transform3D>();
		m_registry.RegisterComponentType<Sprite>(a);
		m_registry.RegisterComponentType<Particle>(a);
	}

	void Shutdown()
	{
		
	}

	void Update(float dt)
	{
		for (int i = 0; i < 100; i++)
		{
			Entity id = m_registry.CreateEntity();

			Transform2D tf;
			tf.position = Vec2(APP_VIRTUAL_WIDTH / 2.0f, APP_VIRTUAL_HEIGHT * 0.25f);
			Vec2 dir = Math::RandDirection(); dir.y = dir.y < 0 ? -dir.y : dir.y;; dir.y *= 1.2f;
			tf.velocity = dir * 20.0f;
			tf.acceleration.y = -0.5f;

			Sprite spr;
			spr.textureHandle = fishSprite;

			Particle p; p.lifetime = FRAND_RANGE(1.8f, 2.2f);

			m_registry.Add<Transform2D>(id, tf);
			m_registry.Add<Sprite>(id, spr);
			m_registry.Add<Particle>(id, p);
		}

		m_sysMovement.Update();
		m_sysParticle.Update(dt);

		m_registry.FlushDeleteQueue();
	}

	void Render()
	{
		m_renderer->DrawFilledRect(0.0f, 0.0f, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT);
		m_renderer->DrawLine(0, 0, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT, Color::BLUE);

		static float a = 0.0f;
		const float r = 1.0f;
		if (Input::IsPressed("mouse-left"))
			a += 0.1f;
		for (int i = 0; i < 20; i++)
		{

			const float sx = x + 200 + sinf(a + i * 0.1f) * 60.0f;
			const float sy = y + 200 + cosf(a + i * 0.1f) * 60.0f;
			const float ex = x + 700 - sinf(a + i * 0.1f) * 60.0f;
			const float ey = y + 700 - cosf(a + i * 0.1f) * 60.0f;
			m_renderer->DrawLine(sx, sy, ex, ey, Color::RED);
		}

		m_renderer->DrawTextLine(100, 50, "Default Font", Color::BLACK);
		m_renderer->DrawTextLine(100, 100, "Monospace 8x13", Color::BLACK, f1);
		m_renderer->DrawTextLine(100, 100 + m_rm->Get<Font>(f2).GetFontHeight(), "Monospace 9x15", Color::BLACK, f2);

		auto testStr1 = "Hello World!";
		auto testStr2 = "Goodbye World!";
		m_renderer->DrawTextLine(200, 50, testStr1, Color::RED, f2);
		m_renderer->DrawTextLine(200 + m_rm->Get<Font>(f2).GetFontLength(testStr1), 50, testStr2, Color::BLUE, f2);

		int count = 0;
		for (auto [id, tf, sp] : m_registry.AllWith<Transform2D, Sprite>())
		{
			count++;
			m_renderer->DrawTexture(tf.position.x, tf.position.y, sp.textureHandle);
		}

		m_renderer->DrawTextLine(50, 50, std::to_string(count).c_str(), Color::BLUE);

	}
};

REGISTER_GAME(DemoGame)
