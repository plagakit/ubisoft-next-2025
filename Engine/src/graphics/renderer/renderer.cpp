#include "pch.h"
#include "renderer.h"

#include "core/app_settings.h"
#include "core/debug/logger.h"
#include "graphics/texture/texture.h"
#include "graphics/mesh/mesh.h"
#include "math/vector/vector4.h"
#include "graphics/shading_mode.h"

#include <App/App.h>
#include <App/SimpleSprite.h>

Renderer::Renderer(ResourceManager& resourceManager) :
	m_resourceManager(resourceManager)
{
	m_defaultFontHandle = m_resourceManager.Load<Font>("HELVETICA_18");
	m_defaultGlutFont = m_resourceManager.Get<Font>(m_defaultFontHandle).GetGLUTFont();

	// 30000 x 32 bytes approx 1 mb each for vertex/norm
	m_vertexVRAM.reserve(VRAM_ARR_SIZE);
	m_normalVRAM.reserve(VRAM_ARR_SIZE);
	m_indexVRAM.reserve(VRAM_ARR_SIZE);
}

void Renderer::DrawTextLine(float x, float y, const char* text, Color col)
{
	App::Print(x, y, text, col.r, col.g, col.b, m_defaultGlutFont);
}

void Renderer::DrawTextLine(float x, float y, const char* text, Color col, RID fontHandle)
{
	void* glutFont = m_resourceManager.Get<Font>(fontHandle).GetGLUTFont();
	App::Print(x, y, text, col.r, col.g, col.b, glutFont);
}

void Renderer::DrawLine(float x0, float y0, float x1, float y1, Color col)
{
	App::DrawLine(x0, y0, x1, y1, col.r, col.g, col.b);
}

void Renderer::DrawRect(float x0, float y0, float x1, float y1, Color col)
{
	App::DrawLine(x0, y0, x1, y0, col.r, col.g, col.b);
	App::DrawLine(x0, y1, x1, y0, col.r, col.g, col.b);
	App::DrawLine(x0, y0, x0, y1, col.r, col.g, col.b);
	App::DrawLine(x1, y0, x1, y1, col.r, col.g, col.b);
}

void Renderer::DrawFilledRect(float x0, float y0, float x1, float y1, Color col)
{
	// TODO: assert y0 < y1
	for (float y = y0; y <= y1; y++)
		App::DrawLine(x0, y, x1, y, col.r, col.g, col.b);
}

void Renderer::DrawTexture(float x, float y, RID textureHandle)
{
	CSimpleSprite& css = m_resourceManager.Get<Texture>(textureHandle).Get();
	css.SetPosition(x, y);
	css.Draw();
}

void Renderer::DrawTexture(const Transform2D& tf, RID textureHandle)
{
	CSimpleSprite& css = m_resourceManager.Get<Texture>(textureHandle).Get();
	css.SetPosition(tf.position.x, tf.position.y);
	css.SetAngle(tf.rotation);
	css.SetScale(tf.scale);
	css.Draw();
}

void Renderer::ClearScreen()
{
	m_rasterizer.Clear();
}

void Renderer::DrawMesh(const Mat4& model, const MeshInstance& meshInstance)
{
	Mesh& mesh = m_resourceManager.Get<Mesh>(meshInstance.meshHandle);
	const auto& vertices = mesh.GetVertexBuffer();
	const auto& normals = mesh.GetNormalBuffer();
	const auto& indices = mesh.GetIndexBuffer();

	ASSERT_ERROR(vertices.size() < VRAM_ARR_SIZE, "Vertex count for %s exceeds Renderer buffer! (%d > %d)", mesh.GetPath().c_str(), vertices.size(), VRAM_ARR_SIZE);
	ASSERT_ERROR(normals.size() < VRAM_ARR_SIZE, "Normal count for %s exceeds Renderer buffer! (%d > %d)", mesh.GetPath().c_str(), normals.size(), VRAM_ARR_SIZE);
	ASSERT_ERROR(indices.size() < VRAM_ARR_SIZE, "Index count for %s exceeds Renderer buffer! (%d > %d)", mesh.GetPath().c_str(), indices.size(), VRAM_ARR_SIZE);

	// Copy verts into "VRAM", vector::assign will call Vec4(Vec3) which sets w = 1
	m_vertexVRAM.assign(vertices.begin(), vertices.end());
	m_normalVRAM.assign(normals.begin(), normals.end());

	// Model Space -> World Space -> View Space -> Clip Space
	Mat4 MVP = m_projection * m_view * model;
	for (Vec4& p : m_vertexVRAM) p = MVP * p;

	if (meshInstance.mode == ShadingMode::SHADED)
		for (Vec3& n : m_normalVRAM) 
			n = model * n;

	// Near Culling
	m_indexVRAM.clear();
	for (int i = 0; i < indices.size(); i += 6)
	{
		Vec4& a = m_vertexVRAM[indices[i]];
		Vec4& b = m_vertexVRAM[indices[i + 2]];
		Vec4& c = m_vertexVRAM[indices[i + 4]];

		// If all verts are in front of "near plane", add triangle indices
		if (a.z >= 0.0f && b.z >= 0.0f && c.z >= 0.0f)
		{
			for (int j = 0; j < 6; j++)
				m_indexVRAM.push_back(indices[i + j]);

			// Convert normals into face normals
			// this would serve better as some sort of vertex shader
			if (meshInstance.mode == ShadingMode::SHADED)
			{
				Vec3 faceNormal = (Vec3(b) - Vec3(a)).Cross(Vec3(c) - Vec3(a)).Normalized();
				m_normalVRAM[indices[i + 1]] = faceNormal;
				m_normalVRAM[indices[i + 3]] = faceNormal;
				m_normalVRAM[indices[i + 5]] = faceNormal;
			}
		}

	}
	
	for (Vec4& p : m_vertexVRAM)
	{
		// Perpsective division
		float w = p.w;
		p.x /= w;
		p.y /= w;
		p.z /= w;
		p.w = w;

		// Clip Space -> Screen Space
		p.x = (p.x + 1.0f) * 0.5f * APP_VIRTUAL_WIDTH;
		p.y = (p.y + 1.0f) * 0.5f * APP_VIRTUAL_HEIGHT;
	}

	for (int i = 0; i < m_indexVRAM.size(); i += 6)
	{
		Vec4& a = m_vertexVRAM[m_indexVRAM[i]];
		Vec4& b = m_vertexVRAM[m_indexVRAM[i+2]];
		Vec4& c = m_vertexVRAM[m_indexVRAM[i+4]];

		// Backface culling
		if (!IsCounterClockwise(a, b, c))
			continue;

		m_rasterizer.RasterizeTriangle(
			a, b, c,
			m_normalVRAM[m_indexVRAM[i + 1]],
			m_normalVRAM[m_indexVRAM[i + 3]],
			m_normalVRAM[m_indexVRAM[i + 5]],
			meshInstance.color, meshInstance.mode
		);
	}
}

void Renderer::DrawBillboard(const Vec3& pos, float scale, RID textureHandle)
{
}

void Renderer::Flush()
{
	m_rasterizer.Flush();

	// Downscaled raster lines make the debug info unreadable
	// This fixes it
#if defined(USE_DEBUG_RASTER_DOWNSCALING) && defined(_DEBUG)
	DrawFilledRect(10, 10, 130, 50, Color::WHITE);
#endif
}

void Renderer::SetViewMatrix(const Mat4& view)
{
	m_view = view;
}

void Renderer::SetProjectionMatrix(const Mat4& projection)
{
	m_projection = projection;
}

void Renderer::SetClearColor(const Color& color)
{
	m_rasterizer.SetClearColor(color);
}

float Renderer::Triangle2DArea(const Vec4& a, const Vec4& b, const Vec4& c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool Renderer::IsCounterClockwise(const Vec4& a, const Vec4& b, const Vec4& c)
{
	float area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	return area > 0;
}

float Renderer::LinePlaneIntersection(const Vec3& planePos, const Vec3& planeNormal, const Vec3& lineStart, const Vec3& lineEnd, Vec4& intersection)
{
	float planeD = planeNormal.Dot(planePos);
	float ad = lineStart.Dot(planeNormal);
	float bd = lineEnd.Dot(planeNormal);
	float t = (-planeD - ad) / (bd - ad);

	intersection = lineStart + (lineEnd - lineStart) * t;
	return t;
}

//int Renderer::ClipTriangleAgainstPlane(
//	const Vec3& planePos, const Vec3& planeNormal, 
//	unsigned int idxA, unsigned int idxB, unsigned int idxC,
//	Vec4& out1A, Vec4& out1B, Vec4& out1C, 
//	Vec4& out2A, Vec4& out2B, Vec4& out2C)
//{
//	Vec4& a = m_vertexVRAM[indices[i]];
//	Vec4& b = m_vertexVRAM[indices[i + 2]];
//	Vec4& c = m_vertexVRAM[indices[i + 4]];
//
//	const Vec4* inside[3];
//	const Vec4* outside[3];
//	int numInside = 0;
//	int numOutside = 0;
//
//	for (auto& v : { a, b, c })
//	{
//		float dist = (
//			planeNormal.x * v.x
//			+ planeNormal.y * v.y
//			+ planeNormal.z * v.z
//			- planeNormal.Dot(planePos)
//			);
//
//		if (dist >= 0)
//			inside[numInside++] = &v;
//		else
//			outside[numOutside++] = &v;
//	}
//
//	// All points outside of the plane - discard tri
//	if (numInside == 0)
//		return 0;
//
//	// All points are inside the plane - tri is all good
//	if (numInside == 3)
//	{
//		out1A = a;
//		out1B = b;
//		out1C = c;
//		return 1;
//	}
//
//	// One point inside -> clip w/ plane to form a triangle
//	if (numInside == 1 && numOutside == 2)
//	{
//		out1A = *inside[0];
//		LinePlaneIntersection(planePos, planeNormal, *inside[0], *outside[0], out1B);
//		LinePlaneIntersection(planePos, planeNormal, *inside[0], *outside[1], out1C);
//
//		if (!IsCounterClockwise(out1A, out1B, out1C))
//			std::swap(out1A, out1C);
//
//		return 1;
//	}
//
//	// Two points inside -> clip w/ plane to form a quad
//	if (numInside == 2 && numOutside == 1)
//	{
//		out1A = *inside[0];
//		out1B = *inside[1];
//		LinePlaneIntersection(planePos, planeNormal, *inside[0], *outside[0], out1C);
//
//		out2A = out1C;
//		out2B = *inside[1];
//		LinePlaneIntersection(planePos, planeNormal, *inside[1], *outside[0], out2C);
//
//		if (!IsCounterClockwise(out1A, out1B, out1C))
//			std::swap(out1A, out1C);
//
//		if (!IsCounterClockwise(out2A, out2B, out2C))
//			std::swap(out2A, out2C);
//
//		return 2;
//	}
//
//	return -1;
//}
