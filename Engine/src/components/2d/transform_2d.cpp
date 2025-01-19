#include "pch.h"
#include "transform_2d.h"

Vec2 Transform2D::Transform(const Vec2& v) const
{
    return (v * scale).Rotated(rotation) + position;
}
