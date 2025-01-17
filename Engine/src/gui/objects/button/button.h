#pragma once

#include "gui/gui_object.h"
#include "graphics/color/color.h"
#include "core/signal/signal.h"

class Button : public GUIObject
{
public:
	Button(Dim2 position, Dim2 size, Color default, Color disabled, Color highlighted, Color clicked);

	Signal<> s_Hovered;
	Signal<> s_Unhovered;
	Signal<> s_Clicked;


protected:
	void UpdateInternal(float dt) override;
	void RenderInternal(Renderer& renderer, Vec2 origin, Vec2 scale) override;

	Color m_defaultColor;
	Color m_disabledColor;
	Color m_highlightedColor;
	Color m_clickedColor;
	Color m_currentColor;

	bool m_disabled;
	bool m_isBeingHovered;
	
};