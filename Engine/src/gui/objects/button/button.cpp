#include "pch.h"
#include "button.h"
#include "core/input/input.h"

Button::Button(const Input& input, Dim2 position, Dim2 size, Color defaultCol, Color disabled, Color highlighted, Color clicked) :
	m_input(input),
    m_defaultColor(defaultCol),
    m_disabledColor(disabled),
    m_highlightedColor(highlighted),
    m_clickedColor(clicked),
    m_currentColor(defaultCol),
    m_disabled(false),
    m_isBeingHovered(false)
{
    m_position = position;
    m_size = size;
}

void Button::UpdateInternal(float dt)
{
    if (m_disabled)
    {
        m_currentColor = m_disabledColor;
        return;
    }

    Vec2 mousePos = m_input.GetMousePos();
    Vec2 pos = GetAbsolutePosition();
    Vec2 scl = GetAbsoluteSize();
    bool inside = mousePos.x >= pos.x && mousePos.y >= pos.y
        && mousePos.x <= pos.x + scl.x
        && mousePos.y <= pos.y + scl.y;

    if (inside)
    {
        if (!m_isBeingHovered)
        {
            m_isBeingHovered = true;
            m_currentColor = m_highlightedColor;
            s_Hovered.Emit(); 
        }

        if (m_input.IsJustPressed("mouse-left"))
        {
            s_Clicked.Emit();
            m_currentColor = m_clickedColor;
        }
        else
            m_currentColor = m_highlightedColor;
    }
    else
    {
        if (m_isBeingHovered)
        {
            m_isBeingHovered = false;
            s_Unhovered.Emit();
            m_currentColor = m_defaultColor;
        }
    }
}

void Button::RenderInternal(Renderer& renderer, Vec2 origin, Vec2 scale)
{
    renderer.DrawFilledRect(origin, origin + scale, m_currentColor);
}
