#include "pch.h"
#include "Vector2.h"

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

template <typename T>
std::string Vector2<T>::ToString() const
{
	return "(" +
		std::to_string(x) + ", " +
		std::to_string(y) + ")";
}

template struct Vector2<float>;
template struct Vector2<int>;