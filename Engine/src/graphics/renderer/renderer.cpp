#include "pch.h"
#include "renderer.h"

#include "core/app_settings.h"
#include "core/debug/logger.h"
#include "graphics/texture/texture.h"
#include "graphics/mesh/mesh.h"
#include "math/vector/vector4.h"
#include "graphics/shading_mode.h"
#include "math/math_utils.h"

#ifdef PLATFORM_WINDOWS
#include <App/App.h>
#include <App/SimpleSprite.h>
#undef max
#undef min
#undef ERROR
#endif

Renderer::Renderer(ResourceManager& resourceManager) :
	m_resourceManager(resourceManager),
	m_paintersRaster(*this, resourceManager)
{
	m_defaultFontHandle = m_resourceManager.Load<Font>("HELVETICA_18");
	m_defaultGlutFont = m_resourceManager.Get<Font>(m_defaultFontHandle).GetGLUTFont();

	m_vertexVRAM.reserve(VRAM_ARR_SIZE);
	m_normalVRAM.reserve(VRAM_ARR_SIZE);
	m_indexVRAM.reserve(VRAM_ARR_SIZE);

	SetNearPlane(Vec3(0.0f, 0.0f, -0.9f), Vec3::FORWARD);
}

// BASIC PLATFORM-DEPENDANT DRAWING

void Renderer::DrawScreenLine(float x0, float y0, float x1, float y1, Color col)
{
#ifdef PLATFORM_WINDOWS
	App::DrawLine(x0, y0, x1, y1, col.r(), col.g(), col.b());
#endif
}

void Renderer::DrawScreenLine(float x0, float y0, float x1, float y1, Color col, int thickness)
{
#ifdef PLATFORM_WINDOWS
	Vec2 dir = (Vec2(x1, y1) - Vec2(x0, y0)).Normalized();
	Vec2 perp = Vec2(dir.y, -dir.x);

	constexpr float THICK_STEP = 1.0f;
	for (int i = 0; i < thickness; i++)
	{
		Vec2 a = Vec2(x0, y0) + perp * (THICK_STEP * i);
		Vec2 b = Vec2(x1, y1) + perp * (THICK_STEP * i);
		DrawScreenLine(a.x, a.y, b.x, b.y, col);
	}
#endif
}

void Renderer::DrawScreenTextLine(float x, float y, const char* text, Color col, RID fontHandle)
{
#ifdef PLATFORM_WINDOWS
	void* glutFont = m_resourceManager.Get<Font>(fontHandle).GetGLUTFont();
	App::Print(x, y, text, col.r(), col.g(), col.b(), glutFont);
#endif
}

void Renderer::DrawScreenTexture(float x, float y, float angle, float scale, RID textureHandle, Color modulate)
{
#ifdef PLATFORM_WINDOWS
	CSimpleSprite& css = m_resourceManager.Get<Texture>(textureHandle).Get();
	css.SetPosition(x, y);
	css.SetAngle(angle);
	css.SetScale(scale);
	css.SetColor(modulate.r(), modulate.g(), modulate.b());
	css.Draw();
#endif
}

// 2D DRAWING FUNCTIONS

void Renderer::Draw2DLine(const Vec2& a, const Vec2& b, Color col)
{
	Vec2 na = m_2Dview.Transform(a);
	Vec2 nb = m_2Dview.Transform(b);
	DrawScreenLine(na.x, na.y, nb.x, nb.y, col);
}

void Renderer::Draw2DLine(const Vec2& a, const Vec2& b, Color col, int thickness)
{
	Vec2 na = m_2Dview.Transform(a);
	Vec2 nb = m_2Dview.Transform(b);
	DrawScreenLine(na.x, na.y, nb.x, nb.y, col, thickness);
}

void Renderer::DrawTextLine(const Vec2& pos, const std::string& text, Color col)
{
	Vec2 newPos = m_2Dview.Transform(pos);
	DrawScreenTextLine(newPos.x, newPos.y, text.c_str(), col, m_defaultFontHandle);
}

void Renderer::DrawTextLine(const Vec2& pos, const std::string& text, Color col, RID fontHandle)
{
	Vec2 newPos = m_2Dview.Transform(pos);
	DrawScreenTextLine(newPos.x, newPos.y, text.c_str(), col, fontHandle);
}

void Renderer::DrawRect(const Vec2& oldA, const Vec2& oldB, Color col)
{
	Vec2 a = m_2Dview.Transform(oldA);
	Vec2 b = m_2Dview.Transform(oldB);
	DrawScreenLine(a.x, a.y, b.x, a.y, col); // top
	DrawScreenLine(a.x, b.y, b.x, b.y, col); // bottom
	DrawScreenLine(a.x, a.y, a.x, b.y, col); // left
	DrawScreenLine(b.x, a.y, b.x, b.y, col); // right
}

void Renderer::DrawRect(const Vec2& pos, float width, float height, Color col)
{
	Vec2 half = Vec2(width, height) * 0.5f;
	DrawRect(pos - half, pos + half, col);
}

void Renderer::DrawFilledRect(const Vec2& oldA, const Vec2& oldB, Color col)
{
	Vec2 a = m_2Dview.Transform(oldA);
	Vec2 b = m_2Dview.Transform(oldB);
	if (a.y > b.y)
		std::swap(a, b);

	for (float y = a.y; y <= b.y; y++)
		DrawScreenLine(a.x, y, b.x, y, col);
}

void Renderer::DrawFilledRect(const Vec2& pos, float width, float height, Color col)
{
	Vec2 half = Vec2(width, height) * 0.5f;
	DrawFilledRect(pos - half, pos + half, col);
}

void Renderer::DrawCircle(const Vec2& pos, float radius, Color col)
{
	int segments = std::max(5, static_cast<int>((radius * m_2Dview.scale + 5.0f) * 0.5f));
	DrawCircle(pos, radius, col, segments);
}

void Renderer::DrawCircle(const Vec2& oldPos, float radius, Color col, int segments)
{
	Vec2 pos = m_2Dview.Transform(oldPos);
	radius *= m_2Dview.scale;

	float angleStep = TWO_PI / segments;

	for (int i = 0; i < segments + 1; i++)
	{
		int j = (i + 1) % segments;

		float x1 = cosf(angleStep * i) * radius + pos.x;
		float y1 = sinf(angleStep * i) * radius + pos.y;
		float x2 = cosf(angleStep * j) * radius + pos.x;
		float y2 = sinf(angleStep * j) * radius + pos.y;

		DrawScreenLine(x1, y1, x2, y2, col);
	}
}

void Renderer::DrawTexture(const Transform2D& tf, RID textureHandle, Color modulate)
{
	DrawScreenTexture(tf.position.x, tf.position.y, tf.rotation, tf.scale, textureHandle, modulate);
}

void Renderer::Set2DView(const Transform2D& view)
{
	m_2Dview = view;
}

void Renderer::ClearDepthRasterizer()
{
	m_depthRaster.Clear();
}

void Renderer::ClearPaintersRasterizer()
{
	m_paintersRaster.Clear();
}

void Renderer::DrawMesh(const Mat4& model, const MeshInstance& meshInstance)
{
	m_metrics.renderedMeshes++;

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
	Mat4 MVP = m_VP * model;
	for (Vec4& p : m_vertexVRAM) 
		p = MVP * p;

	Mat4 MV = m_view * model;
	if (meshInstance.mode == ShadingMode::SHADED)
		for (Vec3& n : m_normalVRAM)
			n = model * Vec4(n.x, n.y, n.z, 0.0f);

	// Near Culling
	m_indexVRAM.clear();
	for (int i = 0; i < indices.size(); i += 6)
	{
		Vec4& a = m_vertexVRAM[indices[i]];
		Vec4& b = m_vertexVRAM[indices[i + 2]];
		Vec4& c = m_vertexVRAM[indices[i + 4]];

		Vec4 out[6];
		ClipResult clip = ClipTriangleAgainstPlane(
			m_nearPlanePos, m_nearPlaneNormal,
			a, b, c, out[0], out[1], out[2], out[3], out[4], out[5]
		);

		// Note: this uses switch case fall through in its logic
		switch (clip)
		{
		case DISCARD:
			m_metrics.clippedTriangles++;
			continue;

		case GOOD: // Add original tri to index buffer (copy 6 indices)
			m_indexVRAM.insert(m_indexVRAM.end(), indices.begin() + i, indices.begin() + i + 6);
			break;

		case TWO_NEW: // Add out tri 2 to index & vert buffer
		{
			unsigned int last2 = static_cast<unsigned int>(m_vertexVRAM.size());
			m_vertexVRAM.insert(m_vertexVRAM.end(), out + 3, out + 6);
			m_indexVRAM.insert(m_indexVRAM.end(), {
				last2, indices[i + 1],	   // pos + normal 1
				last2 + 1, indices[i + 3], // pos + normal 2
				last2 + 2, indices[i + 5]  // pos + normal 3
				});
			[[fallthrough]];
		}
		case ONE_NEW: // Add out tri 1 to index & vert buffer
		{
			unsigned int last = static_cast<unsigned int>(m_vertexVRAM.size());
			m_vertexVRAM.insert(m_vertexVRAM.end(), out, out + 3);
			m_indexVRAM.insert(m_indexVRAM.end(), {
				last, indices[i + 1],	// pos + normal 1
				last + 1, indices[i + 3], // pos + normal 2
				last + 2, indices[i + 5]  // pos + normal 3
				});
			break;
		}
		default:
			ASSERT_ERROR(false, "Clip result is error?!?!?!");
			break;
		}

		// Pseudo near culling, needs to be fixed
		//if (a.z >= 0.0f && b.z >= 0.0f && c.z >= 0.0f)
		//{
		//	for (int j = 0; j < 6; j++)
		//		m_indexVRAM.push_back(indices[i + j]);

		//	// Convert normals into face normals
		//	// this would serve better as some sort of vertex shader
		//	if (meshInstance.mode == ShadingMode::SHADED)
		//	{
		//		Vec3 faceNormal = (Vec3(b) - Vec3(a)).Cross(Vec3(c) - Vec3(a)).Normalized();
		//		m_normalVRAM[indices[i + 1]] = faceNormal;
		//		m_normalVRAM[indices[i + 3]] = faceNormal;
		//		m_normalVRAM[indices[i + 5]] = faceNormal;
		//	}
		//}

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

		// Out of screen culling
		if (a.x < 0.0f && b.x < 0.0f && c.x < 0.0f)	
			continue;
		if (a.x > APP_VIRTUAL_WIDTH && b.x > APP_VIRTUAL_WIDTH && c.x > APP_VIRTUAL_WIDTH)
			continue;
		if (a.y < 0.0f && b.y < 0.0f && c.y < 0.0f)
			continue;
		if (a.y > APP_VIRTUAL_HEIGHT && b.y > APP_VIRTUAL_HEIGHT && c.y > APP_VIRTUAL_HEIGHT)
			continue;

		m_metrics.rasterizedTriangles++;


#ifdef USE_PAINTERS_FOR_WIREFRAME
		if (meshInstance.mode == ShadingMode::WIREFRAME)
		{
			m_paintersRaster.RasterizeTriangle(
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
			m_depthRaster.RasterizeTriangle(
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
	Vec4 a = m_VP * Vec4(start);
	Vec4 b = m_VP * Vec4(end);

	// Near Culling
	if (a.z < 0 || b.z < 0)
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
	m_paintersRaster.RasterizeLine(a, b, color);
#else
	m_depthRaster.RasterizeLine(a, b, color);
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
	m_paintersRaster.RasterizeSphere(point, radius, col);
}

void Renderer::Draw3DTexture(const Vec3& pos, float scale, RID textureHandle)
{
	// World Space -> Clip Space
	Vec4 point = m_VP * Vec4(pos);
	
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
	
	// TODO: can do this better
	//CSimpleSprite& spr = m_resourceManager.Get<Texture>(textureHandle).Get();
	//float approxX = point.x + spr.GetWidth();
	//float approxY = point.y + spr.GetHeight();
	//if (approxX < 0 || approxX > APP_VIRTUAL_WIDTH || approxY < 0 || approxY > APP_VIRTUAL_HEIGHT)
	//	return;

	// Rasterizer expects w = scale
	point.w = scale;
	m_paintersRaster.RasterizeTexture(point, textureHandle);
}

void Renderer::FlushDepthRasterizer()
{
	m_depthRaster.Flush();

	// Downscaled raster lines make the debug info unreadable
	// This fixes it
#if defined(USE_DEBUG_RASTER_DOWNSCALING) && defined(_DEBUG)
	DrawFilledRect(Vec2(10, 10), Vec2(130, 50), Color::WHITE);
#endif
}

void Renderer::FlushPaintersRasterizer()
{
	m_paintersRaster.Flush();
}

void Renderer::SetViewMatrix(const Mat4& view)
{
	m_view = view;
	m_VP = m_projection * view;
	m_invVP = view * m_projection;
}

void Renderer::SetProjectionMatrix(const Mat4& projection)
{
	m_projection = projection;
	m_VP = projection * m_view;
	m_invVP = m_view * projection;
}

void Renderer::SetNearPlane(const Vec3& pos, const Vec3& normal)
{
	m_nearPlanePos = pos;
	m_nearPlaneNormal = normal;
}

void Renderer::SetClearColor(const Color& color)
{
	m_depthRaster.SetClearColor(color);
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

Renderer::ClipResult Renderer::ClipTriangleAgainstPlane(
	const Vec3& planePos, const Vec3& planeNormal, 
	Vec4& a, Vec4& b, Vec4& c,
	Vec4& out1A, Vec4& out1B, Vec4& out1C, 
	Vec4& out2A, Vec4& out2B, Vec4& out2C)
{	
	const Vec4* inside[3];
	const Vec4* outside[3];
	int numInside = 0;
	int numOutside = 0;

	for (auto& v : { a, b, c })
	{
		float dist = (
			planeNormal.x * v.x
			+ planeNormal.y * v.y
			+ planeNormal.z * v.z
			- planeNormal.Dot(planePos)
			);

		if (dist >= 0)
			inside[numInside++] = &v;
		else
			outside[numOutside++] = &v;
	}

	// All points outside of the plane - discard tri
	if (numInside == 0)
		return ClipResult::DISCARD;

	// All points are inside the plane - tri is all good
	if (numInside == 3)
	{
		out1A = a;
		out1B = b;
		out1C = c;
		return ClipResult::GOOD;
	}

	// One point inside -> clip w/ plane to form a triangle
	if (numInside == 1 && numOutside == 2)
	{
		out1A = *inside[0];
		LinePlaneIntersection(planePos, planeNormal, *inside[0], *outside[0], out1B);
		LinePlaneIntersection(planePos, planeNormal, *inside[0], *outside[1], out1C);

		if (!IsCounterClockwise(out1A, out1B, out1C))
			std::swap(out1A, out1C);

		return ClipResult::ONE_NEW;
	}

	// Two points inside -> clip w/ plane to form a quad
	if (numInside == 2 && numOutside == 1)
	{
		out1A = *inside[0];
		out1B = *inside[1];
		LinePlaneIntersection(planePos, planeNormal, *inside[0], *outside[0], out1C);

		out2A = out1C;
		out2B = *inside[1];
		LinePlaneIntersection(planePos, planeNormal, *inside[1], *outside[0], out2C);

		if (!IsCounterClockwise(out1A, out1B, out1C))
			std::swap(out1A, out1C);

		if (!IsCounterClockwise(out2A, out2B, out2C))
			std::swap(out2A, out2C);

		return ClipResult::TWO_NEW;
	}

	return ClipResult::ERROR;
}

const Renderer::Metrics& Renderer::GetMetrics() const
{
	return m_metrics;
}

void Renderer::ResetMetrics()
{
	m_metrics.renderedMeshes = 0;
	m_metrics.clippedTriangles = 0;
	m_metrics.rasterizedTriangles = 0;
}
