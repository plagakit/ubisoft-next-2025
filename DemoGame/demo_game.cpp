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
	}

	void Render()
	{
		Renderer::Text(100, 100, "aaaaaaa");
		Renderer::DrawLine(0, 0, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT);
		
		for (int y = 0; y < APP_VIRTUAL_HEIGHT; y++)
		{
			Renderer::DrawLine(0, y, APP_VIRTUAL_WIDTH, y, 1, 1, 1);
		}

		static float a = 0.0f;
		const float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
		a += 0.1f;
		for (int i = 0; i < 100; i++)
		{
		
			const float sx = 200 + sinf(a + i * 0.1f) * 60.0f;
			const float sy = 200 + cosf(a + i * 0.1f) * 60.0f;
			const float ex = 700 - sinf(a + i * 0.1f) * 60.0f;
			const float ey = 700 - cosf(a + i * 0.1f) * 60.0f;
			g = (float)i / 20.0f;
			b = (float)i / 20.0f;
			Renderer::DrawLine(sx, sy, ex, ey);//, r, g, b);
		}
	}


};

REGISTER_GAME(DemoGame)
