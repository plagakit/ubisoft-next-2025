#pragma once

#include "math/vector/vector3.h"

struct Color : public Vec3
{
	Color();
	Color(float r, float g, float b);
	Color(const Vec3& v);

	float& r();
	float& g();
	float& b();
	float r() const;
	float g() const;
	float b() const;

	static const Color WHITE;
	static const Color BLACK;
	static const Color GRAY;
	static const Color DARK_GRAY;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
};

