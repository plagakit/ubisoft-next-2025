#pragma once

#include "common.h"

class Renderer {

public:
	static void Text(float x, float y, const char* text);
	static void DrawLine(float x0, float y0, float x1, float y1);
	static void DrawLine(float x0, float y0, float x1, float y1, float r, float g, float b);

};