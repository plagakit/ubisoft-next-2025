#pragma once

#include <string>

template<typename T>
struct Vector4;

template <typename T>
struct Vector3
{
	T x, y, z;

	Vector3();
	Vector3(T x, T y, T z);
	Vector3(const Vector4<T>& v);

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

	float Dot(const Vector3& v) const;
	Vector3 Cross(const Vector3& v) const;
	Vector3 ProjectOnto(const Vector3& v) const;

	float Length() const;
	float LengthSq() const;
	float Distance(const Vector3& to) const;
	float DistanceSq(const Vector3& to) const;

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
};

using Vec3 = Vector3<float>;
using IVec3 = Vector3<int>;
