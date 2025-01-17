#pragma once

#include "gui/gui_object.h"
#include "graphics/color/color.h"
#include "graphics/font/font.h"
#include <string>

class Label : public GUIObject
{
public:
	Label(const std::string& text, RID fontHandle, Color textColor);
	
	std::string& GetText();

protected:
	void UpdateInternal(float dt) override;
	void RenderInternal(Renderer& renderer, Vec2 origin, Vec2 scale) override;
	
	std::string m_text;
	RID m_fontHandle;
	Color m_textColor;

};