#include "pch.h"
#include "vector4.h"

template<typename T>
Vector4<T>::Vector4() :
	x(static_cast<T>(0)),
	y(static_cast<T>(0)),
	z(static_cast<T>(0)),
	w(static_cast<T>(0))
{}

template<typename T>
Vector4<T>::Vector4(Vector3<T> v) :
	x(v.x), y(v.y), z(v.z), w(static_cast<T>(1))
{}

template<typename T>
Vector4<T>::Vector4(T x, T y, T z, T w) :
	x(x), y(y), z(z), w(w)
{}

template <typename T>
std::string Vector4<T>::ToString() const
{
	return "(" +
		std::to_string(x) + ", " +
		std::to_string(y) + ", " +
		std::to_string(z) + ", " +
		std::to_string(w) + ")";
}

template struct Vector4<float>;
template struct Vector4<int>;