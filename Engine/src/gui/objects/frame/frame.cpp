#include "pch.h"
#include "frame.h"

void Frame::UpdateInternal(float dt)
{

}

void Frame::RenderInternal(Renderer& renderer, Vec2 origin, Vec2 scale)
{
	renderer.DrawFilledRect(
		origin.x, origin.y,
		origin.x + scale.x, origin.y + scale.y,
		Color::GREEN
	);
}
