#include "pch.h"
#include "transform_3d.h"

Mat4 Transform3D::ToMatrix() const
{
    Mat4 m = Mat4(1.0f);
    m.Scale(scale);
    m.Rotate(orientation);
    m.Translate(position);
    return m;
}
