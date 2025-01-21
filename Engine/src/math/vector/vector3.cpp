#include "pch.h"
#include "vector3.h"

#include "math/vector/vector4.h"
#include "math/math_utils.h"

template<typename T>
Vector3<T>::Vector3(const Vector4<T>& v) :
	x(v.x), y(v.y), z(v.z)
{}

template<typename T>
Vector3<T> Vector3<T>::FromHomogenous(const Vector4<T>&v)
{
	return Vector3<T>(
		v.x / v.w,
		v.y / v.w,
		v.z / v.w
	);
}

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
T Vector3<T>::Dot(const Vector3<T>& v) const
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
T Vector3<T>::Length() const
{
	return static_cast<T>(sqrtf(static_cast<float>(x * x + y * y + z * z)));
}

template<typename T>
T Vector3<T>::LengthSq() const
{
	return x * x + y * y + z * z;
}

template<typename T>
T Vector3<T>::Distance(const Vector3<T>& to) const
{
	return abs((to - *this).Length());
}

template<typename T>
T Vector3<T>::DistanceSq(const Vector3<T>& to) const
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

// The below two specializations use a thread-safe static buffer that prints the vector
// into it so that we can return a non-dangling pointer to the string. We need to specialize
// for the different formatting done by snprintf. thread_local makes sure the buffer
// is per thread, so the strings cannot be overwritten while still being read. BUFFER_SIZE
// the number of bytes needed to accomodate the maximum values for each. *3 for each number,
// +6 for the spaces, brackets, and commas.
//
// After testing, I found out that this does in fact work, but not with Logger. Logger
// evalutes every argument before printing, it doesn't print in a stream like with cout.
// If there's multiple vectors of the same type calling ToCString they will be overwritten.
// Oh well...........

//template<>
//const char* Vector3<int>::ToCString() const
//{
//	constexpr size_t BUFFER_SIZE = std::numeric_limits<int>::digits * 3 + 6;
//	static thread_local char buffer[BUFFER_SIZE];
//	snprintf(buffer, BUFFER_SIZE, "(%d, %d, %d)", x, y, z);
//	return buffer;
//}
//
//template<>
//const char* Vector3<float>::ToCString() const
//{
//	constexpr size_t BUFFER_SIZE = std::numeric_limits<float>::max_digits10 * 3 + 9; // + 9 for decimal in flt
//	static thread_local char buffer[BUFFER_SIZE];
//	snprintf(buffer, BUFFER_SIZE, "(%f, %f, %f)", x, y, z);
//	return buffer;
//}

#define DEFINE_VEC3(name, x, y, z) \
	template<typename T> \
	const Vector3<T> Vector3<T>::name = { static_cast<T>(x), static_cast<T>(y), static_cast<T>(z) }

DEFINE_VEC3(ZERO, 0, 0, 0);
DEFINE_VEC3(ONE, 1, 1, 1);
DEFINE_VEC3(RIGHT, 1, 0, 0);
DEFINE_VEC3(LEFT, -1, 0, 0);
DEFINE_VEC3(UP, 0, 1, 0);
DEFINE_VEC3(DOWN, 0, -1, 0);
DEFINE_VEC3(FORWARD, 0, 0, 1);
DEFINE_VEC3(BACK, 0, 0, -1);

#undef DEFINE_VEC3

template struct Vector3<float>;
template struct Vector3<int>;