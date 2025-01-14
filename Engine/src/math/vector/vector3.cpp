#include "pch.h"
#include "vector3.h"

#include "math/math_utils.h"

template <typename T>
Vector3<T>::Vector3() :
	x(static_cast<T>(0)),
	y(static_cast<T>(0)),
	z(static_cast<T>(0))
{}

template <typename T>
Vector3<T>::Vector3(T x, T y, T z) :
	x(x), y(y), z(z)
{}

template<typename T>
Vector3<T> Vector3<T>::operator-() const
{
	return Vector3(-x, -y, -z);
}

template<typename T>
Vector3<T> Vector3<T>::operator+(const Vector3& v) const
{
	return Vector3(
		x + v.x,
		y + v.y,
		z + v.z
	);
}

template<typename T>
Vector3<T> Vector3<T>::operator-(const Vector3& v) const
{
	return Vector3(
		x - v.x,
		y - v.y,
		z - v.z
	);
}

template<typename T>
Vector3<T> Vector3<T>::operator*(T scalar) const
{
	return Vector3(
		x * scalar,
		y * scalar,
		z * scalar
	);
}

template<typename T>
Vector3<T> Vector3<T>::operator*(const Vector3& v) const
{
	return Vector3(
		x * v.x,
		y * v.y,
		z * v.z
	);
}

template<typename T>
Vector3<T> Vector3<T>::operator/(T scalar) const
{
	return Vector3(
		x / scalar,
		y / scalar,
		z / scalar
	);
}

template<typename T>
Vector3<T> Vector3<T>::operator/(const Vector3& v) const
{
	return Vector3(
		x / v.x,
		y / v.y,
		z / v.z
	);
}

template<typename T>
Vector3<T>& Vector3<T>::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator*=(T scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator/=(T scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator/=(const Vector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

// Manually define specializations for equal, float and int are different
template<>
bool Vector3<int>::Equals(const Vector3<int>& v) const
{
	return x == v.x && y == v.y && z == v.z;
}

template<>
bool Vector3<float>::Equals(const Vector3<float>& v) const
{
	return std::abs(x - v.x) < EPSILON
		&& std::abs(y - v.y) < EPSILON
		&& std::abs(z - v.z) < EPSILON;
}


template<typename T>
float Vector3<T>::Dot(const Vector3<T>& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

template<typename T>
Vector3<T> Vector3<T>::Cross(const Vector3<T>& v) const
{
	return Vector3(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}

template<typename T>
Vector3<T> Vector3<T>::ProjectOnto(const Vector3<T>& v) const
{
	return v * (Dot(v) / v.LengthSq());
}

template<typename T>
float Vector3<T>::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}

template<typename T>
float Vector3<T>::LengthSq() const
{
	return x * x + y * y + z * z;
}

template<typename T>
float Vector3<T>::Distance(const Vector3<T>& to) const
{
	return abs((to - *this).Length());
}

template<typename T>
float Vector3<T>::DistanceSq(const Vector3<T>& to) const
{
	return abs((to - *this).LengthSq());
}

template<typename T>
Vector3<T> Vector3<T>::Normalized() const
{
	return *this / Length();
}

template <typename T>
std::string Vector3<T>::ToString() const
{
	return "(" +
		std::to_string(x) + ", " +
		std::to_string(y) + ", " +
		std::to_string(z) + ")";
}

#define DEFINE_VEC3(name, x, y, z) \
	template<typename T> \
	const Vector3<T> Vector3<T>::name = { static_cast<T>(x), static_cast<T>(y), static_cast<T>(z) }

DEFINE_VEC3(ZERO, 0, 0, 0);
DEFINE_VEC3(ONE, 1, 1, 1);
DEFINE_VEC3(LEFT, 1, 0, 0);
DEFINE_VEC3(RIGHT, -1, 0, 0);
DEFINE_VEC3(UP, 0, 1, 0);
DEFINE_VEC3(DOWN, 0, -1, 0);
DEFINE_VEC3(FORWARD, 0, 0, 1);
DEFINE_VEC3(BACK, 0, 0, -1);

#undef DEFINE_VEC3

template struct Vector3<float>;
template struct Vector3<int>;