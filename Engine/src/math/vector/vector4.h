#pragma once

#include "math/vector/vector3.h"
#include <string>

template <typename T>
struct Vector4
{
	T x, y, z, w;

	Vector4();
	Vector4(Vector3<T> v);
	Vector4(T x, T y, T z, T w);

	std::string ToString() const;
};

using Vec4 = Vector4<float>;
using IVec4 = Vector4<int>;
