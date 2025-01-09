#pragma once

#include "core/resource/resource_manager.h"
#include "graphics/color/color.h"
#include "graphics/font/font.h"
#include "math/vector/vector2.h"

class Renderer {

public:
	Renderer(ResourceManager& resourceManager);

	// 2D Drawing Functions

	void DrawTextLine(float x, float y, const char* text, Color col = Color::WHITE);
	void DrawTextLine(float x, float y, const char* text, Color col, RID fontHandle);

	void DrawLine(float x0, float y0, float x1, float y1, Color col = Color::WHITE);

	void DrawRect(float x0, float y0, float x1, float y1, Color col = Color::WHITE);
	void DrawFilledRect(float x0, float y0, float x1, float y1, Color c = Color::WHITE);

	void DrawTexture(float x, float y, RID textureHandle);

	//void DrawRectC(float x, float y, float width, float height, Color col = Color::WHITE);
	//void DrawFilledRectC(float x, float y, float width, float height, Color c = Color::WHITE);

	// 3D Drawing Function

private:
	ResourceManager& m_resourceManager;

	RID m_defaultFontHandle;
	void* m_defaultGlutFont;

};