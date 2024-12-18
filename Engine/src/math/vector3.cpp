#include "vector3.h"

template <typename T>
std::string Vector3<T>::ToString() const
{
	return "(" +
		std::to_string(x) + ", " +
		std::to_string(y) + ", " +
		std::to_string(z) + ")";
}

// Explicity instantiate the templated types
template struct Vector3<float>;
template struct Vector3<int>;