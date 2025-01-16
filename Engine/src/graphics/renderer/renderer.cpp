#include "pch.h"
#include "renderer.h"

#include "core/app_settings.h"
#include "core/debug/logger.h"
#include "graphics/texture/texture.h"
#include "graphics/mesh/mesh.h"
#include "math/vector/vector4.h"
#include "graphics/shading_mode.h"
#include "math/math_utils.h"

#include <App/App.h>
#include <App/SimpleSprite.h>
#undef max
#undef min

Renderer::Renderer(ResourceManager& resourceManager) :
	m_resourceManager(resourceManager),
	m_txRasterizer(*this, resourceManager)
{
	m_defaultFontHandle = m_resourceManager.Load<Font>("HELVETICA_18");
	m_defaultGlutFont = m_resourceManager.Get<Font>(m_defaultFontHandle).GetGLUTFont();

	m_vertexVRAM.reserve(VRAM_ARR_SIZE);
	m_normalVRAM.reserve(VRAM_ARR_SIZE);
	m_indexVRAM.reserve(VRAM_ARR_SIZE);
}

void Renderer::DrawTextLine(float x, float y, const char* text, Color col)
{
#ifdef PLATFORM_WINDOWS
	App::Print(x, y, text, col.r, col.g, col.b, m_defaultGlutFont);
#endif
}

void Renderer::DrawTextLine(float x, float y, const char* text, Color col, RID fontHandle)
{
#ifdef PLATFORM_WINDOWS
	void* glutFont = m_resourceManager.Get<Font>(fontHandle).GetGLUTFont();
	App::Print(x, y, text, col.r, col.g, col.b, glutFont);
#endif
}

void Renderer::DrawLine(float x0, float y0, float x1, float y1, Color col)
{
#ifdef PLATFORM_WINDOWS
	App::DrawLine(x0, y0, x1, y1, col.r, col.g, col.b);
#endif
}

void Renderer::DrawRect(float x0, float y0, float x1, float y1, Color col)
{
	DrawLine(x0, y0, x1, y0, col);
	DrawLine(x0, y1, x1, y0, col);
	DrawLine(x0, y0, x0, y1, col);
	DrawLine(x1, y0, x1, y1, col);
}

void Renderer::DrawFilledRect(float x0, float y0, float x1, float y1, Color col)
{
	// TODO: assert y0 < y1
	for (float y = y0; y <= y1; y++)
		DrawLine(x0, y, x1, y, col);
}

void Renderer::DrawCircle(Vec2 pos, float radius, Color col)
{
	int segments = std::max(5, static_cast<int>((radius + 5.0f) * 0.5f));
	DrawCircle(pos, radius, col, segments);
}

void Renderer::DrawCircle(Vec2 pos, float radius, Color col, int segments)
{
	float angleStep = TWO_PI / segments;

	for (int i = 0; i < segments + 1; i++)
	{
		int j = (i + 1) % segments;

		float x1 = cosf(angleStep * i) * radius + pos.x;
		float y1 = sinf(angleStep * i) * radius + pos.y;
		float x2 = cosf(angleStep * j) * radius + pos.x;
		float y2 = sinf(angleStep * j) * radius + pos.y;

		DrawLine(x1, y1, x2, y2, col);
	}
}

void Renderer::DrawTexture(float x, float y, RID textureHandle)
{
#ifdef PLATFORM_WINDOWS
	CSimpleSprite& css = m_resourceManager.Get<Texture>(textureHandle).Get();
	css.SetPosition(x, y);
	css.Draw();
#endif
}

void Renderer::DrawTexture(const Transform2D& tf, RID textureHandle)
{
#ifdef PLATFORM_WINDOWS
	CSimpleSprite& css = m_resourceManager.Get<Texture>(textureHandle).Get();
	css.SetPosition(tf.position.x, tf.position.y);
	css.SetAngle(tf.rotation);
	css.SetScale(tf.scale);
	css.Draw();
#endif
}

void Renderer::ClearMeshRasterizer()
{
	m_rasterizer.Clear();
}

void Renderer::ClearTextureRasterizer()
{
	m_txRasterizer.Clear();
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

#ifdef USE_PAINTERS_FOR_WIREFRAME
		if (meshInstance.mode == ShadingMode::WIREFRAME)
		{
			m_txRasterizer.RasterizeTriangle(
				a, b, c,
				m_normalVRAM[m_indexVRAM[i + 1]],
				m_normalVRAM[m_indexVRAM[i + 3]],
				m_normalVRAM[m_indexVRAM[i + 5]],
				meshInstance.color, meshInstance.mode
			);
		}
		else
		{
#endif
			m_rasterizer.RasterizeTriangle(
				a, b, c,
				m_normalVRAM[m_indexVRAM[i + 1]],
				m_normalVRAM[m_indexVRAM[i + 3]],
				m_normalVRAM[m_indexVRAM[i + 5]],
				meshInstance.color, meshInstance.mode
			);
#ifdef USE_PAINTERS_FOR_WIREFRAME
		}
#endif

	}
}

void Renderer::Draw3DLine(const Vec3& start, const Vec3& end, const Color& color)
{
	// World Space -> Clip Space
	Mat4 VP = m_projection * m_view;
	Vec4 a = VP * Vec4(start);
	Vec4 b = VP * Vec4(end);

	// Near Culling
	if (a.z < 0 && b.z < 0)
		return;

	// Perspective division
	a.x /= a.w; b.x /= b.w;
	a.y /= a.w;	b.y /= b.w;
	a.z /= a.w;	b.z /= b.w;

	// Clip Space -> Screen Space
	a.x = (a.x + 1.0f) * 0.5f * APP_VIRTUAL_WIDTH;
	a.y = (a.y + 1.0f) * 0.5f * APP_VIRTUAL_HEIGHT;
	b.x = (b.x + 1.0f) * 0.5f * APP_VIRTUAL_WIDTH;
	b.y = (b.y + 1.0f) * 0.5f * APP_VIRTUAL_HEIGHT;

#ifdef USE_PAINTERS_FOR_WIREFRAME
	m_txRasterizer.RasterizeLine(a, b, color);
#else
	m_rasterizer.RasterizeLine(a, b, color);
#endif
}

void Renderer::DrawSphere(const Vec3& pos, float radius, Color col)
{
	// TODO: this is the exact same process as DrawTexture
	// find a way to combine the two into the same function
	// or don't, if billboard rendering ever becomes different
	// than this

	// World Space -> Clip Space
	Vec4 viewPoint = m_view * Vec4(pos);
	Vec4 point = m_projection * viewPoint;

	// Near-culling
	if (point.z < 0)
		return;

	// Determine the radius by making a point
	// on the edge of the sphere (in view space)
	// and calculating what the radius becomes after projection;
	Vec4 edgeViewPoint = viewPoint; edgeViewPoint.x += radius;
	Vec4 edgePoint = m_projection * edgeViewPoint;

	// Perspective division
	point.x /= point.w;	edgePoint.x /= edgePoint.w;
	point.y /= point.w;	edgePoint.y /= edgePoint.w;
	point.z /= point.w;	edgePoint.z /= edgePoint.w;

	// Clip Space -> Screen Space
	point.x = (point.x + 1.0f) * 0.5f * APP_VIRTUAL_WIDTH;
	point.y = (point.y + 1.0f) * 0.5f * APP_VIRTUAL_HEIGHT;
	edgePoint.x = (edgePoint.x + 1.0f) * 0.5f * APP_VIRTUAL_WIDTH;
	edgePoint.y = (edgePoint.y + 1.0f) * 0.5f * APP_VIRTUAL_HEIGHT;

	//Logger::Info("Center: %s, Edge: %s", point.ToString().c_str(), edgePoint.ToString().c_str());
	radius = edgePoint.x - point.x;
	m_txRasterizer.RasterizeSphere(point, radius, col);
}

void Renderer::DrawBillboard(const Vec3& pos, float scale, RID textureHandle)
{
	// World Space -> Clip Space
	Vec4 point = m_projection * m_view * Vec4(pos);
	
	// Near-culling
	if (point.z < 0)
		return;

	// Perspective division
	point.x /= point.w;
	point.y /= point.w;
	point.z /= point.w;

	// Clip Space -> Screen Space
	point.x = (point.x + 1.0f) * 0.5f * APP_VIRTUAL_WIDTH;
	point.y = (point.y + 1.0f) * 0.5f * APP_VIRTUAL_HEIGHT;
	
	// Rasterizer expects w = scale
	point.w = scale;
	m_txRasterizer.RasterizeTexture(point, textureHandle);
}

void Renderer::FlushMeshes()
{
	m_rasterizer.Flush();

	// Downscaled raster lines make the debug info unreadable
	// This fixes it
#if defined(USE_DEBUG_RASTER_DOWNSCALING) && defined(_DEBUG)
	DrawFilledRect(10, 10, 130, 50, Color::WHITE);
#endif
}

void Renderer::Flush3DTextures()
{
	m_txRasterizer.Flush();
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
