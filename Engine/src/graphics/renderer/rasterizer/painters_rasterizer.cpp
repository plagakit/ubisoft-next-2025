#include "pch.h"
#include "painters_rasterizer.h"

#include "graphics/renderer/renderer.h"
#include <App/app.h>

PaintersRasterizer::PaintersRasterizer(Renderer& renderer, ResourceManager& resourceMgr) :
	m_renderer(renderer),
	m_resourceMgr(resourceMgr), 
	m_doSort(true)
{}

void PaintersRasterizer::Clear()
{
	m_textures.clear();
}

void PaintersRasterizer::RasterizeTriangle(
	const Vec4& a, const Vec4& b, const Vec4& c, 
	const Vec3& an, const Vec3& bn, const Vec3& cn, 
	const Color& color, ShadingMode mode)
{
	m_triangles.push_back({ a, b, c, color });
}

void PaintersRasterizer::RasterizeTexture(const Vec4& pos, RID textureHandle)
{
	m_textures.push_back({ pos, pos.w, textureHandle });
}

void PaintersRasterizer::Flush()
{
	if (m_doSort)
	{
		std::sort(m_triangles.begin(), m_triangles.end(),
			[](const TrianglePrimitive& t1, const TrianglePrimitive& t2)
			{
				float z1 = (t1.a.z + t1.b.z + t1.c.z) * 0.333f;
				float z2 = (t2.a.z + t2.b.z + t2.c.z) * 0.333f;
				return z1 > z2;
			}
		);

		std::sort(m_textures.begin(), m_textures.end(),
			[](const TexturePrimitive& t1, const TexturePrimitive& t2)
			{
				return t1.pos.z > t2.pos.z;
			}
		);
	}

	for (const auto& t : m_triangles)
	{
		App::DrawLine(t.a.x, t.a.y, t.b.x, t.b.y, t.color.r, t.color.g, t.color.b);
		App::DrawLine(t.b.x, t.b.y, t.c.x, t.c.y, t.color.r, t.color.g, t.color.b);
		App::DrawLine(t.c.x, t.c.y, t.a.x, t.a.y, t.color.r, t.color.g, t.color.b);
	}

	for (const auto& t : m_textures)
	{
		Transform2D tf;
		tf.position = { t.pos.x, t.pos.y };
		tf.scale = (1.0f - t.pos.z) * t.scale;
		m_renderer.DrawTexture(tf, t.textureHandle);
	}
}

void PaintersRasterizer::SetDoSort(bool doSort)
{
	m_doSort = doSort;
}
