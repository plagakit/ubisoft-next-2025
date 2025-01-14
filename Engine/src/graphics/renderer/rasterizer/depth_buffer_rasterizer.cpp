#include "pch.h"
#include "depth_buffer_rasterizer.h"

#include "graphics/renderer/renderer.h"
#include "core/app_settings.h"
#include "math/math_utils.h"

#include <App/app.h>
#undef max
#undef min

/*
Resources:
https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/ (this entire series honestly)
*/

DepthBufferRasterizer::DepthBufferRasterizer()
{
	m_colorBuffer.resize(APP_VIRTUAL_WIDTH * APP_VIRTUAL_HEIGHT, Color::WHITE);
	m_depthBuffer.resize(APP_VIRTUAL_WIDTH * APP_VIRTUAL_HEIGHT, 1.0f);
}

void DepthBufferRasterizer::Clear()
{
	std::fill(m_colorBuffer.begin(), m_colorBuffer.end(), Color::BLACK);
	std::fill(m_depthBuffer.begin(), m_depthBuffer.end(), 1.0f);
}

void DepthBufferRasterizer::RasterizeTriangle(const Vec4& a, const Vec4& b, const Vec4& c, const Color& color)
{
	float area = Renderer::Triangle2DArea(a, b, c);
	
	//float fac = (-glm::dot(tri.faceNormal, lightDirection) + 1) * 0.5f;
	//Color darkedCol = DarkenColor(WHITE, fac);

	// Biases for abiding by fill rules
	//int bias0 = IsEdgeTopOrLeft(b, c) ? 0 : -1;
	//int bias1 = IsEdgeTopOrLeft(c, a) ? 0 : -1;
	//int bias2 = IsEdgeTopOrLeft(a, b) ? 0 : -1;

	// Only check the pixels in the triangle's bounding box
	int minX = std::max(0, static_cast<int>(std::floorf(std::min({ a.x, b.x, c.x }))));
	int maxX = std::min(APP_VIRTUAL_WIDTH, static_cast<int>(std::ceilf(std::max({ a.x, b.x, c.x }))));
	int minY = std::max(0, static_cast<int>(std::floorf(std::min({ a.y, b.y, c.y }))));
	int maxY = std::min(APP_VIRTUAL_HEIGHT, static_cast<int>(std::ceilf(std::max({ a.y, b.y, c.y }))));
	//Logger::Info("Min: (%d, %d), Max: (%d, %d)", minX, minY, maxX, maxY);

	// Incremental Pineda rasterization
	Vec4 point = Vec4(static_cast<float>(minX), static_cast<float>(minY), 0.0f, 0.0f);
	float row12 = Renderer::Triangle2DArea(b, c, point);
	float row20 = Renderer::Triangle2DArea(c, a, point);
	float row01 = Renderer::Triangle2DArea(a, b, point);
	
	// https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/
	float A01 = a.y - b.y;
	float A12 = b.y - c.y;
	float A20 = c.y - a.y;
	float B01 = b.x - a.x;
	float B12 = c.x - b.x;
	float B20 = a.x - c.x;

	for (int y = minY; y < maxY; y++)
	{
		float edge01 = row01;
		float edge12 = row12;
		float edge20 = row20;

		for (int x = minX; x < maxX; x++)
		{
			int idx = y * APP_VIRTUAL_WIDTH + x;

			Logger::Info("%f %f %f", edge01, edge12, edge20);

			// Triangle area is positive when the third point is
			// "to the right" of the line formed by first and second
			// point. A point is inside the triangle if it is to the
			// right of every edge.
			if (edge01 >= 0 && edge12 >= 0 && edge20 >= 0)
			{
				float alpha = edge12 / area;
				float beta = edge20 / area;
				float gamma = 1.0f - alpha - beta;

				// Interpolate the depth along the tri w/ barycentric coords
				float z = a.z * alpha + b.z * beta + c.z * gamma;
				if (z < m_depthBuffer[idx])
				{
					m_depthBuffer[idx] = z;
					m_colorBuffer[idx] = color;
					Logger::Info("Setting pixel (%d, %d)", x, y);
					
					// If it wasn't just DrawLine then it'd be nice to include
					// things that use w, like texture coords. But we need to 
					// make sure we use as few colors has possible - we want 
					// contiguous chunks of lines to minimize draw calls.

					// float w = a.w * alpha + b.w * beta + c.w * gamma;
				}
			}

			edge01 += A01;
			edge12 += A12;
			edge20 += A20;
		}

		row01 += B01;
		row12 += B12;
		row20 += B20;
	}
}

void DepthBufferRasterizer::Flush()
{
	/*
	The strategy here is that I want to draw a texture to the screen
	but I want to minimize the amount of DrawLine calls I make - they
	are incredibly slow, since each is both a DLL call and a OpenGL
	call. The idea I had for minimizing DrawLine calls:

	For each row, scan each pixel while keeping a current color and start
	pixel X pos. If the pixel's color is different from the current color, 
	change the current color and draw a line from the start X to current X. 
	Best case: SCREEN_HEIGHT calls (each row is same color), worst case: 
	SCREEN_WIDTH * SCREEN_HEIGHT calls (if each pixel is a different color).

	Hopefully this works well! (i wrote this before implementing it lol)

	This is doing SCREEN_WIDTH * SCREEN_HEIGHT ~= 700k if statements every frame
	My goodness

	*/

	int drawnLines = 0;
	int idx = 0;
	for (float y = 0; y < APP_VIRTUAL_HEIGHT; y++)
	{
		float xStart = 0.0f;
		Color col = m_colorBuffer[idx];
		
		idx++;
		for (float x = 1.0f; x < APP_VIRTUAL_WIDTH; x += 2.0f)
		{
			Color cur = m_colorBuffer[idx];
			if (std::abs(cur.r - col.r) > EPSILON
				|| std::abs(cur.g - col.g) > EPSILON
				|| std::abs(cur.b - col.b) > EPSILON)
			{
				App::DrawLine(xStart, y, x, y, 1.0f, 0.0f, 0.0f);
				xStart = x;
			}
			idx++;
		}
	}
	Logger::Info("Drawn lines: %d", drawnLines);
}
