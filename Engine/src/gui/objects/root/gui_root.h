#pragma once

#include "gui/gui_object.h"

class GUIRoot : public GUIObject
{
public:
	GUIRoot();

	void UpdateGUI(float dt);
	void RenderGUI(Renderer& renderer);

protected:
	void UpdateInternal(float dt) override;
	void RenderInternal(Renderer& renderer, Vec2 absPos, Vec2 absSize) override;

};