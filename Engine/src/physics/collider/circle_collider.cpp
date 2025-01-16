#include "pch.h"
#include "circle_collider.h"

void CircleCollider::DebugDraw(Renderer& renderer, Vec2 position)
{
	renderer.DrawCircle(position + offset, radius, Color::GREEN);
}

IMPL_COLLIDER2D_VISITOR(CircleCollider)
