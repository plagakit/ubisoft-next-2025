#pragma once

#include <string>

template <typename T>
struct Vector2
{
	T x, y;

	Vector2();
	Vector2(T x, T y);

	Vector2 operator-() const;
	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator*(T scalar) const;
	Vector2 operator*(const Vector2& v) const;
	Vector2 operator/(T scalar) const;
	Vector2 operator/(const Vector2& v) const;

	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(T scalar);
	Vector2& operator*=(const Vector2& v);
	Vector2& operator/=(T scalar);
	Vector2& operator/=(const Vector2& v);

	bool Equals(const Vector2& v) const;

	T Dot(const Vector2& v) const;
	T Length() const;
	T LengthSq() const;
	T Distance(const Vector2& to) const;
	T DistanceSq(const Vector2& to) const;
	Vector2 Normalized() const;

	static const Vector2 ZERO;
	static const Vector2 ONE;

	std::string ToString() const;

};

using Vec2 = Vector2<float>;
using IVec2 = Vector2<int>;
