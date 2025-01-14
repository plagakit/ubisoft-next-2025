#pragma once

#include "math/vector/vector3.h"
#include "math/quaternion/quat.h"
#include "math/matrix/mat4.h"

struct Transform3D
{
	Vec3 position		= Vec3::ZERO;
	Quat orientation	= Quat::IDENTITY;
	Vec3 scale			= Vec3::ONE;

	Vec3 velocity		= Vec3::ZERO;
	Quat angVelocity	= Quat::IDENTITY;
	Vec3 acceleration	= Vec3::ZERO;

	Mat4 ToMatrix() const;

};