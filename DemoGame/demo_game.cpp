#include <engine.h>

#include <iostream>

class DemoGame : public Application 
{
	void Init()
	{
		Vec3 v1 = { 10, 12, 3 };
		Vec3 v2 = { -2, 5.01f, 1 };

		auto s = v1.ToString();
		Logger::Info("%s", s.c_str());
	}

	void Shutdown()
	{
		
	}

	void Update(float deltaTime)
	{
		Vec2 mouse = input.GetMousePos();
		float s = input.GetStrength("left");
		bool a = input.IsPressed("left");
		bool b = input.IsJustPressed("left");

		if (b)
			Logger::Info("%s, %f, %d, %d", mouse.ToString().c_str(), s, a, b);
	}

	void Render()
	{
		Renderer::DrawFilledRect(0.0f, 0.0f, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT);
		Renderer::DrawLine(0, 0, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT, Color::BLUE);

		static float a = 0.0f;
		const float r = 1.0f;
		if (input.IsPressed("mouse-left"))
			a += 0.1f;
		for (int i = 0; i < 20; i++)
		{

			const float sx = 200 + sinf(a + i * 0.1f) * 60.0f;
			const float sy = 200 + cosf(a + i * 0.1f) * 60.0f;
			const float ex = 700 - sinf(a + i * 0.1f) * 60.0f;
			const float ey = 700 - cosf(a + i * 0.1f) * 60.0f;
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
