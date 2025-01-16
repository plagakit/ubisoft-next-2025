#pragma once

#include "collider_2d.h"
#include "math/vector/vector2.h"

struct CircleCollider : public Collider2D
{
	Vec2 offset		= Vec2::ZERO;
	float radius	= 1.0f;

	void DebugDraw(Renderer& renderer, Vec2 position) override;

	DEFINE_COLLIDER2D_VISITOR()
};