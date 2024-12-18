#pragma once

#include <common.h>

template <typename T>
struct Vector3
{
	T x;
	T y;
	T z;

	std::string ToString() const;
};

using Vec3 = Vector3<float>;
using IVec3 = Vector3<int>;
