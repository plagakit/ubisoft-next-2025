#include "pch.h"
#include "color.h"

const Color Color::WHITE		= { 1.0f, 1.0f, 1.0f };
const Color Color::BLACK		= { 0.0f, 0.0f, 0.0f };
const Color Color::DARK_GRAY	= { 0.33f, 0.33f, 0.33f };
const Color Color::GRAY			= { 0.66f, 0.66f, 0.66f };
const Color Color::RED			= { 1.0f, 0.0f, 0.0f };
const Color Color::GREEN		= { 0.0f, 1.0f, 0.0f };
const Color Color::BLUE			= { 0.0f, 0.0f, 1.0f };

const Color Color::CYAN			= { 0.0f, 1.0f, 1.0f };
const Color Color::MAGENTA		= { 1.0f, 0.0f, 1.0f };
const Color Color::YELLOW		= { 1.0f, 1.0f, 0.0f };


Color::Color() :
	Vec3(0.0f, 0.0f, 0.0f)
{}

Color::Color(float r, float g, float b) :
	Vec3(r, g, b)
{}

Color::Color(const Vec3 & v) :
	Vec3(v)
{}

float& Color::r()
{
	return x;
}

float& Color::g()
{
	return y;
}

float& Color::b()
{
	return z;
}

float Color::r() const
{
	return x;
}

float Color::g() const
{
	return y;
}

float Color::b() const
{
	return z;
}
