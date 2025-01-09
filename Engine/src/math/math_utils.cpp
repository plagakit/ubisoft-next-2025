#include "pch.h"
#include "math_utils.h"

#include "core/app_settings.h"

float Math::RandAngle()
{
    return FRAND_RANGE(0.0f, 2 * PI);
}

Vec2 Math::RandDirection()
{
    float theta = RandAngle();
    return Vec2(cosf(theta), sinf(theta));
}
