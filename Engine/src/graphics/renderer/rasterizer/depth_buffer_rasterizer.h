#pragma once

#include "rasterizer.h"

class DepthBufferRasterizer : public Rasterizer
{
public:
	DepthBufferRasterizer();

	void Clear() override;
	void RasterizeTriangle(
		const Vec4& a, const Vec4& b, const Vec4& c, 
		const Vec3& an, const Vec3& bn, const Vec3& cn, 
		const Color& color, ShadingMode mode) override;
	void RasterizeTexture(const Vec4& pos, RID textureHandle) override;
	void RasterizeLine(const Vec3& start, const Vec3& end, const Color& color) override;
	void RasterizeSphere(const Vec3& pos, float radius, const Color& color) override;
	void Flush() override;

	void SetClearColor(const Color& color);

private:
	Color m_clearColor;
	std::vector<Color> m_colorBuffer;
	std::vector<float> m_depthBuffer;

	// Draws a line directly into the color buffer using Bresenham's algorithm
	void DrawLineBresenham(int x0, int y0, int x1, int y1, const Color& color);

};