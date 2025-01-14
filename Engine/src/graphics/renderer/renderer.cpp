#include "pch.h"
#include "renderer.h"

#include "core/debug/logger.h"
#include "graphics/texture/texture.h"
#include "graphics/mesh/mesh.h"
#include "math/vector/vector4.h"

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
	m_rasterizedTriangles = 0;
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

	// Model Space -> World Space -> View Space -> Clip Space
	Mat4 MVP = m_projection * m_view * model;
	for (Vec4& p : m_vertexVRAM)
		p = MVP * p;
	
	// Near Culling
	
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

	for (int i = 0; i < indices.size(); i += 6)
	{
		Vec4& a = m_vertexVRAM[indices[i]];
		Vec4& b = m_vertexVRAM[indices[i+2]];
		Vec4& c = m_vertexVRAM[indices[i+4]];

		// Backface culling
		if (IsCounterClockwise(a, b, c))
			continue;

		App::DrawLine(a.x, a.y, b.x, b.y, meshInstance.color.r, meshInstance.color.g, meshInstance.color.b);
		App::DrawLine(b.x, b.y, c.x, c.y, meshInstance.color.r, meshInstance.color.g, meshInstance.color.b);
		App::DrawLine(c.x, c.y, a.x, a.y, meshInstance.color.r, meshInstance.color.g, meshInstance.color.b);
		m_rasterizer.RasterizeTriangle(a, b, c, meshInstance.color);
		m_rasterizedTriangles++;
	}
}

void Renderer::FlushRasterQueue()
{
	Logger::Info("Rasterized Triangles: %d", m_rasterizedTriangles);
	m_rasterizer.Flush();
}

void Renderer::SetViewMatrix(const Mat4& view)
{
	m_view = view;
}

void Renderer::SetProjectionMatrix(const Mat4& projection)
{
	m_projection = projection;
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

void Renderer::ClipTriangleAgainstPlane(const Vec3& planePos, const Vec3& planeNormal)
{
}
