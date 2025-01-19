#include "pch.h"
#include "aabb_2d_collider.h"

void AABB2DCollider::DebugDraw(Renderer& renderer, Vec2 position)
{
	renderer.DrawRect(position, width, height, Color::GREEN);

	//float hx = width * 0.5f;
	//float hy = height * 0.5f;

	//float lx = position.x - width * 0.5f;
	//float rx = position.x + width * 0.5f;
	//float by = position.y - height * 0.5f;
	//float ty = position.y + height * 0.5f;

	//renderer.DrawLine(lx, ty, rx, ty, Color::GREEN);
	//renderer.DrawLine(lx, by, rx, by, Color::GREEN);
	//renderer.DrawLine(lx, by, lx, ty, Color::GREEN);
	//renderer.DrawLine(rx, by, rx, ty, Color::GREEN);
}

IMPL_COLLIDER2D_VISITOR(AABB2DCollider)
