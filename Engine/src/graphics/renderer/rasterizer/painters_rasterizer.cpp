#include "pch.h"
#include "painters_rasterizer.h"

#include "core/app_settings.h"
#include "graphics/renderer/renderer.h"
#include <App/app.h>
#include <algorithm>

#ifdef USE_TRIVIAL_PARALLELIZATION
#include <execution>
#endif

PaintersRasterizer::PaintersRasterizer(Renderer& renderer, ResourceManager& resourceMgr) :
	m_renderer(renderer),
	m_resourceMgr(resourceMgr), 
	m_doSort(true)
{}

void PaintersRasterizer::ReserveMemory(size_t primitives)
{
	m_primitives.reserve(primitives);
}

void PaintersRasterizer::Clear()
{
	m_primitives.clear();
}

void PaintersRasterizer::RasterizeTriangle(
	const Vec4& a, const Vec4& b, const Vec4& c, 
	const Vec3& an, const Vec3& bn, const Vec3& cn, 
	const Color& color, ShadingMode mode)
{
	m_primitives.emplace_back(TrianglePrimitive{ a, b, c, color });
}

void PaintersRasterizer::RasterizeTexture(const Vec4& pos, RID textureHandle)
{
	//m_textures.push_back({ pos, pos.w, textureHandle });
	m_primitives.emplace_back(TexturePrimitive{ pos, pos.w, textureHandle });
}

void PaintersRasterizer::RasterizeLine(const Vec3& start, const Vec3& end, const Color& color)
{
	m_primitives.emplace_back(LinePrimitive{ start, end, color });
}

void PaintersRasterizer::RasterizeSphere(const Vec3& pos, float radius, const Color& color)
{
	m_primitives.emplace_back(SpherePrimitive{ pos, radius, color });
}

void PaintersRasterizer::Flush()
{
	if (m_doSort)
	{
		// https://stackoverflow.com/questions/54222962/sorting-vectorvariant-does-not-work-correctly-via-operator
		std::sort(
#ifdef USE_TRIVIAL_PARALLELIZATION
			std::execution::par,
#endif
			m_primitives.begin(), m_primitives.end(),
			[](const Primitive& p1, const Primitive& p2)
			{
				return std::visit(
					[](const auto& x, const auto& y) 
					{
						return x.GetDepth() > y.GetDepth();
					}, 
				p1, p2);
			}
		);
	}

	for (auto& primitive : m_primitives)
	{
		std::visit(
			[this](auto& p)
			{
				p.Render(m_renderer);
			},
		primitive);
	}
}

void PaintersRasterizer::SetDoSort(bool doSort)
{
	m_doSort = doSort;
}

float PaintersRasterizer::TrianglePrimitive::GetDepth() const
{
	return (a.z + b.z + c.z) * 0.333f;
}

void PaintersRasterizer::TrianglePrimitive::Render(Renderer& renderer)
{
	renderer.DrawLine(a.x, a.y, b.x, b.y, color);
	renderer.DrawLine(b.x, b.y, c.x, c.y, color);
	renderer.DrawLine(c.x, c.y, a.x, a.y, color);
}

float PaintersRasterizer::TexturePrimitive::GetDepth() const
{
	return pos.z;
}

void PaintersRasterizer::TexturePrimitive::Render(Renderer& renderer)
{
	Transform2D tf;
	tf.position = { pos.x, pos.y };
	tf.scale = (1.0f - pos.z) * scale;
	renderer.DrawTexture(tf, textureHandle);
}

float PaintersRasterizer::LinePrimitive::GetDepth() const
{
	return (start.z + end.z) * 0.5f;
}

void PaintersRasterizer::LinePrimitive::Render(Renderer& renderer)
{
	renderer.DrawLine(start.x, start.y, end.x, end.y, color);
}

float PaintersRasterizer::SpherePrimitive::GetDepth() const
{
	// TODO: test this to adjust and see what the best looking result is
	return pos.z;
}

void PaintersRasterizer::SpherePrimitive::Render(Renderer& renderer)
{
	Vec2 pos2d = Vec2(pos.x, pos.y);
	float scaledRadius = radius; //(1.0f - pos.z) * radius;
	renderer.DrawCircle(pos2d, scaledRadius, color);
}
