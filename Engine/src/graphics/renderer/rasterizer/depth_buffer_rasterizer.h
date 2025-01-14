#pragma once

#include "rasterizer.h"
#include "math/vector/vector4.h"
#include "graphics/color/color.h"

class DepthBufferRasterizer// : public Rasterizer
{
public:
	DepthBufferRasterizer();

	void Clear();
	void RasterizeTriangle(const Vec4& a, const Vec4& b, const Vec4& c, const Color& color);
	void Flush();

private:
	std::vector<Color> m_colorBuffer;
	std::vector<float> m_depthBuffer;

};