#pragma once

#include "rasterizer.h"
#include "core/resource/resource_manager.h"
#include <variant>

class Renderer;

class PaintersRasterizer : public Rasterizer
{
public:
	PaintersRasterizer(Renderer& renderer, ResourceManager& resourceMgr);

	void ReserveMemory(size_t primitives);

	void Clear() override;
	void RasterizeTriangle(const Vec4& a, const Vec4& b, const Vec4& c, const Vec3& an, const Vec3& bn, const Vec3& cn, const Color& color, ShadingMode mode) override;
	void RasterizeTexture(const Vec4& pos, RID textureHandle) override;
	void RasterizeLine(const Vec3& start, const Vec3& end, const Color& color) override;
	void RasterizeSphere(const Vec3& pos, float radius, const Color& color) override;
	void Flush() override;

	void SetDoSort(bool doSort);

private:

	Renderer& m_renderer;
	ResourceManager& m_resourceMgr;
	bool m_doSort;

	// We avoid using virtual functions because polymorphism would 
	// require us storing the raster queue as a list of Primitive 
	// pointers, which would need heap allocation. I want to avoid
	// heap operations during a tight render loop since both
	// creation and iteration would be slow, so instead I use
	// std::variant to keep everything nice and contiguous while
	// still letting me a get a "polymorphic" GetDepth().
	//
	// Resources:
	// https://levelup.gitconnected.com/understanding-std-visit-in-c-a-type-safe-way-to-traverse-variant-objects-dbeff9b47003

	struct TrianglePrimitive
	{
		Vec3 a, b, c;
		Color color;

		float GetDepth() const;
		void Render(Renderer&);
	};

	struct LinePrimitive
	{
		Vec3 start, end;
		Color color;

		float GetDepth() const;
		void Render(Renderer&);
	};

	struct TexturePrimitive
	{
		Vec3 pos;
		float scale;
		RID textureHandle;

		float GetDepth() const;
		void Render(Renderer&);
	};

	struct SpherePrimitive
	{
		Vec3 pos;
		float radius;
		Color color;

		float GetDepth() const;
		void Render(Renderer&);
	};

	using Primitive = std::variant<TrianglePrimitive, LinePrimitive, TexturePrimitive, SpherePrimitive>;

	std::vector<Primitive> m_primitives;

};