#pragma once

#include "core/resource/resource_manager.h"
#include "graphics/color/color.h"
#include "graphics/font/font.h"
#include "math/vector/vector2.h"
#include "math/vector/vector3.h"
#include "math/vector/vector4.h"
#include "math/matrix/mat4.h"
#include "components/2d/transform_2d.h"
#include "components/3d/transform_3d.h"
#include "components/3d/mesh_instance.h"
#include "graphics/renderer/rasterizer/depth_buffer_rasterizer.h"
#include "graphics/renderer/rasterizer/painters_rasterizer.h"

#include <vector>


class Renderer {

public:
	Renderer(ResourceManager& resourceManager);

	// Basic, platform-dependant, straight-to-screen drawing functions

	void DrawScreenLine(float x0, float y0, float x1, float y1, Color col);
	void DrawScreenLine(float x0, float y0, float x1, float y1, Color col, int thickness);
	void DrawScreenTextLine(float x, float y, const char* text, Color col, RID fontHandle);
	void DrawScreenTexture(float x, float y, float angle, float scale, RID textureHandle, Color modulate);

	// 2D Drawing Functions

	void Draw2DLine(const Vec2& a, const Vec2& b, Color col = Color::WHITE);
	void Draw2DLine(const Vec2& a, const Vec2& b, Color col, int thickness);

	void DrawTextLine(const Vec2& pos, const std::string& text, Color col = Color::WHITE);
	void DrawTextLine(const Vec2& pos, const std::string& text, Color col, RID fontHandle);
	
	void DrawRect(const Vec2& min, const Vec2& max, Color col = Color::WHITE);
	void DrawRect(const Vec2& pos, float width, float height, Color col = Color::WHITE);
	void DrawFilledRect(const Vec2& min, const Vec2& max, Color c = Color::WHITE);
	void DrawFilledRect(const Vec2& pos, float width, float height, Color col = Color::WHITE);

	void DrawCircle(const Vec2& pos, float radius, Color col = Color::WHITE);
	void DrawCircle(const Vec2& pos, float radius, Color col, int segments);

	void DrawTexture(const Transform2D& tf, RID textureHandle, Color modulate = Color::WHITE);

	// TODO: change this to Mat3
	void Set2DView(const Transform2D& view);

	// 3D Drawing Functions

	void ClearDepthRasterizer();
	void ClearPaintersRasterizer();

	void DrawMesh(const Mat4& model, const MeshInstance& meshInstance);

	void Draw3DLine(const Vec3& start, const Vec3& end, const Color& color);
	void DrawSphere(const Vec3& pos, float radius, Color col = Color::WHITE);
	void Draw3DTexture(const Vec3& pos, float scale, RID textureHandle);

	void FlushDepthRasterizer();
	void FlushPaintersRasterizer();

	void SetViewMatrix(const Mat4& view);
	void SetProjectionMatrix(const Mat4& projection);
	void SetNearPlane(const Vec3& pos, const Vec3& normal);
	void SetClearColor(const Color& color);

	// 3D Drawing Utils

	static float Triangle2DArea(const Vec4& a, const Vec4& b, const Vec4& c);
	static bool IsCounterClockwise(const Vec4& a, const Vec4& b, const Vec4& c);

	static float LinePlaneIntersection(
		const Vec3& planePos, const Vec3& planeNormal,
		const Vec3& lineStart, const Vec3& lineEnd, 
		Vec4& intersection);

	enum ClipResult
	{
		DISCARD,
		GOOD,
		ONE_NEW,
		TWO_NEW,
		ERROR
	};

	static ClipResult ClipTriangleAgainstPlane(
		const Vec3& planePos, const Vec3& planeNormal,
		Vec4& a, Vec4& b, Vec4& c,
		Vec4& out1A, Vec4& out1B, Vec4& out1C,
		Vec4& out2A, Vec4& out2B, Vec4& out2C);

private:
	ResourceManager& m_resourceManager;

	RID m_defaultFontHandle;
	void* m_defaultGlutFont;

	Transform2D m_2Dview;

	// Lots of cached matrices, but it's worth it....
	Mat4 m_view;
	Mat4 m_projection;
	Mat4 m_VP;
	Mat4 m_invVP;

	Vec3 m_nearPlanePos;
	Vec3 m_nearPlaneNormal;

	// I think it's funny how I'm simulating a GPU a little bit by naming it VRAM hehe
	// DrawMesh calls will copy over their mesh data into the VRAM arrays for processing, 
	// since we need local storage. We can avoid copying into arrs by iterating over 
	// triangle data, but then we lose the benefit of reusing already computed vertices.
	//
	// Is it a benefit? I think that would need profiling, but this is a nice
	// opportunity to learn how by implementing it myself and comparing to
	// other projects where I didn't do it like this! :D

	// 10k * (16 + 12 + 4) bytes ~ 320 kB of memory for meshes
	static constexpr size_t VRAM_ARR_SIZE = 10000;
	std::vector<Vec4> m_vertexVRAM;
	std::vector<Vec3> m_normalVRAM;
	std::vector<unsigned int> m_indexVRAM;

	DepthBufferRasterizer m_depthRaster;
	PaintersRasterizer m_paintersRaster;

};