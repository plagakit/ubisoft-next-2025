#pragma once

class Rasterizer
{
public:
	virtual void RasterizeTriangle() = 0;
	virtual void Flush() = 0;
};