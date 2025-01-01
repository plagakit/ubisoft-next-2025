#pragma once

#include "math/vector/vector4.h"
#include "math/vector/vector3.h"

struct Mat4
{
	float m[4][4];

    Mat4();
    Mat4(float diagonal);

    Mat4 operator*(const Mat4& m) const;
    Vec4 operator*(const Vec4& v) const;
    Vec3 operator*(const Vec3& v) const;

    //void Translate(const Vector3& v);
    //void Rotate(const Vector3& v);
    //void Scale(const Vector3& v);
};