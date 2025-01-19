#pragma once

#include "collider_2d.h"
#include "math/vector/vector2.h"

struct AABB2DCollider : public Collider2D
{
	Vec2 offset		= Vec2::ZERO;
	float width		= 1.0f;
	float height	= 1.0f;

	void DebugDraw(Renderer& renderer, Vec2 position) override;

	DEFINE_COLLIDER2D_VISITOR()
};