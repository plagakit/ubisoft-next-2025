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

	// 2D Drawing Functions

	void DrawTextLine(float x, float y, const char* text, Color col = Color::WHITE);
	void DrawTextLine(float x, float y, const char* text, Color col, RID fontHandle);

	void DrawLine(float x0, float y0, float x1, float y1, Color col = Color::WHITE);

	void DrawRect(float x0, float y0, float x1, float y1, Color col = Color::WHITE);
	void DrawFilledRect(float x0, float y0, float x1, float y1, Color c = Color::WHITE);

	void DrawCircle(Vec2 pos, float radius, Color col = Color::WHITE);
	void DrawCircle(Vec2 pos, float radius, Color col, int segments);

	void DrawTexture(float x, float y, RID textureHandle);
	void DrawTexture(const Transform2D& tf, RID textureHandle);

	//void DrawRectC(float x, float y, float width, float height, Color col = Color::WHITE);
	//void DrawFilledRectC(float x, float y, float width, float height, Color c = Color::WHITE);

	// 3D Drawing Functions

	void ClearMeshRasterizer();
	void ClearTextureRasterizer();

	void DrawMesh(const Mat4& model, const MeshInstance& meshInstance);

	void Draw3DLine(const Vec3& start, const Vec3& end, const Color& color);
	void DrawSphere(const Vec3& pos, float radius, Color col = Color::WHITE);
	void DrawBillboard(const Vec3& pos, float scale, RID textureHandle);

	void FlushMeshes();
	void Flush3DTextures();

	void SetViewMatrix(const Mat4& view);
	void SetProjectionMatrix(const Mat4& projection);
	void SetClearColor(const Color& color);

	// 3D Drawing Utils

	static float Triangle2DArea(const Vec4& a, const Vec4& b, const Vec4& c);
	static bool IsCounterClockwise(const Vec4& a, const Vec4& b, const Vec4& c);

	static float LinePlaneIntersection(
		const Vec3& planePos, const Vec3& planeNormal,
		const Vec3& lineStart, const Vec3& lineEnd, 
		Vec4& intersection);

	//int ClipTriangleAgainstPlane(
	//	const Vec3& planePos, const Vec3& planeNormal,
	//	unsigned int idxA, unsigned int idxB, unsigned int idxC,
	//	Vec4& out1A, Vec4& out1B, Vec4& out1C,
	//	Vec4& out2A, Vec4& out2B, Vec4& out2C) const;

private:
	ResourceManager& m_resourceManager;

	RID m_defaultFontHandle;
	void* m_defaultGlutFont;

	// TODO: cache V * P instead of separate
	Mat4 m_view;
	Mat4 m_projection;

	// I think it's funny how I'm simulating a GPU a little bit by naming it VRAM hehe
	// DrawMesh calls will copy over their mesh data into the VRAM arrays for processing, 
	// since we need local storage. We can avoid copying into arrs by iterating over 
	// triangle data, but then we lose the benefit of reusing already computed vertices.
	//
	// Is it a benefit? I think that would need profiling, but this is a nice
	// opportunity to learn how by implementing it myself and comparing to
	// other projects where I didn't do it like this! :D

	static constexpr size_t VRAM_ARR_SIZE = 30000;
	std::vector<Vec4> m_vertexVRAM;
	std::vector<Vec3> m_normalVRAM;
	std::vector<unsigned int> m_indexVRAM;

	DepthBufferRasterizer m_rasterizer;
	PaintersRasterizer m_txRasterizer;

};