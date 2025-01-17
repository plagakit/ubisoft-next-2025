#include "pch.h"
#include "gui_root.h"

#include "core/app_settings.h"

GUIRoot::GUIRoot()
{
	m_origin = Vec2(0, 0);
	m_scale = Vec2(APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT);
}

void GUIRoot::UpdateGUI(float dt)
{
	for (auto& child : m_children)
		child->Update(dt);
}

void GUIRoot::RenderGUI(Renderer& renderer)
{
	for (auto& child : m_children)
		child->Render(renderer, m_origin, m_scale);
}

void GUIRoot::UpdateInternal(float dt) {}
void GUIRoot::RenderInternal(Renderer& renderer, Vec2 absPos, Vec2 absSize) {}
