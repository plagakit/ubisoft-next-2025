#include "pch.h"
#include "mat4.h"

Mat4::Mat4()
{
}

Mat4::Mat4(float diagonal)
{
}

Mat4 Mat4::operator*(const Mat4& other) const
{
	// TODO: unroll this
	
	// https://lemire.me/blog/2024/06/13/rolling-your-own-fast-matrix-multiplication-loop-order-and-vectorization/
	Mat4 out;
	for (int i = 0; i < 4; i++)
		for (int k = 0; k < 4; k++)
			for (int j = 0; j < 4; j++)
				out.m[i][j] += m[i][k] * other.m[k][j];
	return out;
}

Vec4 Mat4::operator*(const Vec4& v) const
{
	return Vec4(
		v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0],
		v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1],
		v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2],
		v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + m[3][3]
	);
}

Vec3 Mat4::operator*(const Vec3& v) const
{
	Vec3 o = Vec3(
		v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0],
		v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1],
		v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2]
	);

	// "Normalizing" w back to 1
	float w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + m[3][3];
	if (w != 0.0f)
	{
		o.x /= w;
		o.y /= w;
		o.z /= w;
	}

	return o;
}
