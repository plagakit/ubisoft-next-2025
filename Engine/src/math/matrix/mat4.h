#pragma once

#include "math/vector/vector4.h"
#include "math/vector/vector3.h"
#include "math/quaternion/quat.h"

/**
* 4x4 matrices, stored in column-major order.
*  -            -
* | 0  4   8  12 |
* | 1  5   9  13 |
* | 2  6  10  14 | 
* | 3  7  11  15 |
*  -            -
*/
struct Mat4
{
    float m[16];

    // Commented out methods are methods that aren't 100% necesary
    // and I didn't have the time to implement them

    Mat4();
    Mat4(float diagonal);
    Mat4(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8, float m9, float m10, float m11, float m12, float m13, float m14, float m15);

    float operator()(size_t row, size_t col) const;
    float& operator()(size_t row, size_t col);
    float operator[](size_t idx) const;
    float& operator[](size_t idx);

    Mat4 operator*(const Mat4& n) const;
    //Mat4& operator*=(const Mat4& n);
    Vec4 operator*(const Vec4& v) const;
    //Vec3 operator*(const Vec3& v) const;

    //Mat4& LookAt(const Vec3& target, const Vec3& up);
    Mat4& Translate(const Vec3& offset);
    Mat4& Rotate(const Quat& q);
    Mat4& Rotate(const Vec3& axis, float angleRad);
    Mat4& Scale(const Vec3& factor);

    Vec3 Right() const;
    Vec3 Up() const;
    Vec3 Front() const;

    std::string ToString() const;

    static const Mat4 IDENTITY;
};