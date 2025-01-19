#include "pch.h"
#include "depth_buffer_rasterizer.h"

#include "graphics/renderer/renderer.h"
#include "core/app_settings.h"
#include "math/math_utils.h"
#include <execution>

#ifdef PLATFORM_WINDOWS
#include <App/app.h>
#undef max
#undef min
#endif

/*
Resources:
https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/ (this entire series honestly)
*/

DepthBufferRasterizer::DepthBufferRasterizer()
{
	m_clearColor = Color::BLACK;
	m_colorBuffer.resize(APP_VIRTUAL_WIDTH * APP_VIRTUAL_HEIGHT, m_clearColor);
	m_depthBuffer.resize(APP_VIRTUAL_WIDTH * APP_VIRTUAL_HEIGHT, 1.0f);
	m_shadedLightDir = Vec3(0.5f, 0.5f, 1.0f).Normalized();
}


void DepthBufferRasterizer::Clear()
{
	// I used std::fill before, then I thought oh why don't
	// I just clear it as I go? This method kinda does
	// nothing, clearing the color/depth buffer happens during
	// the flush. 

	// This takes 10 ms unparallelized.... oh my god....
//#ifdef USE_TRIVIAL_PARALLELIZATION
//	std::fill(std::execution::par_unseq, m_colorBuffer.begin(), m_colorBuffer.end(), m_clearColor);
//	std::fill(std::execution::par_unseq, m_depthBuffer.begin(), m_depthBuffer.end(), 1.0f);
//#else
//	std::fill(m_colorBuffer.begin(), m_colorBuffer.end(), m_clearColor);
//	std::fill(m_depthBuffer.begin(), m_depthBuffer.end(), 1.0f);
//#endif
}

void DepthBufferRasterizer::RasterizeTriangle(
	const Vec4& a, const Vec4& b, const Vec4& c, 
	const Vec3& an, const Vec3& bn, const Vec3& cn,
	const Color& baseColor, ShadingMode mode)
{
	Color color = baseColor;
	if (mode == ShadingMode::WIREFRAME)
	{
		DrawLineBresenham(static_cast<int>(a.x), static_cast<int>(a.y), static_cast<int>(b.x), static_cast<int>(b.y), color);
		DrawLineBresenham(static_cast<int>(b.x), static_cast<int>(b.y), static_cast<int>(c.x), static_cast<int>(c.y), color);
		DrawLineBresenham(static_cast<int>(c.x), static_cast<int>(c.y), static_cast<int>(a.x), static_cast<int>(a.y), color);
		return;
	}
	else if (mode == ShadingMode::SHADED)
	{
		Vec3 faceNormal = (an + bn + cn).Normalized();
		float shadingFactor = (1.0f + faceNormal.Dot(m_shadedLightDir)) * 0.5f;
		color.r() = baseColor.r() * shadingFactor;
		color.g() = baseColor.g() * shadingFactor;
		color.b() = baseColor.b() * shadingFactor;
	}

	float area = Renderer::Triangle2DArea(a, b, c);

	// Biases for abiding by fill rules
	//int bias0 = IsEdgeTopOrLeft(b, c) ? 0 : -1;
	//int bias1 = IsEdgeTopOrLeft(c, a) ? 0 : -1;
	//int bias2 = IsEdgeTopOrLeft(a, b) ? 0 : -1;

	// Only check the pixels in the triangle's bounding box
	int minX = std::max(0, static_cast<int>(std::floorf(std::min({ a.x, b.x, c.x }))));
	int maxX = std::min(APP_VIRTUAL_WIDTH, static_cast<int>(std::ceilf(std::max({ a.x, b.x, c.x }))));
	int minY = std::max(0, static_cast<int>(std::floorf(std::min({ a.y, b.y, c.y }))));
	int maxY = std::min(APP_VIRTUAL_HEIGHT, static_cast<int>(std::ceilf(std::max({ a.y, b.y, c.y }))));

	// Incremental Pineda rasterization
	Vec4 point = Vec4(static_cast<float>(minX), static_cast<float>(minY), 0.0f, 1.0f);
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

void DepthBufferRasterizer::RasterizeTexture(const Vec4& pos, RID textureHandle)
{
	ASSERT_ERROR(false, "Depth-buffer rasterizer can't draw a texture!");
}

void DepthBufferRasterizer::RasterizeLine(const Vec3& start, const Vec3& end, const Color& color)
{
	DrawLineBresenham(
		static_cast<int>(start.x),
		static_cast<int>(start.y),
		static_cast<int>(end.x),
		static_cast<int>(end.y),
		color);
}

void DepthBufferRasterizer::RasterizeSphere(const Vec3& pos, float radius, const Color& color)
{
	ASSERT_ERROR(false, "Depth-buffer rasterizer's sphere rasterization- isn't implemented yet!");
	// TODO: add method to fill in sphere here, should be simple
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

	for (float y = 0; y < APP_VIRTUAL_HEIGHT; y += RASTER_DOWNSCALING)
	{
		float xStart = 0.0f;
		Color col = m_colorBuffer[idx];
		
		idx++;
		for (float x = 1.0f; x < APP_VIRTUAL_WIDTH; x += RASTER_DOWNSCALING)
		{
			Color cur = m_colorBuffer[idx];
			if (std::abs(cur.r() - col.r()) > EPSILON
				|| std::abs(cur.g() - col.g()) > EPSILON
				|| std::abs(cur.b() - col.b()) > EPSILON)
			{
				App::DrawLine(xStart, y, x, y, col.r(), col.g(), col.b());
				col = cur;
				xStart = x;
			}

			// Now that we've processed the pixel, we can reset it
			// This saved me 7 ms on debug.......
			m_colorBuffer[idx] = m_clearColor;
			m_depthBuffer[idx] = 1.0f;
				
			idx += RASTER_DOWNSCALING;
		}
		idx += APP_VIRTUAL_WIDTH * (RASTER_DOWNSCALING - 1);

		App::DrawLine(xStart, y, APP_VIRTUAL_WIDTH, y, col.r(), col.g(), col.b());
	}
}

void DepthBufferRasterizer::SetClearColor(const Color& color)
{
	m_clearColor = color;
}

void DepthBufferRasterizer::SetLightDirection(const Vec3& direction)
{
	m_shadedLightDir = direction;
}

void DepthBufferRasterizer::DrawLineBresenham(int x0, int y0, int x1, int y1, const Color& color)
{
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;

	while (true)
	{
		if (x0 >= 0 && x0 < APP_VIRTUAL_WIDTH && y0 >= 0 && y0 < APP_VIRTUAL_HEIGHT)
			m_colorBuffer[y0 * APP_VIRTUAL_WIDTH + x0] = color;

		if (x0 == x1 && y0 == y1)
			return;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	}
}
