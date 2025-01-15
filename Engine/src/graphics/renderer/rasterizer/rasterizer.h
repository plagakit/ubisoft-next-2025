#pragma once

#include "math/vector/vector4.h"
#include "graphics/color/color.h"
#include "graphics/shading_mode.h"
#include "graphics/texture/texture.h"

// An interface for a rasterizer, any derived classes implement their
// own rasterization methods, the customization almost makes this like
// a fragment shader if you think about it (really hard b/c its a stretch)
class Rasterizer
{
public:
	// Clear any internal memory and prepare for the next frame
	virtual void Clear() = 0;

	// Rasterize a triangle given its screen-space vertex positions, world-space vertex normals, and a color
	virtual void RasterizeTriangle(
		const Vec4& a, const Vec4& b, const Vec4& c, 
		const Vec3& an, const Vec3& bn, const Vec3& cn, 
		const Color& color, ShadingMode mode) = 0;

	// Rasterize a texture given its x, y in screen-space, z in clip space, and w as a scale
	virtual void RasterizeTexture(const Vec4& pos, RID textureHandle) = 0;

	// Flush all rasterize calls and blit to the screen
	virtual void Flush() = 0;
};