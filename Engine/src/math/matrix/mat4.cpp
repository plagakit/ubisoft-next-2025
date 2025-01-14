#include "pch.h"
#include "mat4.h"

#include "math/matrix/mat4.h"
#include "math/math_utils.h"

const Mat4 Mat4::IDENTITY = Mat4(1.0f);

Mat4::Mat4() :
	m{ 0.0f }
{}

Mat4::Mat4(float diagonal) :
	m{ 0.0f }
{
	m[0] = diagonal;
	m[5] = diagonal;
	m[10] = diagonal;
	m[15] = diagonal;
}

Mat4::Mat4(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8, float m9, float m10, float m11, float m12, float m13, float m14, float m15)
{
	// man....
	m[0] = m0;
	m[1] = m1;
	m[2] = m2;
	m[3] = m3;
	m[4] = m4;
	m[5] = m5;
	m[6] = m6;
	m[7] = m7;
	m[8] = m8;
	m[9] = m9;
	m[10] = m10;
	m[11] = m11;
	m[12] = m12;
	m[13] = m13;
	m[14] = m14;
	m[15] = m15;
}

float& Mat4::operator()(size_t row, size_t col)
{
	return m[col * 4 + row];
}

float Mat4::operator()(size_t row, size_t col) const
{
	return m[col * 4 + row];
}

float Mat4::operator[](size_t idx) const
{
	return m[idx];
}

float& Mat4::operator[](size_t idx)
{
	return m[idx];
}

Mat4 Mat4::operator*(const Mat4& n) const
{
	//Mat4 out;
	//for (int i = 0; i < 4; i++)
	//	for (int k = 0; k < 4; k++)
	//		for (int j = 0; j < 4; j++)
	//			out(i, j) += (*this)(i, k) * n(k, j);

	return Mat4(
		m[0]*n[0] + m[4]*n[1] + m[8]*n[2] + m[12]*n[3],
		m[1]*n[0] + m[5]*n[1] + m[9]*n[2] + m[13]*n[3], 
		m[2]*n[0] + m[6]*n[1] + m[10]*n[2] + m[14]*n[3], 
		m[3]*n[0] + m[7]*n[1] + m[11]*n[2] + m[15]*n[3],
		m[0]*n[4] + m[4]*n[5] + m[8]*n[6] + m[12]*n[7], 
		m[1]*n[4] + m[5]*n[5] + m[9]*n[6] + m[13]*n[7], 
		m[2]*n[4] + m[6]*n[5] + m[10]*n[6] + m[14]*n[7], 
		m[3]*n[4] + m[7]*n[5] + m[11]*n[6] + m[15]*n[7],
		m[0]*n[8] + m[4]*n[9] + m[8]*n[10] + m[12]*n[11], 
		m[1]*n[8] + m[5]*n[9] + m[9]*n[10] + m[13]*n[11], 
		m[2]*n[8] + m[6]*n[9] + m[10]*n[10] + m[14]*n[11], 
		m[3]*n[8] + m[7]*n[9] + m[11]*n[10] + m[15]*n[11],
		m[0]*n[12] + m[4]*n[13] + m[8]*n[14] + m[12]*n[15], 
		m[1]*n[12] + m[5]*n[13] + m[9]*n[14] + m[13]*n[15],
		m[2]*n[12] + m[6]*n[13] + m[10]*n[14] + m[14]*n[15], 
		m[3]*n[12] + m[7]*n[13] + m[11]*n[14] + m[15]*n[15]
	);
}

Vec4 Mat4::operator*(const Vec4& v) const
{
	return Vec4(
		m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w,
		m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w,
		m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w,
		m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w
	);
}

Mat4& Mat4::Translate(const Vec3& offset)
{
	// Translate basis 1
	m[0] += m[3] * offset.x;        
	m[1] += m[3] * offset.y;        
	m[2] += m[3] * offset.z;        

	// Translate basis 2
	m[4] += m[7] * offset.x;
	m[5] += m[7] * offset.y;
	m[6] += m[7] * offset.z;

	// Translate basis 3
	m[8] += m[11] * offset.x;
	m[9] += m[11] * offset.y;
	m[10] += m[11] * offset.z;

	// Translate basis 4
	m[12] += m[15] * offset.x;
	m[13] += m[15] * offset.y;
	m[14] += m[15] * offset.z;

	return *this;
}

Mat4& Mat4::Rotate(const Quat& q)
{
	// TODO: is there an better way to do this? maybe look into how glm does it
	(*this) = q.ToRotationMatrix() * (*this);
	return *this;
}

Mat4& Mat4::Rotate(const Vec3& axis, float angle)
{
	// https://ai.stackexchange.com/questions/14041/how-can-i-derive-the-rotation-matrix-from-the-axis-angle-rotation-vector

	float cos = cosf(angle * DEG2RAD);
	float sin = sinf(angle * DEG2RAD);
	float invCos = 1.0f - cos;
	Mat4 tm = *this; // this matrix, temporary copy

	float r0 = axis.x * axis.x * invCos + cos;
	float r1 = axis.x * axis.y * invCos + axis.z * sin;
	float r2 = axis.x * axis.z * invCos - axis.y * sin;
	float r4 = axis.x * axis.y * invCos - axis.z * sin;
	float r5 = axis.y * axis.y * invCos + cos;
	float r6 = axis.y * axis.z * invCos + axis.x * sin;
	float r8 = axis.x * axis.z * invCos + axis.y * sin;
	float r9 = axis.y * axis.z * invCos - axis.x * sin;
	float r10 = axis.z * axis.z * invCos + cos;

	m[0] = r0 * tm[0] + r4 * tm[1] + r8 * tm[2];
	m[1] = r1 * tm[0] + r5 * tm[1] + r9 * tm[2];
	m[2] = r2 * tm[0] + r6 * tm[1] + r10 * tm[2];

	m[4] = r0 * tm[4] + r4 * tm[5] + r8 * tm[6];
	m[5] = r1 * tm[4] + r5 * tm[5] + r9 * tm[6];
	m[6] = r2 * tm[4] + r6 * tm[5] + r10 * tm[6];

	m[8] = r0 * tm[8] + r4 * tm[9] + r8 * tm[10];
	m[9] = r1 * tm[8] + r5 * tm[9] + r9 * tm[10];
	m[10] = r2 * tm[8] + r6 * tm[9] + r10 * tm[10];

	m[12] = r0 * tm[12] + r4 * tm[13] + r8 * tm[14];
	m[13] = r1 * tm[12] + r5 * tm[13] + r9 * tm[14];
	m[14] = r2 * tm[12] + r6 * tm[13] + r10 * tm[14];

	return *this;
}

Mat4& Mat4::Scale(const Vec3& factor)
{
	m[0] *= factor.x;        
	m[1] *= factor.y;        
	m[2] *= factor.z;        

	m[4] *= factor.x;
	m[5] *= factor.y;
	m[6] *= factor.z;

	m[8] *= factor.x;
	m[9] *= factor.y;
	m[10] *= factor.z;

	m[12] *= factor.x;
	m[13] *= factor.y;
	m[14] *= factor.z;

	return *this;
}

std::string Mat4::ToString() const
{
	return "[[" + 
		std::to_string(m[0]) + ", " +
		std::to_string(m[4]) + ", " +
		std::to_string(m[8]) + ", " +
		std::to_string(m[12]) + "], [" +
		std::to_string(m[1]) + ", " +
		std::to_string(m[5]) + ", " +
		std::to_string(m[9]) + ", " +
		std::to_string(m[13]) + "], [" +
		std::to_string(m[2]) + ", " +
		std::to_string(m[6]) + ", " +
		std::to_string(m[10]) + ", " +
		std::to_string(m[14]) + "], [" +
		std::to_string(m[3]) + ", " +
		std::to_string(m[7]) + ", " +
		std::to_string(m[11]) + ", " +
		std::to_string(m[15]) + "]]";
}
