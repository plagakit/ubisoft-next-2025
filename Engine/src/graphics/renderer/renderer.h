#pragma once

#include "graphics/color/color.h"
#include "graphics/font/font.h"
#include "math/vector/vector2.h"

class Renderer {

public:
	static void DrawTextLine(float x, float y, const char* text, Color col = Color::WHITE, Font font = defaultFont);

	static void DrawLine(float x0, float y0, float x1, float y1, Color col = Color::WHITE);

	static void DrawRect(float x0, float y0, float x1, float y1, Color col = Color::WHITE);
	static void DrawFilledRect(float x0, float y0, float x1, float y1, Color c = Color::WHITE);

	//static void DrawRectC(float x, float y, float width, float height, Color col = Color::WHITE);
	//static void DrawFilledRectC(float x, float y, float width, float height, Color c = Color::WHITE);

private:
	static Font defaultFont;

};