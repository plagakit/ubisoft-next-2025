#pragma once

#include "rasterizer.h"

class PaintersRasterizer : public Rasterizer
{
public:
	void Clear() override;
	void RasterizeTriangle(const Vec4& a, const Vec4& b, const Vec4& c, const Vec3& an, const Vec3& bn, const Vec3& cn, const Color& color, ShadingMode mode) override;
	void Flush() override;

	void SetDoSort(bool doSort);

private:
	struct Triangle
	{
		Vec3 a, b, c;
		Color color;
	};

	bool m_doSort;
	std::vector<Triangle> m_triangles;

};