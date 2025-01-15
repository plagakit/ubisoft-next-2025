#include "pch.h"
#include "painters_rasterizer.h"
#include <App/app.h>

void PaintersRasterizer::Clear()
{
	m_triangles.clear();
}

void PaintersRasterizer::RasterizeTriangle(
	const Vec4& a, const Vec4& b, const Vec4& c, 
	const Vec3& an, const Vec3& bn, const Vec3& cn, 
	const Color& color, ShadingMode mode)
{
	m_triangles.push_back({ a, b, c, color });
}

void PaintersRasterizer::Flush()
{
	if (m_doSort)
	{
		std::sort(m_triangles.begin(), m_triangles.end(),
			[](const Triangle& t1, const Triangle& t2)
			{
				float z1 = (t1.a.z + t1.b.z + t1.c.z) * 0.333f;
				float z2 = (t2.a.z + t2.b.z + t2.c.z) * 0.333f;
				return z1 > z2;
			}
		);
	}

	for (const auto& t : m_triangles)
	{
		App::DrawLine(t.a.x, t.a.y, t.b.x, t.b.y, t.color.r, t.color.g, t.color.b);
		App::DrawLine(t.b.x, t.b.y, t.c.x, t.c.y, t.color.r, t.color.g, t.color.b);
		App::DrawLine(t.c.x, t.c.y, t.a.x, t.a.y, t.color.r, t.color.g, t.color.b);
	}
}

void PaintersRasterizer::SetDoSort(bool doSort)
{
	m_doSort = doSort;
}
