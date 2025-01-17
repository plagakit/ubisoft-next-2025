#pragma once

#include "gui/gui_object.h"
#include "graphics/color/color.h"

class Frame : public GUIObject
{
public:
	Color& GetColor();

protected:
	void UpdateInternal(float dt) override;
	void RenderInternal(Renderer& renderer, Vec2 origin, Vec2 scale) override;

	Color m_color;

};