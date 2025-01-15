#pragma once

#include "rasterizer.h"
#include "core/resource/resource_manager.h"

class Renderer;

class PaintersRasterizer : public Rasterizer
{
public:
	PaintersRasterizer(Renderer& renderer, ResourceManager& resourceMgr);

	void Clear() override;
	void RasterizeTriangle(const Vec4& a, const Vec4& b, const Vec4& c, const Vec3& an, const Vec3& bn, const Vec3& cn, const Color& color, ShadingMode mode) override;
	void RasterizeTexture(const Vec4& pos, RID textureHandle);
	void Flush() override;

	void SetDoSort(bool doSort);

private:
	struct TrianglePrimitive
	{
		Vec3 a, b, c;
		Color color;
	};

	struct TexturePrimitive
	{
		Vec3 pos;
		float scale;
		RID textureHandle;
	};

	Renderer& m_renderer;
	ResourceManager& m_resourceMgr;

	bool m_doSort;
	std::vector<TrianglePrimitive> m_triangles;
	std::vector<TexturePrimitive> m_textures;

};