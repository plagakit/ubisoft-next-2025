#pragma once

#include "math/vector/vector3.h"
struct Mat4; // mat4 depends on quat

struct Quat
{
	float x, y, z, w;
	
	Quat();
	explicit Quat(const Vec3& v);
	Quat(float x, float y, float z, float w);

	Quat operator*(float scalar) const;
	Vec3 operator*(const Vec3& v) const;
	Quat operator*(const Quat& q) const;
	void operator*=(float scalar);
	void operator*=(const Quat& q);

	Quat Inverse() const;
	Mat4 ToRotationMatrix() const;

	// Yaw-pitch-roll sequence of angles
	Vec3 ToEulerAngles() const;
	void ToAxisAngle(Vec3& axis, float& angle) const;
	void ToLookAt(Vec3& eye, Vec3& up) const;

	// Yaw-pitch-roll sequence of angles
	static Quat FromEulerAngles(float roll, float yaw, float pitch);
	static Quat FromAxisAngle(const Vec3& axis, float angle);
	static Quat LookAt(const Vec3& direction, const Vec3& up);


	void Normalize();

	std::string ToString() const;

	static const Quat IDENTITY;

};