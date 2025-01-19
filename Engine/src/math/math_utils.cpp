#include "pch.h"
#include "math_utils.h"

#include "core/app_settings.h"

int Math::Sign(float t)
{
	return t < 0.0f ? -1 : t > 0.0f;
}

Quat Math::Slerp(Quat start, Quat end, float t)
{
	// https://www.lix.polytechnique.fr/Labo/Frank.Nielsen/WEBvisualcomputing/programs/slerp.cpp

	float dot = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;
	float angle, sa, sat, sait, c1, c2;

	angle = acosf(dot);
	if (angle < 0.0f)
		angle = -angle;
	if (angle < EPSILON) // == 0
		return end;

	sa = sinf(angle);
	sat = sinf(t * angle);
	sait = sinf((1 - t) * angle);
	c1 = sait / sa;
	c2 = sat / sa;

	Quat q = {
		c1 * start.x + c2 * end.x,
		c1 * start.y + c2 * end.y,
		c1 * start.z + c2 * end.z,
		c1 * start.w + c2 * end.w
	};
	q.Normalize();

	if (std::isnan(q.w)) 
		return end;

	return q;
}

float Math::RandAngle()
{
    return FRAND_RANGE(0.0f, 2 * PI);
}

Vec2 Math::RandDirection()
{
    float theta = RandAngle();
    return Vec2(cosf(theta), sinf(theta));
}
