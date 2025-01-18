#include "pch.h"
#include "quat.h"

#include "math/matrix/mat4.h"
#include "math/math_utils.h"
#include "core/debug/assert.h"

Quat::Quat() :
	x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{}

Quat::Quat(const Vec3& v) :
	x(v.x), y(v.y), z(v.z), w(1.0f)
{}

Quat::Quat(float x, float y, float z, float w) :
	x(x), y(y), z(z), w(w)
{}

Quat Quat::operator*(float scalar) const
{
	Quat q = { x * scalar, y * scalar, z * scalar, w * scalar };
	q.Normalize();
	return q;
}

Vec3 Quat::operator*(const Vec3& v) const
{
	// https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/

	Vec3 axis = Vec3(x, y, z);
	Vec3 t = axis.Cross(v) * 2.0f;
	return v + (t * w) + axis.Cross(t);
}

Quat Quat::operator*(const Quat& q) const
{
	Quat n = {
	w * q.x + x * q.w + y * q.z - z * q.y,
	w * q.y - x * q.z + y * q.w + z * q.x,
	w * q.z + x * q.y - y * q.x + z * q.w,
	w * q.w - x * q.x - y * q.y - z * q.z
	};
	n.Normalize();
	return n;
}

void Quat::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	Normalize();
}

void Quat::operator*=(const Quat& q)
{
	x = w * q.x + x * q.w + y * q.z - z * q.y;
	y = w * q.y - x * q.z + y * q.w + z * q.x;
	z = w * q.z + x * q.y - y * q.x + z * q.w;
	w = w * q.w - x * q.x - y * q.y - z * q.z;
	Normalize();
}

Quat Quat::Inverse() const
{
	return Quat(-x, -y, -z, -w);
}

Mat4 Quat::ToRotationMatrix() const
{
	return {
		1 - 2 * y * y - 2 * z * z,	2 * x * y - 2 * w * z,		2 * x * z + 2 * w * y,				0,
		2 * x * y + 2 * w * z,		1 - 2 * x * x - 2 * z * z,	2 * y * z - 2 * w * x,				0,
		2 * x * z - 2 * w * y,		2 * y * z + 2 * w * x,		1 - 2 * x * x - 2 * y * y,			0,
		0,					0,					0,							1
	};
}

Vec3 Quat::ToEulerAngles() const
{
	// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

	float sinr_cosp = 2 * (w * x + y * z);
	float cosr_cosp = 1 - 2 * (x * x + y * y);
	float siny_cosp = 2 * (w * z + x * y);
	float cosy_cosp = 1 - 2 * (y * y + z * z);
	float sinp = sqrtf(1 + 2 * (w * y - x * z));
	float cosp = sqrtf(1 - 2 * (w * y - x * z));

	return Vec3(
		atan2f(sinr_cosp, cosr_cosp),
		atan2f(siny_cosp, cosy_cosp),
		2.0f * atan2f(sinp, cosp) - HALF_PI
	);
}

void Quat::ToAxisAngle(Vec3& axis, float& angle) const
{
	angle = 2 * acosf(w);
	axis = Vector3(x, y, z);

	float s = sqrtf(1 - w * w);
	if (s > EPSILON) // to avoid division by 0
		axis /= s;
}

//void Quat::ToLookAt(Vec3& eye, Vec3& up) const
//{
//	eye = (*this) * Vec3::FORWARD;
//	up = (*this) * Vec3::UP;
//}

Quat Quat::LookAt(const Vec3& src, const Vec3& dest)
{
	Vec3 forward = (dest - src).Normalized();
	Vec3 up = Vec3::UP;

	// If forward == up
	if (std::abs(forward.Dot(Vec3::UP)) > 1.0f - EPSILON)
		up = Vec3::FORWARD;

	Vec3 right = up.Cross(forward).Normalized();
	up = forward.Cross(right).Normalized();

	Quat lookAtQuat;
	lookAtQuat.w = sqrtf(1.0f + right.x + up.y + forward.z) * 0.5f;
	float wRecip = 1.0f / (4.0f * lookAtQuat.w);
	lookAtQuat.x = (up.z - forward.y) * wRecip;
	lookAtQuat.y = (forward.x - right.z) * wRecip;
	lookAtQuat.z = (right.y - up.x) * wRecip;

	lookAtQuat.Normalize();
	return lookAtQuat;
}

Quat Quat::FromEulerAngles(float y, float z, float x)
{
	float cr = cosf(x * 0.5f);
	float sr = sinf(x * 0.5f);

	float cy = cosf(y * 0.5f);
	float sy = sinf(y * 0.5f);

	float cp = cosf(z * 0.5f);
	float sp = sinf(z * 0.5f);

	Quat q = Quat(
		sr * cp * cy - cr * sp * sy,
		cr * sp * cy + sr * cp * sy,
		cr * cp * sy - sr * sp * cy,
		cr * cp * cy + sr * sp * sy
	);
	return q;
}

Quat Quat::FromAxisAngle(const Vec3& axis, float  angle)
{
	ASSERT_WARN(abs(1 - axis.LengthSq()) < EPSILON, "Quat::FromAxisAngle: Axis should be normalized!");

	float s = sinf(angle / 2);
	return {
		axis.x * s,
		axis.y * s,
		axis.z * s,
		cosf(angle / 2)
	};
}

void Quat::Normalize()
{
	float lenSq = x * x + y * y + z * z + w * w;
	if (abs(1 - lenSq) > EPSILON)
	{
		float len = sqrtf(lenSq);
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}
}

std::string Quat::ToString() const
{
	return std::string();
}

const Quat Quat::IDENTITY = { 0.0f, 0.0f, 0.0f, 1.0f };