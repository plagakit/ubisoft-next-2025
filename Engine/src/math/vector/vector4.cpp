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

// See vector3.cpp's implementation of Vec3::ToCString for
// more info on how these work and why I think these are (probably 99%) safe

//template<>
//const char* Vector4<int>::ToCString() const
//{
//	constexpr size_t BUFFER_SIZE = std::numeric_limits<int>::digits * 4 + 8;
//	static thread_local char buffer[BUFFER_SIZE];
//	snprintf(buffer, BUFFER_SIZE, "(%d, %d, %d, %d)", x, y, z, w);
//	return buffer;
//}
//
//template<>
//const char* Vector4<float>::ToCString() const
//{
//	constexpr size_t BUFFER_SIZE = std::numeric_limits<float>::max_digits10 * 4 + 12; // + 12 for decimal in flt
//	static thread_local char buffer[BUFFER_SIZE];
//	snprintf(buffer, BUFFER_SIZE, "(%f, %f, %f, %f)", x, y, z, w);
//	return buffer;
//}

template struct Vector4<float>;
template struct Vector4<int>;