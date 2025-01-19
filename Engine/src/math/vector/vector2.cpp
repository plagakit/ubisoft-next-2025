#include "pch.h"
#include "Vector2.h"

#include "math/math_utils.h"
#include "core/debug/assert.h"

template <typename T>
Vector2<T>::Vector2() :
	x(static_cast<T>(0)),
	y(static_cast<T>(0))
{}

template <typename T>
Vector2<T>::Vector2(T x, T y) :
	x(x), y(y)
{};

template <typename T>
Vector2<T> Vector2<T>::operator-() const
{
	return Vector2(-x, -y);
}

template <typename T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& v) const
{
	return Vector2(x + v.x, y + v.y);
}

template <typename T>
Vector2<T> Vector2<T>::operator-(const Vector2& v) const
{
	return Vector2(x - v.x, y - v.y);
}

template <typename T>
Vector2<T> Vector2<T>::operator*(T scalar) const
{
	return Vector2(x * scalar, y * scalar);
}

template <typename T>
Vector2<T> Vector2<T>::operator*(const Vector2& v) const
{
	return Vector2(x * v.x, y * v.y);
}

template <typename T>
Vector2<T> Vector2<T>::operator/(T scalar) const
{
	return Vector2(x / scalar, y / scalar);
}

template <typename T>
Vector2<T> Vector2<T>::operator/(const Vector2& v) const
{
	return Vector2(x / v.x, y / v.y);
}

template <typename T>
Vector2<T>& Vector2<T>::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator*=(T scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator*=(const Vector2& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator/=(T scalar)
{
	x /= scalar;
	y /= scalar;
	return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator/=(const Vector2& v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}

template<typename T>
bool Vector2<T>::Equals(const Vector2& v) const
{
	return std::abs(x - v.x) < EPSILON
		&& std::abs(y - v.y) < EPSILON;
}

template<typename T>
T Vector2<T>::Dot(const Vector2& v) const
{
	return x * v.x + y * v.y;
}

template<typename T>
T Vector2<T>::Length() const
{
	return static_cast<T>(sqrtf(static_cast<float>(x * x + y * y)));
}

template<typename T>
T Vector2<T>::LengthSq() const
{
	return x * x + y * y;
}

template<typename T>
T Vector2<T>::Distance(const Vector2& to) const
{
	return abs((to - *this).Length());
}

template<typename T>
T Vector2<T>::DistanceSq(const Vector2& to) const
{
	return abs((to - *this).LengthSq());
}

template<typename T>
Vector2<T> Vector2<T>::Normalized() const
{
	T len = Length();
	if (len > static_cast<T>(0))
		return (*this) / Length();
	return Vector2<T>::ZERO;
}

template<typename T>
Vector2<T> Vector2<T>::ProjectOnto(const Vector2<T>& v) const
{
	return v * (Dot(v) / v.LengthSq());
}

template<typename T>
Vector2<T> Vector2<T>::Rotated(float angle) const
{
	T cos = static_cast<T>(cosf(angle));
	T sin = static_cast<T>(sinf(angle));
	T rx = x * cos - y * sin;
	T ry = x * sin + y * cos;
	return Vector2<T>(rx, ry);
}

template <typename T>
std::string Vector2<T>::ToString() const
{
	return "(" +
		std::to_string(x) + ", " +
		std::to_string(y) + ")";
}

// See vector3.cpp's implementation of Vec3::ToCString for
// more info on how these work and why I think these are (probably 99%) safe

//template<>
//const char* Vector2<int>::ToCString() const
//{
//	constexpr size_t BUFFER_SIZE = std::numeric_limits<int>::digits * 2 + 4;
//	static thread_local char buffer[BUFFER_SIZE];
//	snprintf(buffer, BUFFER_SIZE, "(%d, %d)", x, y);
//	return buffer;
//}
//
//template<>
//const char* Vector2<float>::ToCString() const
//{
//	constexpr size_t BUFFER_SIZE = std::numeric_limits<float>::max_digits10 * 2 + 6;
//	static thread_local char buffer[BUFFER_SIZE];
//	snprintf(buffer, BUFFER_SIZE, "(%f, %f)", x, y);
//	return buffer;
//}

template<typename T>
const Vector2<T> Vector2<T>::ZERO = { static_cast<T>(0), static_cast<T>(0) };

template<typename T>
const Vector2<T> Vector2<T>::ONE = { static_cast<T>(1), static_cast<T>(1) };


template struct Vector2<float>;
template struct Vector2<int>;