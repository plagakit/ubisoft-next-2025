#include "pch.h"
#include "vector3.h"

template <typename T>
Vector3<T>::Vector3() :
	x(static_cast<T>(0)),
	y(static_cast<T>(0)),
	z(static_cast<T>(0))
{}

template <typename T>
Vector3<T>::Vector3(T x, T y, T z) :
	x(x), y(y), z(z)
{};

template <typename T>
std::string Vector3<T>::ToString() const
{
	return "(" +
		std::to_string(x) + ", " +
		std::to_string(y) + ", " +
		std::to_string(z) + ")";
}

template struct Vector3<float>;
template struct Vector3<int>;