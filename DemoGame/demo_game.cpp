#include <engine.h>

#include <iostream>

class DemoGame : public Application 
{
private:
	float x = 0.0f;
	float y = 0.0f;

public:
	void Init()
	{
		m_registry.ReserveEntities(1500);
		m_registry.RegisterComponentType<Transform>(1500);
		m_registry.RegisterComponentType<TestComponent>(1500);

		Entity e1 = m_registry.CreateEntity();
		Entity e2 = m_registry.CreateEntity();

		Transform c1 = { Vec3(1, 2, 3) };
		TestComponent c2 = { 23 };

		ASSERT_ERROR(m_registry.Exists(e1), "%d does not exist", e1);
		ASSERT_ERROR(m_registry.Exists(e2), "%d does not exist", e2);
		ASSERT_ERROR(m_registry.Count() == 2, "Wrong count: %d", m_registry.Count());

		for (int i = 0; i < 1000; i++)
		{
			Entity e = m_registry.CreateEntity();
			TestComponent a = { e * 2 };
			m_registry.Add<TestComponent>(e, a);
		}

		for (int i = 300; i < 700; i++) 
			m_registry.QueueDelete(i);
		m_registry.FlushDeleteQueue();

		Logger::Info("Count %d", m_registry.Count());
		ASSERT_ERROR(m_registry.Has<TestComponent>(299), "Entity 299 doesn't have TestComponent");
		ASSERT_ERROR(!m_registry.Has<TestComponent>(300), "Entity 300 has TestComponent.");

		for (int i = 0; i < 700 - 300; i++)
		{
			Entity id = m_registry.CreateEntity();
			TestComponent a = { id * 100 };
			m_registry.Add<TestComponent>(id, a);
		}

		Logger::Info("Count: %d:", m_registry.Count());

		for (auto [id, tc, tf] : m_registry.AllWith<TestComponent, Transform>())
			Logger::Info("%d %d", id, tc.a);

		int count = 0;
		for (auto [id, tc] : m_registry.AllWith<TestComponent>())
			count++;
		Logger::Info("Count w/ TestComponent: %d", count);
	}

	void Shutdown()
	{
		
	}

	void Update(float dt)
	{
		//if (Input::IsJustPressed("mouse-right"))
		//	ASSERT_WARN(false, "%d", 24123);

		// TODO: optimize Input
		x += Input::GetAxis("left", "right");
		y += Input::GetAxis("down", "up");

		for (int i = 0; i < 100; i++)
		{
			x += Input::GetAxis("left", "right");
		}
	}

	void Render()
	{
		Renderer::DrawFilledRect(0.0f, 0.0f, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT);
		Renderer::DrawLine(0, 0, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT, Color::BLUE);

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
			Renderer::DrawLine(sx, sy, ex, ey, Color::RED);
		}

		Font f1 = { Font::Type::MONOSPACE_8x13 };
		Font f2 = { Font::Type::MONOSPACE_9x15 };

		Renderer::DrawTextLine(100, 50, "Default Font", Color::BLACK);
		Renderer::DrawTextLine(100, 100, "Monospace 8x13", Color::BLACK, f1);
		Renderer::DrawTextLine(100, 100 + f2.GetFontHeight(), "Monospace 9x15", Color::BLACK, f2);

		auto testStr1 = "Hello World!";
		auto testStr2 = "Goodbye World!";
		Renderer::DrawTextLine(200, 50, testStr1, Color::RED, f2);
		Renderer::DrawTextLine(200 + f2.GetFontLength(testStr1), 50, testStr2, Color::BLUE, f2);
	}
};

REGISTER_GAME(DemoGame)
