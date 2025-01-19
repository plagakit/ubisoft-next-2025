#include "pch.h"
#include "label.h"

// TODO: overhaul this entirely
// I want to only change the size on text change

Label::Label(const std::string& text, RID fontHandle, Color textColor) :
	m_text(text), m_fontHandle(fontHandle), m_textColor(textColor)
{
	//m_anchor = Vec2(0.5f, 0.5f);
}

std::string& Label::GetText()
{
	return m_text;
}

void Label::UpdateInternal(float dt)
{

}

void Label::RenderInternal(Renderer& renderer, Vec2 origin, Vec2 scale)
{
	renderer.DrawTextLine(origin, m_text, m_textColor, m_fontHandle);
}
