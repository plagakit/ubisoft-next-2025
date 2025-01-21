#pragma once

#include <string>

template<typename T>
struct Vector4;

template <typename T>
struct Vector3
{
	T x, y, z;

	constexpr Vector3() :
		x(static_cast<T>(0)),
		y(static_cast<T>(0)),
		z(static_cast<T>(0))
	{};

	constexpr Vector3(T x, T y, T z) :
		x(x), y(y), z(z)
	{};

	// Converts by copying values, no homogenous division
	Vector3(const Vector4<T>& v);
	static Vector3 FromHomogenous(const Vector4<T>& v);

	Vector3 operator-() const;
	Vector3 operator+(const Vector3& v) const;
	Vector3 operator-(const Vector3& v) const;
	Vector3 operator*(T scalar) const;
	Vector3 operator*(const Vector3& v) const;
	Vector3 operator/(T scalar) const;
	Vector3 operator/(const Vector3& v) const;

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(T scalar);
	Vector3& operator*=(const Vector3& v);
	Vector3& operator/=(T scalar);
	Vector3& operator/=(const Vector3& v);

	bool Equals(const Vector3& v) const;

	T Dot(const Vector3& v) const;
	Vector3 Cross(const Vector3& v) const;
	Vector3 ProjectOnto(const Vector3& v) const;

	T Length() const;
	T LengthSq() const;
	T Distance(const Vector3& to) const;
	T DistanceSq(const Vector3& to) const;

	Vector3 Normalized() const;

	static const Vector3 ZERO;
	static const Vector3 ONE;
	static const Vector3 LEFT;
	static const Vector3 RIGHT;
	static const Vector3 UP;
	static const Vector3 DOWN;
	static const Vector3 FORWARD;
	static const Vector3 BACK;

	std::string ToString() const;
	//const char* ToCString() const;
};

using Vec3 = Vector3<float>;
using IVec3 = Vector3<int>;
