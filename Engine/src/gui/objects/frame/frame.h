#pragma once

#include "gui/gui_object.h"

class Frame : public GUIObject
{
protected:
	void UpdateInternal(float dt) override;
	void RenderInternal(Renderer& renderer, Vec2 origin, Vec2 scale) override;
};