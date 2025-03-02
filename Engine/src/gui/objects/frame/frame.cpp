#include "pch.h"
#include "frame.h"

Color& Frame::GetColor()
{
	return m_color;
}

void Frame::UpdateInternal(float dt)
{

}

void Frame::RenderInternal(Renderer& renderer, Vec2 origin, Vec2 scale)
{
	renderer.DrawFilledRect(origin, origin + scale, m_color);
}
